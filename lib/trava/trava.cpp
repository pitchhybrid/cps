#include <Arduino.h>
#include "trava.h"

void Trava::iniciar()
{
    Component::iniciar(INPUT_PULLUP, false);
    estado = digitalRead(pin);
};

bool Trava::liberar()
{
    digitalWrite(pin, HIGH);
    estado = digitalRead(pin);
    return estado;
};

bool Trava::travar()
{
    digitalWrite(pin, LOW);
    estado = digitalRead(pin);
    return estado;
};