#include "display.h"

void Display::iniciar(){
    lcd->init();
    lcd->backlight();
    lcd->home();
};