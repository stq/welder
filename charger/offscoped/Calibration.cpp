#include <Arduino.h>
#include "AnalogReader.h"
#include "Calibration.h"

AnalogReader vr(A7);

void Calibration::start() {

    float voltageOn, voltageOff;

    for (int t = 0; t < 30; t++) {

        digitalWrite(12, LOW);
        delay(60000);
        for (int i = 0; i < 30; i++) {
            voltageOn = vr.read();
            delay(10);
        }

        digitalWrite(12, HIGH);
        delay(50);
        for (int i = 0; i < 30; i++) {
            voltageOff = vr.read();
            delay(10);
        }

        float v = vr.read();

        float voltageOn2 = voltageOn * 0.0246F + 0.056f;
        float voltageOff2 = voltageOff * 0.0246F + 0.498f;

        Serial.print("raw on-off:");
        Serial.print(voltageOn);
        Serial.print(" & ");
        Serial.print(voltageOff);
        Serial.print("   display on-off: ");
        Serial.print(voltageOff2);
        Serial.print(" & ");
        Serial.print(voltageOn2);
        Serial.print(" D=");
        Serial.println(voltageOn - voltageOff);


    }
}
