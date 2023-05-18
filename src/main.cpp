#include <main.h>


const char *topic = "";

std::set<std::string> cartoes;

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
  String id = rfid.ler_id(); /*ID do cartão*/

  if (cartoes.find((std::string) id.c_str()) != cartoes.end())
  {
    trava.liberar();
    buzzer._tone(1000, 1000);
    mqtt.publish("" /* TOPICO */ , id.c_str());
    display.home();
    display.print("TRAVA LIBERADA");
    Serial.println("TRAVA LIBERADA");
  }
  else
  {
    trava.travar();
    buzzer._tone(1000, 500);
    buzzer._tone(1000, 500);

    display.home();
    display.print("NEGADO");
    Serial.println("TRAVA NEGADA");
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Uma mensagem chegou no tópico: ");
  Serial.println(topic);

  Serial.print("Payload: ");
  Serial.println((char *)payload);
  std::vector<std::string> ids = split(((std::string)((char *)payload)), "|");
  for(std::string str: ids){
    cartoes.insert(str);
  }
}

std::vector<std::string> split(std::string s, std::string delimiter)
{
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
  {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}