# Thermocouple-Arduino
for use with the sunbacked bricks solar oven project.

## Google Data Sheet
https://docs.google.com/spreadsheets/d/1a8y_awyQa8ON-Y7WlzSzk3OCoIJYemhQLntR6cPDIwY/edit?usp=sharing

## Set Up
- [Install the Arduino IDE](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE)
Then pull this repository and open it in the IDE.

* Add http://arduino.esp8266.com/stable/package_esp8266com_index.json to Additional Board Manager's URL in the Arduino IDE (v1.8.18)
* Connect to Thermocouple Arduino through wifi using a phone 
  - Once the Node MCU is powered on, it will immediately instance the wifi server for connection, this will be named "Node MCU" in the wifi manager.

## Aruduino configuration

Board Used:

> Adafruit Feather HUZZAH ESP8266

Libraries needed:
* Adafruit_GFX
* Adafruit_SSD1306
* Adafruit_MAX31855

WiFiManager
* WiFiManager by tzapu

## Use

If the dial is adjusted manually at any time, the Node MCU will not be aware of this change. Allways ensure the dial is in the OFF position before powering on the oven.

