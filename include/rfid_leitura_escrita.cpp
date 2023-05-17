#include <SPI.h>     // Lib para comunicação SPI
#include <MFRC522.h>
#define PINO_RST 15
#define PINO_SS 5
#define LED_VERD 4
#define LED_VERM 2

#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16

byte bloco = 12;

MFRC522::MIFARE_Key chave;  // Objeto utilizado para autenticação
MFRC522::StatusCode status; // Status com código do retorno da autenticação

//Definicoes pino modulo RC522
MFRC522 mfrc522(PINO_SS, PINO_RST);

int menu();
void ler_dados(byte bloco);
void escrever_dados(byte bloco);
void setup()
{

    Serial.begin(9600); // Configura a porta serial
    SPI.begin();        // Configura a comunicação SPI
    mfrc522.PCD_Init(); // Configura o modulo MFRC522

    /* Configura os pinos de LEDs */
    pinMode(LED_VERD, OUTPUT);
    pinMode(LED_VERM, OUTPUT);

    Serial.println("Aproxime a TAG...");
}

void loop()
{

    // Aguarda a aproximacao do cartao
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    // Seleciona um dos cartoes
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    int comando = menu();
    Serial.println(comando);

    if (comando == 0)
    {
        ler_dados(bloco);
    }
    else if (comando == 1)
    {
        escrever_dados(bloco);
    }
    else
    {
        Serial.println("Opção inválida!");
        return;
    }

    // Pausa a encriptação do PCD.
    // Deve ser chamado após a comunicação com autenticação, caso contrário novas comunicações não poderão ser iniciadas.
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}

void ler_dados(byte bloco)
{

    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); // Imprime os detalhes tecnicos do cartão/tag

    // Prepara a chave
    for (byte i = 0; i < 6; i++)
    {
        chave.keyByte[i] = 0xFF;
    }

    // Dados lidos são armazenados em buffer
    byte buffer[SIZE_BUFFER] = {0};
    byte tamanho = SIZE_BUFFER;

    // Autenticação do bloco que vamos operar
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloco, &chave, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {

        Serial.print("Falha na Autenticação: ");
        Serial.println(mfrc522.GetStatusCodeName(status));

        digitalWrite(LED_VERM, HIGH);
        delay(1000);
        digitalWrite(LED_VERM, LOW);

        return;
    }

    // Caso autenticação for sucesso, faz a leitura dos dados do bloco
    status = mfrc522.MIFARE_Read(bloco, buffer, &tamanho);
    if (status != MFRC522::STATUS_OK)
    {

        Serial.print("Leitura Falhou: ");
        Serial.println(mfrc522.GetStatusCodeName(status));

        digitalWrite(LED_VERM, HIGH);
        delay(1000);
        digitalWrite(LED_VERM, LOW);

        return;
    }
    else
    {
        digitalWrite(LED_VERD, HIGH);
        delay(1000);
        digitalWrite(LED_VERD, LOW);
    }

    Serial.println(">> Dados do bloco [" + (String)bloco + "]");

    // Imprime os dados lidos
    for (int i = 0; i < MAX_SIZE_BLOCK; i++)
    {
        Serial.write(buffer[i]);
    }
    Serial.println(" ");
}

void escrever_dados(byte bloco)
{

    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); // Imprime os detalhes tecnicos do cartão/tag
    Serial.setTimeout(20000L);                        // Pode-se esperar até 20 segundos para entrar os dados

    Serial.println(F("Insira os dados a serem gravados com o caractere '#' ao final (máximo de 16 caracteres):"));

    // Prepara a chave
    for (byte i = 0; i < 6; i++)
    {
        chave.keyByte[i] = 0xFF;
    }

    // Dados a serem escritos são armazenados em buffer
    byte buffer[MAX_SIZE_BLOCK] = "";

    // Realiza a leitura dos dados no "Monitor Serial"
    // Dados são lidos até encontrar o caractere "#"
    byte tamanho = Serial.readBytesUntil('#', (char *)buffer, MAX_SIZE_BLOCK);

    // Espaços que sobrarem do buffer são preenchidos com espaço em branco
    for (byte i = tamanho; i < MAX_SIZE_BLOCK; i++)
    {
        buffer[i] = ' ';
    }

    String buffer_str = (char *)buffer; // transforma os dados em string para imprimir

    // Autenticação do bloco que vamos operar, habilitando uma comunicação segura
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloco, &chave, &(mfrc522.uid));

    if (status != MFRC522::STATUS_OK)
    {

        Serial.print(F("Falha na Autenticação: "));
        Serial.println(mfrc522.GetStatusCodeName(status));

        digitalWrite(LED_VERM, HIGH);
        delay(1000);
        digitalWrite(LED_VERM, LOW);

        return;
    }

    // Grava no bloco
    status = mfrc522.MIFARE_Write(bloco, buffer, MAX_SIZE_BLOCK);

    if (status != MFRC522::STATUS_OK)
    {

        Serial.print(F("Escrita falhou: "));
        Serial.println(mfrc522.GetStatusCodeName(status));

        digitalWrite(LED_VERM, HIGH);
        delay(1000);
        digitalWrite(LED_VERM, LOW);

        return;
    }
    else
    {

        Serial.println(F("Escrita bem sucedida"));

        digitalWrite(LED_VERD, HIGH);
        delay(1000);
        digitalWrite(LED_VERD, LOW);
    }
}

int menu()
{

    Serial.println("Insira a opção:");
    Serial.println("[0] - Leitura");
    Serial.println("[1] - Gravação");

    while (!Serial.available())
    {
    };

    int comando = (int)Serial.read();

    while (Serial.available())
    {
        if (Serial.read() == '\n')
            break;
        Serial.read();
    }

    Serial.println(comando);

    return (comando - 48);
}