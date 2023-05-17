#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include "component.h"
#include <WiFi.h>
#include <PubSubClient.h>

#pragma once

class MQTT: public Component {
    private:
        PubSubClient clientMqtt;

    public:
        void iniciar(){};
        void iniciar(char *ssid, char *pass);
        void iniciar_broker(char * server, u_int port, char *user, char* pass);
        void set_callback(MQTT_CALLBACK_SIGNATURE);
        void publish(char * topic, char* payload);
        void subscribe(char * topic);
        void loop();
};

#endif