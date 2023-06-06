#include "rfid.h"

void RFID::iniciar()
{
    SPI.begin();
    mfrc522->PCD_Init();
}

bool RFID::aguardar_cartao()
{
    return mfrc522->PICC_IsNewCardPresent();
}

String RFID::ler_id()
{
    String content = "";
    if (!mfrc522->PICC_ReadCardSerial())
    {
        return content;
    }
    for (byte i = 0; i < mfrc522->uid.size; i++)
    {
        content.concat(String(mfrc522->uid.uidByte[i] < HEX ? " 0" : " "));
        content.concat(String(mfrc522->uid.uidByte[i], HEX));
    }
    return content.isEmpty() ? content : content.substring(1);
}

String RFID::ler_dados(byte bloco)
{

    MFRC522::StatusCode status;

    mfrc522->PICC_DumpDetailsToSerial(&(mfrc522->uid)); // Imprime os detalhes tecnicos do cartão/tag

    // Prepara a chave
    for (byte i = 0; i < 6; i++)
    {
        chave.keyByte[i] = 0xFF;
    }

    // Dados lidos são armazenados em buffer
    byte buffer[SIZE_BUFFER] = {0};
    byte tamanho = SIZE_BUFFER;

    // Autenticação do bloco que vamos operar
    status = mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloco, &chave, &(mfrc522->uid));
    if (status != MFRC522::STATUS_OK)
    {

        Serial.print("Falha na Autenticação: ");
        Serial.println(mfrc522->GetStatusCodeName(status));
        return "";
    }

    // Caso autenticação for sucesso, faz a leitura dos dados do bloco
    status = mfrc522->MIFARE_Read(bloco, buffer, &tamanho);
    if (status != MFRC522::STATUS_OK)
    {

        Serial.print("Leitura Falhou: ");
        Serial.println(mfrc522->GetStatusCodeName(status));
        return "";
    }
    return String((char *)buffer);
}

void RFID::escrever_dados(byte bloco)
{
    MFRC522::StatusCode status;

    mfrc522->PICC_DumpDetailsToSerial(&(mfrc522->uid)); // Imprime os detalhes tecnicos do cartão/tag
    Serial.setTimeout(20000L);                          // Pode-se esperar até 20 segundos para entrar os dados

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
    status = mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloco, &chave, &(mfrc522->uid));

    if (status != MFRC522::STATUS_OK)
    {

        Serial.print(F("Falha na Autenticação: "));
        Serial.println(mfrc522->GetStatusCodeName(status));

        return;
    }

    // Grava no bloco
    status = mfrc522->MIFARE_Write(bloco, buffer, MAX_SIZE_BLOCK);

    if (status != MFRC522::STATUS_OK)
    {

        Serial.print(F("Escrita falhou: "));
        Serial.println(mfrc522->GetStatusCodeName(status));
        return;
    }
    else
    {
        Serial.println(F("Escrita bem sucedida"));
    }
}