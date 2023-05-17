#include "led.h"

void Led::iniciar(){
    Component::iniciar(OUTPUT,false);
    estado = digitalRead(pin);
};

void Led::acender(){
    digitalWrite(pin,HIGH);
}

void Led::acender(int ms){
    acender();
    delay(ms);
    apagar();
}

void Led::apagar(){
    digitalWrite(pin,LOW);
}

void Led::piscar(int v){
    for(int i= 0; i < v;i++){
        acender(1000);
    }
}