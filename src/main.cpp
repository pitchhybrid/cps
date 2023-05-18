#include <main.h>

const char *topic = "";

MQTT mqtt;
Trava trava(PINO_TRAVA);
Tone buzzer(PINO_BUZZER);
Display display(LCD, ROWS, COLS);
RFID rfid(SS, RST);

void setup()
{
  // INICIAR OS COMPONENTES
  mqtt.iniciar(ssid, password);
  mqtt.iniciar_broker(mqttServer, mqttPort, mqttUser, mqttPassword);
  trava.iniciar();
  display.iniciar();
  rfid.iniciar();

  // SUBSCRIBE TOPIC
  mqtt.set_callback(callback);
  mqtt.subscribe(topic);
}

void loop()
{
  
  mqtt.loop();

  rfid.aguardar_cartao();
  String id = rfid.ler_id();
}

void callback(char *topic, byte *payload, unsigned int length){

}