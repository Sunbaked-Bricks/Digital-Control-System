// Libraries
#include "max6675.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <string>
#include <ArduinoJson.h>
using namespace std;

//--

//For the OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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


// THERMOCOUPLE //
// thermocouple pins
#define thermoSO D4
#define thermoCS D3
#define thermoCLK D2

// initialize thermocouple
MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO); // ***

//--

void setup() {
  Serial.begin(9600);

  // SETUP LCD DISPLAY //
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
  // SETUP LCD DISPLAY //

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
  // SETUP THERMOCOUPLE //
  
  Serial.println("stepper motor/thermocouple/webserver test");
}

//--

void loop() {
  // test thermocouple
  cur_temp = thermocouple.readFarenheit();
  Serial.print("C = "); Serial.print(thermocouple.readCelsius()); Serial.print(" | F = "); Serial.println(thermocouple.readFahrenheit());

  // test webserver
  server.handleClient();

  // test lcd screen
  update_screen();
  
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
}

//--

///* LCD SCREEN FUNCTIONS
void update_screen() {
  // setup 
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1); // Draw 2X-scale text

 
  
  display.display();
}
//*/

///* WEBSERVER FUNCTIONS
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
// WEBSERVER FUNCTIONS */
