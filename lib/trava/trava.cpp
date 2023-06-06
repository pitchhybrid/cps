#include <Arduino.h>
#include "trava.h"

void Trava::iniciar()
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
};

bool Trava::liberar()
{
    digitalWrite(pin, LOW);
    estado = digitalRead(pin);
    return estado;
};

bool Trava::travar()
{
    digitalWrite(pin, HIGH);
    estado = digitalRead(pin);
    return estado;
};