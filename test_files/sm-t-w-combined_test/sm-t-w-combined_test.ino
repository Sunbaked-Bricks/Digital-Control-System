// Libraries
#include "max6675.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <string>
#include <ArduinoJson.h>
#include <EEPROM.h>
using namespace std;


// WIFI WEBSERVER //
// ssid and password
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "";  //Enter Password here

// IP Address Details
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// initialize webserver
ESP8266WebServer server(80);

// temperature (Farenheit)
int cur_temp = 0;
int tar_temp = 0;

// STEPPER MOTOR //
// motor pins
#define IN1  D5
#define IN2  D6
#define IN3  D7
#define IN4  D8

// motor variables
#define CW   1
#define CCW  0
int pos = -1;
boolean Direction = true; //true -> clockwise, false -> counter-clockwise
int min_pos = 0;
int max_pos = 60;


// THERMOCOUPLE //
// thermocouple pins
#define thermoSO D4
#define thermoCS D3
#define thermoCLK D2

// initialize thermocouple
MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO); // ***


void setup() {
  Serial.begin(9600);

  // TEST EEPROM //
  delay(3000);
  Serial.print("EEPROM[0]: "); Serial.println(EEPROM.read(0)); // check what was written to EEPROM[0] -> for first time, should print 255
  EEPROM.write(0, 98);
  

  // SETUP MOTOR //
  // set motor pins ***
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);


  // SETUP WEBSERVER //
  // set wifi configuration
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  // web-server html pages
  server.on("/", handle_OnConnect);
  server.on("/info", handle_post);
  server.on("/getTemp", handle_temp);
  server.onNotFound(handle_NotFound);
  
  // start server
  server.begin();
  Serial.println("HTTP server started");
  // SETUP WEBSERVER //


  // SETUP THERMOCOUPLE //
  // wait for MAX chip to stabilize
  delay(500);
  
  Serial.println("stepper motor/thermocouple/webserver test");
}

//--

void loop() {
  // test stepper motor
  step_once(CCW);

  // test thermocouple
  cur_temp = thermocouple.readFarenheit();
  Serial.print("C = "); Serial.print(thermocouple.readCelsius()); Serial.print(" | F = "); Serial.println(thermocouple.readFahrenheit());

  // WEBSERVER LOOP //
  server.handleClient();
  // WEBSERVER LOOP //
  
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
}

//--

///* STEPPER MOTOR FUNCTIONS
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
// STEPPER MOTOR FUNCTIONS */

//--

// WIFI WEBSERVER FUNCTIONS //
// this function is called whenever the app requests the temperature readings from the thermocouple
void handle_temp() {
  Serial.println("temperature requested");
  server.send(200, "text/plain", String(cur_temp));
}

void handle_post(){
  String input = server.arg("plain");
  DynamicJsonDocument doc(1024);
  
  DeserializationError error = deserializeJson(doc, input);
  
  tar_temp = doc["temp"];
  Serial.print("Input from app: "); Serial.print(input);
  Serial.print(" | Target temp set to: "); Serial.println(tar_temp);
  server.send(200, "text/html", SendHomeHTML()); 
}

void handle_OnConnect() {
  Serial.println("client connected");
  server.send(200, "text/html", SendHomeHTML());
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHomeHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";

  ptr +="<p>Target Temperature: " + String(tar_temp) + "</p>";
  ptr +="<p>Current Temperature: " + String(cur_temp) + "</p>";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
// WIFI WEBSERVER FUNCTIONS //
