#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <component.h>

#pragma once
#define LEFT 1
#define RIGHT 0
class Display : Component
{

private:
    u_int cols;
    uint rows;
    LiquidCrystal_I2C *lcd;

public:
    Display(int _pin, u_int cols, u_int rows) : Component(_pin)
    {
        this->cols = cols;
        this->rows = rows;
        this->lcd = new LiquidCrystal_I2C(_pin, cols, rows);
    }

    void iniciar();
    void setCursor(u_int c, u_int l);
    void print(const char *txt);
    void home();
    void display();
    void noDisplay();
    void scroll(u_int d);
};

#endif
