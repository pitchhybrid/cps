#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int modo = 0;

int segundos = 0;

byte smile[8] = {
    B00000,
    B10001,
    B00000,
    B00000,
    B10001,
    B01110,
    B00000,
    B00000};

void setup()
{
    Serial.begin(9600);

    lcd.init();
    lcd.backlight();

    lcd.createChar(0, smile);

    if (modo == 0)
    {
        lcd.setCursor(5, 0);
        lcd.print("CPS");
        // lcd.cursor();
        // lcd.blink();
    }

    if (modo == 2)
    {
        lcd.home();
        lcd.write(224);
        lcd.write(B11111100);
        lcd.write(126);
        lcd.write(B00100110);
    }

    if (modo == 4)
    {
        lcd.home();
        lcd.print("Ciber Fisico");
    }

    if (modo == 5)
    {
        lcd.rightToLeft();
    }

    if (modo == 6)
    {
        lcd.home();
        lcd.write(0);
    }
}

void loop()
{

    if (modo == 1)
    {

        lcd.setCursor(2, 0); // Coluna, Linha
        lcd.print("Tempo: ");

        lcd.setCursor(9, 0);
        lcd.print(segundos++);
        delay(1000);
    }

    if (modo == 3)
    {
        lcd.setCursor(5, 0);
        lcd.print("Display");
        lcd.display();
        delay(1000);
        lcd.noDisplay();
        delay(1000);
    }

    if (modo == 4)
    {
        lcd.scrollDisplayLeft();
        // lcd.scrollDisplayRight();
        delay(400);
    }

    if (modo == 5)
    {
        String texto = "ABC";

        lcd.setCursor(15, 1);

        for (int i = 0; i < texto.length(); i++)
        {
            lcd.write(texto.charAt(i));
        }
    }
}