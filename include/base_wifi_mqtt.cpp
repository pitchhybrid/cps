#include <WiFi.h>
#include <PubSubClient.h>

    /* CONFIGURAÇÕES DO WIFI */
const char *ssid = "nome_da_rede";  // Nome da rede WiFi
const char *password = "senha_secreta"; // Senha da rede WiFi

/* CONFIGURAÇÕES DO MQTT*/
const char *mqttServer = "broker.hivemq.com"; // Endereço do Broker MQTT
const int mqttPort = 1883;                    // Porta TCP do Broker MQTT
const char *mqttUser = "";                    // Usuário MQTT
const char *mqttPassword = "";                // Senha MQTT

int led = 2;
int potenciometro = 34;

WiFiClient espClient;               // Cliente de Rede WiFi
PubSubClient clientMqtt(espClient); // Cria uma instancia de um cliente MQTT
void callback(char *topic, byte *payload, unsigned int length);

void setup()
{

    // Configuração dos pinos do ESP32
    pinMode(led, OUTPUT);
    pinMode(potenciometro, INPUT);

    Serial.begin(9600);         // Configura a porta serial
    WiFi.begin(ssid, password); // Configura o WiFi

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("Tentando se conectar na rede: ");
        Serial.println(ssid);
    }

    Serial.println("Conectado na Rede WiFi.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    clientMqtt.setServer(mqttServer, mqttPort);
    clientMqtt.setCallback(callback);

    /* Verifica se o cliente está conectado ao Broker */
    while (!clientMqtt.connected())
    {

        Serial.println("Conectando ao Broker MQTT...");

        String clientId = "ESP32Client_" + String(random(0xffff), HEX);
        Serial.println("clientId = " + clientId);

        /* Conecta o cliente ao Broker MQTT.
           Configurações de credenciais e Last Will podem ser configuradas aqui*/
        if (clientMqtt.connect(clientId.c_str(), mqttUser, mqttPassword))
        {

            Serial.println("O cliente " + clientId + " foi conectado com sucesso");
        }
        else
        {

            // Estado do cliente MQTT. Quando a conexão falhar pode ser usado para obter informações sobre a falha
            int clientState = clientMqtt.state();

            Serial.print("Falha ao se conectar. ");
            Serial.println("Estado do cliente: " + (String)clientState);

            delay(2000);
        }
    }

    Serial.print("Tentando enviar a mensagem");

    clientMqtt.publish("unifor/cps/esp32", "Hello from ESP32");
    clientMqtt.subscribe("unifor/cps/led");
}

/* A função de callback é chamada quando uma mensagem chega, devido a assinatura de um tópico.
Ou seja, toda vez que chegar uma mensagem em um tópico inscrito por este cliente, a função
callback será chamada. */
void callback(char *topic, byte *payload, unsigned int length)
{

    Serial.print("Uma mensagem chegou no tópico: ");
    Serial.println(topic);

    Serial.print("Payload: ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }

    if (payload[0] == '0')
    {
        Serial.println("\nDesligando luz");
        digitalWrite(led, LOW);
    }

    if (payload[0] == '1')
    {
        Serial.println("\nLigando luz");
        digitalWrite(led, HIGH);
    }

    Serial.println();
    Serial.println(" -----------------------");
}

void loop()
{

    /* loop: função que deve ser chamada regularmente para permitir que o cliente
    processe as mensagens recebidas e mantenha sua conexão com o Broker.*/
    clientMqtt.loop();

    int leitura = analogRead(potenciometro);
    int res = map(leitura, 0, 4095, 0, 100);

    Serial.println("Valor do Potênciometro: " + (String)res);

    String payload = (String)res;
    clientMqtt.publish("unifor/cps/potenciometro", payload.c_str());

    delay(250);
}