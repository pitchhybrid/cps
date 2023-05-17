#include <Arduino.h>

/* CONFIGURAÇÕES DO WIFI */
const char *ssid = "nome_da_rede";      // Nome da rede WiFi
const char *password = "senha_secreta"; // Senha da rede WiFi

/* CONFIGURAÇÕES DO MQTT*/
const char *mqttServer = "broker.hivemq.com"; // Endereço do Broker MQTT
const int mqttPort = 1883;                    // Porta TCP do Broker MQTT
const char *mqttUser = "";                    // Usuário MQTT
const char *mqttPassword = "";                // Senha MQTT
