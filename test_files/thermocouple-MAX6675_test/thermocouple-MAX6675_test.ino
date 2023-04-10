// this example is public domain. enjoy! https://learn.adafruit.com/thermocouple/

// THERMOCOUPLE //
#include "max6675.h"

int thermoCLK = D2;
int thermoCS = D3;
int thermoSO = D4;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);
// THERMOCOUPLE //

void setup() {
  // THERMOCOUPLE //
  Serial.begin(9600);

  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
  // THERMOCOUPLE //
}

void loop() {
  // THERMOCOUPLE //
  // basic readout test, just print the current temp
  Serial.print("C = "); 
  Serial.print(thermocouple.readCelsius());
  Serial.print(" | F = ");
  Serial.println(thermocouple.readFahrenheit());
 
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
  // THERMOCOUPLE //
}
