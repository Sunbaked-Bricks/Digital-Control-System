// WIFI WEBSERVER INIT //
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <string>
#include <ArduinoJson.h>
using namespace std;

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
// WIFI WEBSERVER INIT //

// THERMOCOUPLE INIT //
#include "max6675.h"

int thermoCLK = D0;
int thermoCS = D3;
int thermoSO = D4;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);
// THERMOCOUPLE INIT //

// RELAY INIT //
// We are reusing IN4 as the control for the relay
int relay_status = 0;
// RELAY INIT //

// STEPPER MOTOR INIT //
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
int step_pos = 0;
int Direction = CW; //true -> clockwise, false -> counter-clockwise
// STEPPER MOTOR INIT //

// OLED INIT //
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// OLED INIT //



void setup() {
  Serial.begin(9600);
  
  // SETUP WEBSERVER //
  // set wifi configuration
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  // web-server html pages
  server.on("/", handle_OnConnect);
  server.on("/returnHome", handle_returnHome);
  server.on("/info", handle_post);
  server.on("/getTemp", handle_temp);
  
  server.on("/relayOn", handle_relayOn);
  server.on("/relayOff", handle_relayOff);
  
  server.on("/stepLeft", handle_stepLeft);
  server.on("/stepRight", handle_stepRight);
  server.on("/stepLeftTen", handle_stepLeftTen);
  server.on("/stepRightTen", handle_stepRightTen);
  server.onNotFound(handle_NotFound);
  
  // start server
  server.begin();
  Serial.println("HTTP server started");
  // SETUP WEBSERVER //  
  
  // THERMOCOUPLE SETUP //
  // wait for MAX chip to stabilize
  delay(500);
  // THERMOCOUPLE SETUP //

  // STEPPER MOTOR SETUP //
  Serial.begin(9600);
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  // STEPPER MOTOR SETUP //
  
  // RELAY SETUP //
  digitalWrite(IN4, LOW);
  // RELAY SETUP //
  
  // OLED SETUP //
  // Display Setup
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.display();      // Show initial text
  delay(2000);
  // OLED SETUP //
}



void loop() {
  // WEBSERVER LOOP //
  server.handleClient();
  // WEBSERVER LOOP //

  // THERMOCOUPLE LOOP //
  // basic readout test, just print the current temp
  cur_temp = thermocouple.readFahrenheit();  
  Serial.print("C = "); Serial.print(thermocouple.readCelsius()); Serial.print(" | F = "); Serial.println(thermocouple.readFahrenheit());
  
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
  // THERMOCOUPLE LOOP //
  
  // RELAY LOOP //
  // the stepper motor is controlled from the webserver commands
  // RELAY LOOP //
  
  // STEPPER MOTOR LOOP //
  // the stepper motor is controlled from the webserver commands
  // STEPPER MOTOR LOOP //
  
  // OLED LOOP //  
  update_screen();
  // OLED LOOP //
}



// WIFI WEBSERVER FUNCTIONS //
// this function is called whenever the app requests the temperature readings from the thermocouple
void handle_returnHome() {
  Serial.println("home page requested");
  server.send(200, "text/html", SendHomeHTML());
}

void handle_temp() {
  Serial.println("temperature requested");
  server.send(200, "text/plain", String(cur_temp));
}

void handle_relayOn(){
  close_relay();
  server.send(200, "text/html", SendHomeHTML());
}

void handle_relayOff(){
  open_relay();
  server.send(200, "text/html", SendHomeHTML());
}

void handle_stepLeft() {
  Serial.println("step left requested");
  server.send(200, "text/html", SendHomeHTML());
  step_once(CCW);
}

void handle_stepRight() {
  Serial.println("step left requested");
  server.send(200, "text/html", SendHomeHTML());
  step_once(CW);
}

void handle_stepRightTen() {
  Serial.println("step right requested");
  server.send(200, "text/html", SendHomeHTML());
  step_x(CW, 10);
}

void handle_stepLeftTen() {
  Serial.println("step left requested");
  server.send(200, "text/html", SendHomeHTML());
  step_x(CCW, 10);
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
  ptr +="<p>Absolute Step Position: " + String(step_pos) + "</p>"; // this should probably be able to get reset

  ptr +="<p>Home</p><a class=\"button button-on\" href=\"/returnHome\">Home</a>\n";

  if (relay_status) {
    // if relay status == 1, the relay button should be used to turn the relay off
    ptr +="<p>Oven Control</p><a class=\"button button-on\" href=\"/relayOff\">On</a>\n";
  } else {
    // if relay status == 0, the relay button should be used to turn the relay on
    ptr +="<p>Oven Control</p><a class=\"button button-off\" href=\"/relayOn\">Off</a>\n";    
  }

  /*
  ptr +="<p>Step Left</p><a class=\"button button-on\" href=\"/stepLeft\">CCW 1</a>\n";
  ptr +="<p>Step Right</p><a class=\"button button-on\" href=\"/stepRight\">CW 1</a>\n";
  ptr +="<p>Step Left x10</p><a class=\"button button-on\" href=\"/stepLeftTen\">CCW 10</a>\n";
  ptr +="<p>Step Right x10</p><a class=\"button button-on\" href=\"/stepRightTen\">CW 10</a>\n";
  */

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
// WIFI WEBSERVER FUNCTIONS //

// RELAY FUNCTIONS //
void close_relay(){
  relay_status = 1;
  digitalWrite(IN4, HIGH);
}

void open_relay(){
  relay_status = 0;
  digitalWrite(IN4, LOW);
}
// RELAY FUNCTIONS //

// STEPPER MOTOR FUNCTIONS //
void step_x(int Direction, int x) {
  for (int i = 0; i < x; i++) {
    step_once(Direction);
  }
}

void step_once(int Direction) {
  // handle direction of step
  if (Direction == 1) {
    pos += 1;
    step_pos += 1;
    if (pos > 7) pos = 0;
  } else if (Direction == 0) {
    pos -= 1;
    step_pos -= 1;
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
  
  // control turn rate
  delay(1000);
}
// STEPPER MOTOR FUNCTIONS //

// OLED FUNCTIONS //
void update_screen() {
  // setup 
  display.clearDisplay();

  //Display the Temperature at the probe
  display.setTextSize(2); // Draw 2X-scale text
  display.setCursor(0, 32);
  display.print("TEMP (F): ");
  display.print(cur_temp);
  
  display.display();
}
// OLED FUNCTIONS //
