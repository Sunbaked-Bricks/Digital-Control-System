// this example is public domain. enjoy! https://learn.adafruit.com/thermocouple/

// THERMOCOUPLE INIT //
#include "max6675.h"

int thermoCLK = D1;
int thermoCS = D3;
int thermoSO = D4;

int cur_temp = 0;
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
  // THERMOCOUPLE EXAMPLE //
  // basic readout test, just print the current temp
  cur_temp = thermocouple.readFahrenheit();
  Serial.print("C = "); 
  Serial.print(thermocouple.readCelsius());
  Serial.print(" | F = ");
  Serial.println(cur_temp);
 
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
  // THERMOCOUPLE EXAMPLE //
}
