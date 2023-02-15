#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_MAX31855.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//For scanning of availible wifi networks
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <ESP8266HTTPClient.h>


/* From local working version
#ifndef STASSID
#define STASSID "Poppy's Slow Friend"
#define STAPSK  "2SlowFurYou"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "script.google.com";
*/

//Needed for the GET
const char* host = "script.google.com";

//For the OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };


/* Pin 13 is to turn on the fan if 
 * active cooling is required
 */
int val;
int elapsedTime;
int linecount = 0;
int secondsToWait = 10;
bool displayFarh = true;
// Using LM35dz Temperature Sensors
//int probeTempPin = 0;
//int controlTempPin = 1;

// Thermocouple Setup
#define MAXDO   12
#define MAXCS   13
#define MAXCLK  14
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

bool usingThermocouple = true;
int fanPin = 0;

void setup(){
  Serial.begin(115200);
  //Serial.println(WiFi.localIP());

  //Display Setup
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
  // Probe setup
  //Serial.begin(9600);
  //pinMode(fanPin,OUTPUT);
  //Serial.println("MAX31855 test");
  //display.println("MAX31855 test");
  //linecount += 1;
  display.display();      // Show initial text
  // wait for MAX chip to stabilize
  delay(500);
  //Serial.println("Initializing sensor...");
  //display.println("Initializing sensor...");
  //linecount += 2;

  display.display();      // Show initial text
  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }
  //display.println("Setup complete!");
  //linecount += 1;
  //display.display();      // Show initial text
  //for(;;);
  display.cp437(true);         // Use full 256 char 'Code Page 437' font


}

void loop(){

   while(1){
    double controlCel,controlFarh,probeCel,probeFarh;
    if (usingThermocouple){
      controlCel = thermocouple.readInternal();
      controlFarh = (controlCel*9)/5 +32;
      probeCel = thermocouple.readCelsius();
      probeFarh = thermocouple.readFahrenheit();
    
      //Alert the user of issues with the thermocouple
      if (isnan(probeCel) || isnan(probeFarh)) {
        display.clearDisplay();
        display.setCursor(0, 0);
        //Serial.println("Something wrong with thermocouple!");
        display.println("Something wrong with thermocouple!");
        display.display();      // Show initial text
        delay(2000); // Pause for 2 seconds
        continue;
      }
    }
    //Code for using the LM35DZ
    else {
      //int controlVal = analogRead(controlTempPin);
      //int probeVal = analogRead(probeTempPin);
      //double controlMv = ( controlVal / 1024.0) * 5000;
      //controlCel = controlMv / 10;
      //controlFarh = (controlCel*9)/5 +32;
    
      //double probeMv = ( probeVal / 1024.0) * 5000;
      //probeCel = probeMv / 10;
      //probeFarh = (probeCel*9)/5 +32;
    }
    display.clearDisplay();
    //Display the Temperature at the board
    display.setTextSize(1); // Draw 2X-scale text
    display.setCursor(0, 16);
    display.print("EXTERNAL");
    display.setCursor(0, 24);
    display.println("TEMP");
    display.setCursor(64, 16);
    display.setTextSize(2); // Draw 2X-scale text
    
    display.print(int(controlFarh));
    display.write(248);
    display.print("F");
    
    
    //Display the Temperature at the probe
    display.setTextSize(1); // Draw 2X-scale text
    display.setCursor(0, 32);
    display.print("PROBE");
    display.setCursor(0, 40);
    display.println("TEMP");
    display.setCursor(64, 32);
    display.setTextSize(2); // Draw 2X-scale text
    
    display.print(int(probeFarh));
    display.write(248);
    display.print("F");
    
    display.display();
    //set delay on how often to check temp
    delay(1000); // Pause for 10 seconds
    
  } 

  

}
