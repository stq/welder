#include <Arduino.h>
#include "pins.h"
#include "speaker.h"

void midi() {
    int tonePin = PIN_SPEAKER;
    tone(tonePin, 493, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 987, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 739, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 622, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 987, 53.0660625);
    delay(58.9622916667);
    tone(tonePin, 739, 53.0660625);
    delay(58.9622916667);
    delay(117.924583333);
    tone(tonePin, 622, 159.1981875);
    delay(176.886875);
    delay(58.9622916667);
    tone(tonePin, 523, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 1046, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 783, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 659, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 1046, 53.0660625);
    delay(58.9622916667);
    tone(tonePin, 783, 53.0660625);
    delay(58.9622916667);
    delay(117.924583333);
    tone(tonePin, 659, 159.1981875);
    delay(176.886875);
    delay(58.9622916667);
    tone(tonePin, 493, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 987, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 739, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 622, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 987, 53.0660625);
    delay(58.9622916667);
    tone(tonePin, 739, 53.0660625);
    delay(58.9622916667);
    delay(117.924583333);
    tone(tonePin, 622, 159.1981875);
    delay(176.886875);
    delay(58.9622916667);
    tone(tonePin, 622, 53.0660625);
    delay(58.9622916667);
    tone(tonePin, 659, 53.0660625);
    delay(58.9622916667);
    tone(tonePin, 698, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 698, 53.0660625);
    delay(58.9622916667);
    tone(tonePin, 739, 53.0660625);
    delay(58.9622916667);
    tone(tonePin, 783, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 783, 53.0660625);
    delay(58.9622916667);
    tone(tonePin, 830, 53.0660625);
    delay(58.9622916667);
    tone(tonePin, 880, 53.0660625);
    delay(58.9622916667);
    delay(58.9622916667);
    tone(tonePin, 987, 106.132125);
    delay(117.924583333);

}

void Speaker::init() {
    pinMode(PIN_SPEAKER, OUTPUT);
    if( random(100) == 1 ) midi();
};

void Speaker::play(int f, int t) {
    tone(PIN_SPEAKER, f, t);
};


