#include <Arduino.h>
#include <mqtt.h>
#include <trava.h>
#include <display.h>
#include <rfid.h>
#include <tone.h>
/* CONFIGURAÇÕES DO WIFI */
const char *ssid = "nome_da_rede";      // Nome da rede WiFi
const char *password = "senha_secreta"; // Senha da rede WiFi

/* CONFIGURAÇÕES DO MQTT*/
const char *mqttServer = "broker.hivemq.com"; // Endereço do Broker MQTT
const int mqttPort = 1883;                    // Porta TCP do Broker MQTT
const char *mqttUser = "";                    // Usuário MQTT
const char *mqttPassword = "";                // Senha MQTT

// LCD
#define LCD 0x27 // LED ADDRESS
#define COLS 16  // COLUNAS
#define ROWS 2   // LINHAS

// RFID
#define SS 0  // PINO SS
#define RST 0 // PINO RST

// TRAVA
#define PINO_TRAVA 0

// BUZZER
#define PINO_BUZZER 0

void callback(char *topic, byte *payload, unsigned int length);