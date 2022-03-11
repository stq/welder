#include <EEPROM.h>
#include <Arduino.h>
#include "storage.h"

void Storage::read(int& value){
  EEPROM.get(0, value);
};

void Storage::write(int value){
  int currentValue;
  EEPROM.get(0, currentValue);
  if (currentValue != value) {
    Serial.print("Limit saved: ");
    Serial.print(value);
    EEPROM.put(0, value);
  }
};