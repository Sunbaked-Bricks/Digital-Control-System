# Thermocouple-Arduino
> for use with the sunbacked bricks solar oven project.

## Set Up
- [Install the Arduino IDE](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE)
- Then pull this repository and open it in the IDE.

* Add the following to Additional Board Manager's URL in the Arduino IDE (v1.8.18):
  * http://arduino.esp8266.com/stable/package_esp8266com_index.json
  * https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
* Connect to Thermocouple Arduino through wifi using a phone 
  - Once the Node MCU is powered on, it will immediately instance the wifi server for connection, this will be named "Node MCU" in the wifi manager.

## Aruduino configuration

Board Used:
> NodeMCU 1.0 (ESP-12E Module)

Libraries needed:
* WiFiManager (by tzapu)
* Adafruit_SSD1306
* ESP8266Wifi
* ESP8266WebServer
* ArduinoJson
* string

## Use Instructions

If the dial is adjusted manually at any time, the Node MCU will not be aware of this change. Always ensure the dial is in the OFF position before powering on the oven.

