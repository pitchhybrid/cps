#include "display.h"

void Display::iniciar()
{
    lcd->init();
    lcd->backlight();
    lcd->home();
};

void Display::setCursor(u_int c, u_int l)
{
    lcd->setCursor(c, l);
};

void Display::print(const char *txt)
{
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