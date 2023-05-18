#include <Arduino.h>
#include "component.h"

void Component::iniciar(int mode, bool analogic)
{
    if (!analogic)
    {
        pinMode(pin, mode);
    }
};