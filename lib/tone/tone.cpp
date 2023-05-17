#include "tone.h"
#include <Arduino.h>
void Tone::iniciar(){}

void Tone::_tone(int frequency,long duration){
    tone(pin, frequency,duration);
}

void Tone::_tone(int frequency){
    tone(pin,frequency);
}

void Tone::_noTone(){
    noTone(pin);
}