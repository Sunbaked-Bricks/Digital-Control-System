// this example is public domain. enjoy! https://learn.adafruit.com/thermocouple/

// THERMOCOUPLE INIT //
#include "max6675.h"

int thermoCLK = D1;
int thermoCS = D3;
int thermoSO = D4;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);
// THERMOCOUPLE INIT //

void setup() {
  Serial.begin(9600);
  // THERMOCOUPLE SETUP //
  // wait for MAX chip to stabilize
  delay(500);
  // THERMOCOUPLE SETUP //
}

void loop() {
  // THERMOCOUPLE LOOP //
  // basic readout test, just print the current temp
  Serial.print(thermocouple.readFahrenheit());
  
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
  // THERMOCOUPLE LOOP //
}
