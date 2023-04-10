// Libraries
#include "max6675.h"
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WebServer.h>
#include <string>
using namespace std;

//--

// OLED SETUP //
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  
  Serial.println("oled screen test");
}

//--

void loop() {
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

//?
  //Display the Temperature at the probe
  display.setCursor(0, 0);
  display.setTextSize(2); // Draw 2X-scale text
  display.setCursor(0, 32);
  display.print("TEMP (F): ");
  display.print(10000);
//?
  
  display.display();
}
//*/
