#include <Arduino.h>
#include "GyverPWM.h"
#include "global.h"
#include "params.h"
#include "model.h"
#include "gate.h"
#include "pins.h"

bool Gate::breakFlag;
bool Gate::isSequenceInProgress;

void Gate::init() {
    pinMode(PIN_GATE, OUTPUT);
};

void Gate::open() {
    digitalWrite(GATE_PIN, HIGH);
};

void Gate::close() {
    digitalWrite(GATE_PIN, LOW);
};

void Gate::openFor(long mks) {
    if (mks <= MAX_MKS_DELAY) {
        digitalWrite(GATE_PIN, HIGH);
        delayMicroseconds(mks);
        digitalWrite(GATE_PIN, LOW);
    } else {
        long ms = mks / 1000;
        digitalWrite(GATE_PIN, HIGH);
        delay(ms);
        digitalWrite(GATE_PIN, LOW);
    }
};

void Gate::waitFor(long mks) {
    if (mks <= MAX_MKS_DELAY) delayMicroseconds(mks);
    else delay(mks / 1000);
};


void Gate::finishSequence() {
    Serial.println("finishSequence");
    PWM_default(PIN_GATE);
    if( Params::isContinous() ) {
        isSequenceInProgress = false;
    } else {
        startCooldown();
    }
};

long startCooldownTime;
void Gate::startCooldown(){
    Serial.println("finishSequence");

    Model::remainingCooldownTime = Params::cooldown/1000;
    startCooldownTime = millis();
}

void Gate::tick(){
    if (Model::remainingCooldownTime > 0) {
        Serial.println("remainingCooldownTime > 0");

        Model::remainingCooldownTime = Params::cooldown/1000 - (millis() - startCooldownTime);
        Serial.println(Model::remainingCooldownTime);
    }

    if( isSequenceInProgress && Model::remainingCooldownTime <= 0 ) {
        Serial.println("isSequenceInProgress = false");
        isSequenceInProgress = false;
    }

}

bool Gate::isActive() {
    return isSequenceInProgress;
};

long Gate::meander(long freq, bool test) {
    long actualFreq = PWM_square_D9(float(freq));
    if (test) PWM_default(PIN_GATE);
    return long(actualFreq);
}

void Gate::startSequence() {
    if (isSequenceInProgress) return;
    breakFlag = false;

    isSequenceInProgress = true;
    switch (Params::mode) {
        case OneImpulse:
            openFor(Params::impulseLength);
            finishSequence();
            return;
        case DualImpulse:
            openFor(Params::impulseLength);
            waitFor(Params::impulseDelay);
            openFor(Params::secondImpulseLength);
            finishSequence();
            return;
        case TripleImpulse:
            openFor(Params::impulseLength);
            waitFor(Params::impulseDelay);
            openFor(Params::secondImpulseLength);
            waitFor(Params::secondImpulseDelay);
            openFor(Params::thirdImpulseLength);
            finishSequence();
            return;
        case Burst:
            for (int i = 0; i < Params::burstLength || Params::burstLength == 0; i++) {
                if (Gate::breakFlag) break;
                openFor(Params::impulseLength);
                waitFor(Params::impulseDelay);
            }
            finishSequence();
            return;
        case Meander:
            meander(Params::frequency, false);
            return;
    }
};

void Gate::cancelSequence() {
    breakFlag = true;
    finishSequence();
};