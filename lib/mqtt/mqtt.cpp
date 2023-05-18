#include <mqtt.h>

void MQTT::iniciar(const char *ssid, const char *pass)
{
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("Tentando se conectar na rede: ");
        Serial.println(ssid);
    }
    Serial.println("Conectado na Rede WiFi.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
};

void MQTT::iniciar_broker(const char *server, const u_int port, const char *user, const char *pass)
{
    WiFiClient wiFiClient;
    this->clientMqtt = PubSubClient(wiFiClient);

    clientMqtt.setServer(server, port);

    while (!clientMqtt.connected())
    {

        Serial.println("Conectando ao Broker MQTT...");

        String clientId = "ESP32Client_" + String(random(0xffff), HEX);
        Serial.println("clientId = " + clientId);

        /* Conecta o cliente ao Broker MQTT.
           Configurações de credenciais e Last Will podem ser configuradas aqui*/
        if (clientMqtt.connect(clientId.c_str(), user, pass))
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
}

void MQTT::set_callback(MQTT_CALLBACK_SIGNATURE)
{
    this->clientMqtt.setCallback(callback);
}

boolean MQTT::publish(const char *topic, const char *payload)
{
    return this->clientMqtt.publish(topic, payload);
}

boolean MQTT::subscribe(const char *topic)
{
    return this->clientMqtt.subscribe(topic);
}

boolean MQTT::loop()
{
    return this->clientMqtt.loop();
}
