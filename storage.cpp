#include <EEPROM.h>
#include "global.h"

#include "storage.h"

int Storage::position = 0;

void Storage::from(int initialPosition){
  position = initialPosition;
};

void Storage::read(ulong& value){
  EEPROM.get(position, value);
  position += sizeof(ulong);
};

void Storage::read(int& value){
  EEPROM.get(position, value);
  position += sizeof(int);
};

void Storage::write(ulong value){
  ulong currentValue;
  EEPROM.get(position, currentValue);
  if (currentValue != value) Display::dbg("EEPROM WRITE");//EEPROM.put(shift, value);
  position += sizeof(ulong);
};

void Storage::write(int value){
  int currentValue;
  EEPROM.get(position, currentValue);
  if (currentValue != value) Display::dbg("EEPROM WRITE");//EEPROM.put(shift, value);
  position += sizeof(int);
};