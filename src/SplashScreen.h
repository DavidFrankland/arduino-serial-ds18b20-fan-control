#include <LiquidCrystal.h>

class SplashScreen
{
public:
    /// @brief display a nice splash screen to the LCD
    /// @param lcd LCD display to use
    static void show(LiquidCrystal lcd);

private:
    static void writeLines(LiquidCrystal lcd, char *line1text, char *line2text);
};

void SplashScreen::show(LiquidCrystal lcd)
{
    uint8_t blockChar[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    lcd.createChar(0, blockChar);
    lcd.clear();
    lcd.noCursor();

    char line1text[] = "     Otley      ";
    char line2text[] = "  MAKER SPACE   ";
    char spaces[] = "                ";

    writeLines(lcd, line1text, line2text);
    delay(1000);
    writeLines(lcd, spaces, spaces);
    lcd.clear();
}

void SplashScreen::writeLines(LiquidCrystal lcd, char *line1text, char *line2text)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        lcd.setCursor(i, 0);
        lcd.print((char)0);
        lcd.setCursor(i, 1);
        lcd.print((char)0);
        delay(80);
        lcd.setCursor(i, 0);
        lcd.print(line1text[i]);
        lcd.setCursor(i, 1);
        lcd.print(line2text[i]);
    }
}
