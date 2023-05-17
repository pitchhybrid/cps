#include <Arduino.h>
#include "component.h"

#pragma once
class Trava: public Component{
    public:
        Trava(int _pin) : Component(_pin){};
        void iniciar();
        bool travar();
        bool liberar();
};