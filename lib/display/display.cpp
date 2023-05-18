#include "display.h"

void Display::iniciar(){
    lcd->init();
    lcd->backlight();
    lcd->home();
};

void Display::setCursor(u_int c, u_int l){
    lcd->setCursor(c,l);
};

void Display::print(char * txt){
    lcd->print(txt);
};

void Display::home(){
    lcd->home();
};

void Display::display(){
    lcd->display();
};

void Display::noDisplay(){
    lcd->noDisplay();
};

void Display::scroll(u_int d){
    if(d == LEFT){
        lcd->scrollDisplayLeft();
    }
    if(d == RIGHT){
        lcd->scrollDisplayRight();
    }
}