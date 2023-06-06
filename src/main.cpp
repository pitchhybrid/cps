#include <main.h>
#include <WiFi.h>
#include <PubSubClient.h>

std::set<std::string> cartoes;
Trava trava(PINO_TRAVA);
Tone buzzer(PINO_BUZZER);
Display display(LCD, ROWS, COLS);
RFID rfid(SS, RST);

WiFiClient espClient;        
PubSubClient clientMqtt(espClient);

void callback(char *topic, byte *payload, unsigned int length);

void configureMqtt() {
  clientMqtt.setServer(mqttServer, mqttPort);

  while (!clientMqtt.connected())
  {
    Serial.println("Conectando ao Broker MQTT...");

    String clientId = "ESP32Client_" + String(random(0xffff), HEX);
    Serial.println("clientId = " + clientId);

    if (clientMqtt.connect(clientId.c_str(), mqttUser, mqttPassword))
    {
      Serial.println("O cliente " + clientId + " foi conectado com sucesso");

      clientMqtt.setCallback(callback);

      bool isSub = clientMqtt.subscribe("esp32/input");
      Serial.print("subscrieb ");
      Serial.println(isSub);
    }
    else
    {
      int clientState = clientMqtt.state();

      Serial.print("Falha ao se conectar. ");
      Serial.println("Estado do cliente: " + (String)clientState);

      delay(2000);
    }
  }  
}

void connectToWifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("Tentando se conectar na rede: ");
    Serial.println(ssid);
  }

  Serial.println("Conectado na Rede WiFi.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configureMqtt();
}

void setup()
{
  Serial.begin(115200);

  rfid.iniciar();
  trava.iniciar();
  display.iniciar();
  connectToWifi();
}

void loop()
{
  trava.travar();
  clientMqtt.loop();
  
  bool isCardPresent = rfid.aguardar_cartao();
  if(!isCardPresent) return;

  String id = rfid.ler_id();
  Serial.println(id);

  bool status = clientMqtt.publish("esp32/output", id.c_str());
  Serial.println(status ? "enviado com sucesso" : "falha ao enviar");
  delay(1000);
}


void callback(char *topic, byte *payload, unsigned int length)
{
  byte text = payload[0];
  char access = (char) text == '1';
  if (access)
  {
    display.print("ACESSO LIBERADO");
    trava.liberar();

    buzzer._tone(1000, 500);
    delay(2000);
  }
  else
  {
    display.print("ACESSO NEGADO");

    buzzer._tone(1000, 400);
    delay(500);
    buzzer._tone(1000, 400);
    delay(1000);
  }

  display.iniciar();
}