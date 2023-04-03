#define IN1  D5
#define IN2  D6
#define IN3  D7
#define IN4  D8
#define CW   1
#define CCW  0

// motor variables
int Steps = 0;
int steps_left=50;
int pos = -1;
int Direction = CW; //true -> clockwise, false -> counter-clockwise

// time variables
unsigned long last_time;
unsigned long currentMillis;
long tp;

void setup()
{
  Serial.begin(115200);
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
}

void loop(){
  step_once(CW);
  delay(1000);
}

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
