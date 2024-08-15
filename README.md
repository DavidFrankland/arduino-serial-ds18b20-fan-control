# Arduino serial DS18B20 temperature sensor

Arduino code for USB serial temperature sensor with optional fan control and LCD display.
The temperature sensor used is a DS18B20.

## Serial port

The Arduino connects to a device such as a PC or Raspberry Pi via USB, emulating a serial port.
The Arduino outputs the temperature in °C on the serial port.
The value is decimal, with 4 decimal places.

## LCD display

The temperature, along with operating parameters, can also be displayed on a two-line LCD display. 

## Fan control

In addition, if the temperature is too high, a fan can be controlled.

## Configurable parameters

```C++
const float THRESHOLD = 50.0; // temperature above which the fan should be on
const float HYSTERESIS = 0.1; // fan switches at THRESHOLD ± HYSTERESIS
```
`THRESHOLD` is the maximum temperature we wish to maintain. 
When the temperature rises too high, a fan is switched on.
A small `HYSTERESIS` value is used to prevent the fan rapidly switching when the temperature is hovering around the `THRESHOLD` value.
When the temperature goes over `THRESHOLD + HYSTERESIS`, the fan is switched on, and it remains on until the temperature drops below `THRESHOLD - HYSTERESIS`.

```C++
const uint16_t DISPLAY_SHOW_TIME = 2000; // time that line 2 on the LCD is shown
```
When an LCD display is used, the information on line 2 of the display show the `THRESHOLD` and `HYSTERESIS` values, and whether the fan is on or off.
The 3 lines of text are displayed in turn, changing every `DISPLAY_SHOW_TIME` milliseconds.
