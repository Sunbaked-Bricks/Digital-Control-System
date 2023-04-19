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


void setup() {
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
}

void loop() {
  // WEBSERVER LOOP //
  server.handleClient();
  // WEBSERVER LOOP //
}

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
