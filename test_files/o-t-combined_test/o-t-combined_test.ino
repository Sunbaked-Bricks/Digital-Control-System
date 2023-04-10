// THERMOCOUPLE INIT //
#include "max6675.h"

int thermoCLK = D0;
int thermoCS = D3;
int thermoSO = D4;

int cur_temp = 0;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);
// THERMOCOUPLE INIT //

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
  // THERMOCOUPLE SETUP //
  // wait for MAX chip to stabilize
  delay(500);
  // THERMOCOUPLE SETUP //

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

  Serial.println("o-t_test");
}

void loop() {
  // basic readout test, just print the current temp
  cur_temp = thermocouple.readFahrenheit();
  Serial.print("C = "); 
  Serial.print(thermocouple.readCelsius());
  Serial.print(" | F = ");
  Serial.println(cur_temp);

  update_screen();
  
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);
}

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
