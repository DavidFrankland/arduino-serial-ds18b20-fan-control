#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal.h>

//      Arduino       DS18B20
// +5V  ──────────┬────────── VDD
//                │
//               ┌┴┐
//               │ │4k7
//               └┬┘
//                │
// Temp ──────────┴────────── DQ
// GND  ───────────────────── GND
//
//                ┌────────── 12V
//                │
//               ┌┴┐
//               │ │fan
//               └┬┘
//                │
//          npn ┃ ╱ c
// Fan  ────────┨🮤
//            b ┃ ╲ e
//                │
// GND  ──────────┴────────── GND

const uint8_t DS18S20_PIN = A5;          // temperature sensor GPIO (Temp on diagram)
const uint8_t FAN_PIN = LED_BUILTIN;     // fan control GPIO (Fan on diagram)
const float THRESHOLD = 50.0;            // temperature above which the fan should be on
const float HYSTERESIS = 0.1;            // fan switches at THRESHOLD ± HYSTERESIS
const uint8_t RS_PIN = 1;                // LCD RS
const uint8_t ENABLE_PIN = 1;            // LCD enable
const uint8_t D0_PIN = 1;                // LCD D0
const uint8_t D1_PIN = 1;                // LCD D1
const uint8_t D2_PIN = 1;                // LCD D2
const uint8_t D3_PIN = 1;                // LCD D3
const uint16_t DISPLAY_SHOW_TIME = 2000; // time that line 2 on the LCD is shown

// global variables
unsigned long line2UpdateTime = 0;
byte line2mode = 0;
bool isFanOn = false;

// create the temperature sensor(s)
OneWire oneWire(DS18S20_PIN);
DallasTemperature sensors(&oneWire);

// create the LCD display
LiquidCrystal lcd(RS_PIN, ENABLE_PIN, D0_PIN, D1_PIN, D2_PIN, D3_PIN);

// function prototypes
void lcdPrintFloat(LiquidCrystal lcd, float f);

void setup()
{
  Serial.begin(9600);
  pinMode(FAN_PIN, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  sensors.begin();
}

void loop()
{
  lcd.home();

  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  if (temperature != DEVICE_DISCONNECTED_C)
  {
    lcd.print("Temperature ");
    if (temperature > 0)
    {
      Serial.println(temperature, 4);
      lcdPrintFloat(lcd, temperature);
      if (temperature > THRESHOLD + HYSTERESIS)
      {
        isFanOn = true;
        digitalWrite(FAN_PIN, HIGH);
      }
      if (temperature < THRESHOLD - HYSTERESIS)
      {
        isFanOn = false;
        digitalWrite(FAN_PIN, LOW);
      }
    }
    else
    {
      lcd.print("----");
    }
  }
  else
  {
    lcd.print("Sensors error!!!");
  }

  lcd.setCursor(0, 1);
  if (millis() > line2UpdateTime)
  {
    switch (line2mode)
    {
    case 0:
      lcd.print("Threshold   ");
      lcdPrintFloat(lcd, THRESHOLD);
      break;
    case 1:
      lcd.print("Hysteresis  ");
      lcdPrintFloat(lcd, HYSTERESIS);
      break;
    case 2:
      lcd.print("Fan          ");
      lcd.print(isFanOn ? " ON" : "OFF");
      break;
    }
    line2UpdateTime += DISPLAY_SHOW_TIME;
    line2mode = (line2mode + 1) % 3;
  }
}

// prints values in one of the forms:
// xx.x
//  x.x
void lcdPrintFloat(LiquidCrystal lcd, float f)
{
  if (f < 10)
  {
    lcd.print(' ');
  }
  lcd.print(f, 1);
}
