#include <EEPROM.h>
#include "display.h"
#include "storage.h"

int Storage::position = 0;

void Storage::from(int initialPosition){
  position = initialPosition;
};

void Storage::read(unsigned long& value){
  EEPROM.get(position, value);
  position += sizeof(unsigned long);
};

void Storage::read(int& value){
  EEPROM.get(position, value);
  position += sizeof(int);
};

void Storage::write(unsigned long value){
  unsigned long currentValue;
  EEPROM.get(position, currentValue);
  if (currentValue != value) Display::dbg("EEPROM WRITE");//EEPROM.put(shift, value);
  position += sizeof(unsigned long);
};

void Storage::write(int value){
  int currentValue;
  EEPROM.get(position, currentValue);
  if (currentValue != value) Display::dbg("EEPROM WRITE");//EEPROM.put(shift, value);
  position += sizeof(int);
};