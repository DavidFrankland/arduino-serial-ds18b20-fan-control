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
