// this example is public domain. enjoy! https://learn.adafruit.com/thermocouple/

#include "max6675.h"

int thermoSO = D4;
int thermoCS = D5;
int thermoCLK = D6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);

void setup() {
  Serial.begin(9600);

  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
  
  Serial.print("C = "); 
  Serial.print(thermocouple.readCelsius());
  Serial.print(" | F = ");
  Serial.println(thermocouple.readFahrenheit());
 
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
}
