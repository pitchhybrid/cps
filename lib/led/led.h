
#include <Arduino.h>

#include "component.h"
#pragma once
class Led: public Component
{
public:
    Led(int _pin) :Component(_pin){}
    void iniciar();
    void acender();
    void acender(int ms);
    void apagar();
    void piscar(int v);
     
};