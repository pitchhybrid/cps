#include "Wire.h"
#include "I2CKeyPad.h"
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

I2CKeyPad teclado(0x20);

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

      clientMqtt.subscribe("esp32/input");
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

void configureKeypad() {
  Wire.begin();
  Wire.setClock(400000);
  if (!teclado.begin()) {
    Serial.println("Falha na comunicação do Teclado. Reinicie o dispositivo.");
    while(1);
  }
}

void setup()
{
  Serial.begin(115200);

  rfid.iniciar();
  trava.iniciar();
  display.iniciar();

  connectToWifi();
  configureKeypad();
}

void scanAddress() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000); 
}

String listenerKeys(){
    String word = "";
    display.clear();

    int ultima_vez_apertado = 0;

    while(1){
        int tempo_atual = millis();
        char teclas[] = "123A456B789C*0#DNF";  // N = NoKey, F = Fail
        int index = teclado.getKey();
        char key = teclas[index];

        if(key == 'N') continue;

        if(tempo_atual - ultima_vez_apertado <= 200) {
          Serial.println("Aguarde");
          continue;
        }

        ultima_vez_apertado = millis();

        Serial.print("tecla:");
        Serial.println(key);
        Serial.println(word);

        if(key){
            if(key == '#'){
              break;
            }
            word += key;
        }
    }

    return word;
}


void loop()
{
  trava.travar();
  clientMqtt.loop();

  if(teclado.isPressed()) {
    Serial.println("Tecla pressionada");
    String password = listenerKeys();

    if(password == "1111"){
      display.print("ACESSO LIBERADO");
      trava.liberar();

      buzzer._tone(1000, 500);
      delay(2000);
    }else {
      display.print("ACESSO NEGADO");

      buzzer._tone(1000, 400);
      delay(500);
      buzzer._tone(1000, 400);
      delay(1000);
    }
  }

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