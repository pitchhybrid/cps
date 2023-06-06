#include <Arduino.h>
#include <trava.h>
#include <display.h>
#include <rfid.h>
#include <tone.h>

#include <set>
#include <vector>

/* CONFIGURAÇÕES DO WIFI */
const char *ssid = "Denise";      // Nome da rede WiFi
const char *password = "88208794"; // Senha da rede WiFi

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
#define SS 5  // PINO SS
#define RST 27 // PINO RST

// TRAVA
#define PINO_TRAVA 16

// BUZZER
#define PINO_BUZZER 17

std::vector<std::string> split(std::string s, std::string delimiter);