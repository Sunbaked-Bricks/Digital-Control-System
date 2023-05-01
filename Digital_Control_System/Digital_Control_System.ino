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
#define IN4  D8
int relay_status = 0; // the relay is intialized to turn the oven off
int max_temp = 300;
// RELAY INIT //

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

  // setup web-server html handles
  server.on("/", handle_OnConnect);
  server.on("/returnHome", handle_returnHome);
  server.on("/info", handle_post);
  server.onNotFound(handle_NotFound);

  server.on("/getTemp", handle_temp);
  server.on("/setMaxTemp", handle_setMaxTemp);
  
  server.on("/relayOn", handle_relayOn);
  server.on("/relayOff", handle_relayOff);
  
  // start server
  server.begin();
  Serial.println("HTTP server started");
  // SETUP WEBSERVER //  
  
  // THERMOCOUPLE SETUP //
  // wait for MAX chip to stabilize
  delay(500);
  // THERMOCOUPLE SETUP //

  // RELAY SETUP //
  pinMode(IN4, OUTPUT); 
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
  // the relay is controlled from the webserver commands
  if (cur_temp > max_temp) {
    force_shutdown(); // not sure how to handle a message from the arduino to the app
  } 
  // RELAY LOOP //
  
  // OLED LOOP //  
  update_screen();
  // OLED LOOP //
}


// DIGITAL CONTROL SYSTEM FUNCTIONS //
void start_oven() {
  // turn on oven
  close_relay();
}

void force_shutdown() {
  // shutdown the oven
  open_relay();

  // prevent the oven from turning on until the internal temperature reaches a safe level
  // maintain basic functions
  while(cur_temp > max_temp) {
    // handle server 
    server.handleClient();

    // read internal temperature
    cur_temp = thermocouple.readFahrenheit();  
    Serial.print("C = "); Serial.print(thermocouple.readCelsius()); Serial.print(" | F = "); Serial.println(thermocouple.readFahrenheit());
    delay(1000); // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!

    // update OLED screen
    update_screen();
  }
  
}
// DIGITAL CONTROL SYSTEM FUNCTIONS //

// RELAY FUNCTIONS //
void close_relay(){ // turn the oven on
  relay_status = 1;
  digitalWrite(IN4, HIGH);
}

void open_relay(){ // turn the oven off
  // prevent the oven from turning on when the temperature is too high
  if (cur_temp < max_temp) {
    relay_status = 0;
    digitalWrite(IN4, LOW);
  }
}
// RELAY FUNCTIONS //

// WIFI WEBSERVER FUNCTIONS //
// this function is called whenever the app requests the temperature readings from the thermocouple
void handle_returnHome() {
  Serial.println("home page requested");
  server.send(200, "text/html", SendHomeHTML());
}

void handle_setMaxTemp() {
  String input = server.arg("plain");
  DynamicJsonDocument doc(1024);
  
  DeserializationError error = deserializeJson(doc, input);
  
  max_temp = doc["temp"];
  Serial.print("max temperature set to: "); Serial.println(max_temp);
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
  ptr +="<p>Max Temperature: " + String(max_temp) + "</p>";

  ptr +="<p>Home</p><a class=\"button button-on\" href=\"/returnHome\">Home</a>\n";

  if (relay_status) {
    // if relay status == 1, the relay button should be used to turn the relay off
    ptr +="<p>Oven Control</p><a class=\"button button-on\" href=\"/relayOff\">On</a>\n";
  } else {
    // if relay status == 0, the relay button should be used to turn the relay on
    ptr +="<p>Oven Control</p><a class=\"button button-off\" href=\"/relayOn\">Off</a>\n";    
  }

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
// WIFI WEBSERVER FUNCTIONS //

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
