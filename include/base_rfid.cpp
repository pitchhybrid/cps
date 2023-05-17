#include <MFRC522.h>

#define PINO_RST 15
#define PINO_SS 5
#define LED_VERD 4
#define LED_VERM 2
#define BUZZER 33

// Definicoes pino modulo RC522
 MFRC522 mfrc522(PINO_SS, PINO_RST);


void setup()
{

    Serial.begin(9600); // Configura a porta serial
    SPI.begin();        // Configura a comunicação SPI
    mfrc522.PCD_Init(); // Configura o modulo MFRC522

    pinMode(LED_VERM, OUTPUT);
    pinMode(LED_VERD, OUTPUT);
    pinMode(BUZZER, OUTPUT);
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

    // Obtem a UID
    String conteudo = "";

    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        conteudo.concat(String(mfrc522.uid.uidByte[i] < HEX ? " 0" : " "));
        conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    Serial.print(conteudo);

    if (conteudo.substring(1) == "59 7b 15 b3")
    {

        Serial.println(": Acesso Liberado");

        digitalWrite(BUZZER, HIGH);
        delay(250);
        digitalWrite(BUZZER, LOW);

        digitalWrite(LED_VERD, HIGH);
        delay(2000);
        digitalWrite(LED_VERD, LOW);
    }
    else
    {

        Serial.println(": Acesso Negado");

        for (int i = 0; i < 3; i++)
        {

            digitalWrite(LED_VERM, HIGH);
            digitalWrite(BUZZER, HIGH);
            delay(500);

            digitalWrite(LED_VERM, LOW);
            digitalWrite(BUZZER, LOW);
            delay(500);
        }
    }
}