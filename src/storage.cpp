#include <Arduino.h>
#include <EEPROM.h>
#include "global.h"

#include "storage.h"

int Storage::position = 0;

void Storage::from(int initialPosition) {
    position = initialPosition;
};

void Storage::read(long &value) {
    EEPROM.get(position, value);
    position += sizeof(long);
};

void Storage::write(long value) {
    long currentValue;
    EEPROM.get(position, currentValue);
    if (currentValue != value) EEPROM.put(position, value);
    position += sizeof(long);
};
