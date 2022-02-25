#include <Arduino.h>
#include "GyverPWM.h"
#include "global.h"
#include "params.h"
#include "model.h"
#include "gate.h"
#include "pins.h"

bool Gate::isSequenceInProgress;
ulong Gate::meanderFrequency;

void Gate::init() {
  pinMode(PIN_GATE, OUTPUT);
};

void Gate::open() {
  digitalWrite(GATE_PIN, HIGH);
};

void Gate::close() {
  digitalWrite(GATE_PIN, LOW);
};

void Gate::openFor(ulong mks) {
  if (mks <= MAX_MKS_DELAY) {
    digitalWrite(GATE_PIN, HIGH);
    delayMicroseconds(mks);
    digitalWrite(GATE_PIN, LOW);
  } else {
    ulong ms = mks / 1000;
    digitalWrite(GATE_PIN, HIGH);
    delay(ms);
    digitalWrite(GATE_PIN, LOW);
  }
};

void Gate::waitFor(ulong mks) {
  if (mks <= MAX_MKS_DELAY) delayMicroseconds(mks);
  else delay(mks / 1000);
};

void Gate::finishSequence() {
  PWM_default(PIN_GATE);

  isSequenceInProgress = false;
};


bool Gate::isActive() {
  return isSequenceInProgress;
};

ulong Gate::meander(ulong freq){
  meanderFrequency = PWM_square_D9((float)freq);
  return meanderFrequency;
}

void Gate::startSequence() {
  if (isSequenceInProgress) return;

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
      for (int i = 0; i < Params::burstLength; i++) {
        openFor(Params::impulseLength);
        waitFor(Params::impulseDelay);
      }
      finishSequence();
      return;
    case Meander:
      meander(Params::frequency);
      return;
  }
};

void Gate::cancelSequence() {
  finishSequence();
};