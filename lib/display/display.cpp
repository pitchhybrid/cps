#include "display.h"

void Display::iniciar()
{
    lcd->init();
    lcd->backlight();
    lcd->home();
    lcd->print("Aproxime o cartão");
};

void Display::setCursor(u_int c, u_int l)
{
    lcd->setCursor(c, l);
};

void Display::print(const char *txt)
{
    lcd->clear();
    lcd->print(txt);
};

void Display::home()
{
    lcd->home();
};

void Display::display()
{
    lcd->display();
};

void Display::noDisplay()
{
    lcd->noDisplay();
};

void Display::clear() {
    lcd->clear();
}

void Display::testDisplay(){
    lcd->noBacklight();
    delay(1000);
    lcd->backlight();
    lcd->print("TESTANDO LCD");
    delay(1000);
    lcd->clear();
}

void Display::scroll(u_int d)
{
    if (d)
    {
        lcd->scrollDisplayLeft();
    }
    else
    {
        lcd->scrollDisplayRight();
    }
}