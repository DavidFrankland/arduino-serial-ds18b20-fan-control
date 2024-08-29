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
//                      ┌──── 12V
//                      │
//                     ┌┴┐
//                     │ │fan
//                     └┬┘
//                      │
//          ____  npn ┃ ╱ c
// Fan  ───┤____├─────┨🮤
//           1k     b ┃ ╲ e
//                      │
// GND  ────────────────┴──── GND

const uint8_t DS18S20_PIN = 8;           // temperature sensor GPIO (Temp on diagram)
const uint8_t FAN_PIN = LED_BUILTIN;     // fan control GPIO (Fan on diagram)
const float THRESHOLD = 30.0;            // temperature above which the fan should be on
const float HYSTERESIS = 0.5;            // fan switches at THRESHOLD ± HYSTERESIS
const uint8_t RS_PIN = 7;                // LCD RS
const uint8_t ENABLE_PIN = 6;            // LCD enable
const uint8_t D4_PIN = 5;                // LCD D4
const uint8_t D5_PIN = 4;                // LCD D5
const uint8_t D6_PIN = 3;                // LCD D6
const uint8_t D7_PIN = 2;                // LCD D7
const uint16_t DISPLAY_SHOW_TIME = 2000; // time that line 2 on the LCD is shown

// global variables
uint8_t line2delay = 2;
uint8_t line2count = 0;
byte line2mode = 0;
bool isFanOn = false;

// create the temperature sensor(s)
OneWire oneWire(DS18S20_PIN);
DallasTemperature sensors(&oneWire);

// create the LCD display
LiquidCrystal lcd(RS_PIN, ENABLE_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// function prototypes
void lcdPrintFloat(LiquidCrystal lcd, float f);

void setup()
{
  digitalWrite(FAN_PIN, LOW);
  pinMode(FAN_PIN, OUTPUT);

  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.noCursor();
  lcd.clear();

  sensors.begin();
}

void loop()
{
  lcd.home();

  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  if (temperature == DEVICE_DISCONNECTED_C)
  {
    lcd.clear();
    lcd.print(" Sensors error! ");
    lcd.setCursor(0, 1);
    lcd.print("  Check sensor  ");
    delay(100);
    return;
  }

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

  lcd.setCursor(0, 1);
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
  line2count = (line2count + 1) % line2delay;
  if (line2count == 0)
  {
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
