#include <Arduino.h>
#include "sensor.h"
#include "pins.h"

int readings[5];
int readingsIndex = 0;
bool Sensor::isContacted = false;

void Sensor::analyzeReadings(){
    int min = 1023;
    int max = 0;
    for( int i = 0; i < 5; i++ ){
        min = min(min, readings[i]);
        max = max(max, readings[i]);
    }
    int disp = max - min;
    isContacted =  disp < 30 && readings[4] > 200;
}

void Sensor::tick(){
    int value = analogRead(PIN_CONTACT_SENSOR);

    readings[readingsIndex++] = value;

    if( readingsIndex == 5 ) {
        analyzeReadings();
        readingsIndex = 0;
    }

}