#include "max6675.h"
#define CW   1
#define CCW  0

// STEPPER MOTOR //
// motor pins
#define IN1  D0
#define IN2  D1
#define IN3  D2
#define IN4  D3

// motor variables
int pos = -1;
boolean Direction = true; //true -> clockwise, false -> counter-clockwise

// THERMOCOUPLE //
// thermocouple pins
#define thermoSO D4
#define thermoCS D5
#define thermoCLK D6

// initialize thermocouple
MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO); // ***

void setup() {
  Serial.begin(9600);

  // set motor pins ***
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);
  
  Serial.println("stepper motor/thermocouple test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // test thermocouple
  Serial.print("C = "); Serial.print(thermocouple.readCelsius()); Serial.print(" | F = "); Serial.println(thermocouple.readFahrenheit());

  // test stepper motor
  step_once(CCW);
  
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
}

// stepper motor control
void step_once(int Direction) {
  // handle direction of step
  if (Direction == 1) {
    pos += 1;
    if (pos > 7) pos = 0;
  } else if (Direction == 0) {
    pos -= 1;
    if (pos < 0) pos = 7;
  } else {
    Serial.print("ERROR: Invalid Turn Direction ["); Serial.print(Direction); Serial.println("]");
  }

  // step
  switch(pos){
   case 0:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   case 1:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, HIGH);
   break; 
   case 2:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 3:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 4:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 5:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
     case 6:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 7:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   default:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
  }
}
