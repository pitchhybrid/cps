#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include "component.h"
#include <WiFi.h>
#include <PubSubClient.h>

#pragma once

class MQTT : public Component
{
private:
    PubSubClient clientMqtt;

public:
    void iniciar(){};
    void iniciar(const char *ssid, const char *pass);
    void iniciar_broker(const char *server, const u_int port, const char *user, const char *pass);
    void set_callback(MQTT_CALLBACK_SIGNATURE);
    boolean publish(const char *topic, const char *payload);
    boolean subscribe(const char *topic);
    boolean loop();
};

#endif