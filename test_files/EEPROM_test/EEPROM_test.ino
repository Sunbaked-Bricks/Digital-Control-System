//Libraries
#include <EEPROM.h>//https://github.com/esp8266/Arduino/blob/master/libraries/EEPROM/EEPROM.h
//Constants
#define EEPROM_SIZE 12

int address;

// I'm pretty sure this is the one I got to work

void setup() {
   //Init Serial USB
  Serial.begin(9600);
  delay(8000);
  Serial.println(F("Initialize System"));
  //Init EEPROM
  EEPROM.begin(EEPROM_SIZE);
  /*
  //Write data into eeprom
  int address = 0;
  int boardId = 18;
  EEPROM.put(address, boardId);
  address += sizeof(boardId); //update address value
  float param = 297.5;
  EEPROM.put(address, param);
  EEPROM.commit();
  //*/
  //Read data from eeprom
  address = 0;
  int readId;
  EEPROM.get(address, readId);
  Serial.print("Read Id = ");
  Serial.println(readId);
  address += sizeof(readId); //update address value
  float readParam;
  EEPROM.get(address, readParam); //readParam=EEPROM.readFloat(address);
  Serial.print("Read param = ");
  Serial.println(readParam);
  EEPROM.end();
}
void loop() {}
