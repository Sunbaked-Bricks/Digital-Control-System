#include "Adafruit_MAX31855.h"

// Thermocouple Setup
#define MAXDO   4
#define MAXCS   5
#define MAXCLK  6
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int probeCel = thermocouple.readCelsius();
  int probeFarh = thermocouple.readFahrenheit();
  Serial.print("Celsius: "); Serial.print(probeCel); Serial.print(" | Farenheit: "); Serial.println(probeFarh);
}
