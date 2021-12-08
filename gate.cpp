#include <Arduino.h>
#include "display.h"
#include "preset.h"
#include "model.h"
#include "gate.h"
#include "pins.h"

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

void Gate::openFor(unsigned long mks) {
  if (mks <= MAX_MKS_DELAY) {
    digitalWrite(GATE_PIN, HIGH);
    delayMicroseconds(mks);
    digitalWrite(GATE_PIN, LOW);
  } else {
    unsigned long ms = mks / 1000;
    digitalWrite(GATE_PIN, HIGH);
    delay(ms);
    digitalWrite(GATE_PIN, LOW);
  }
};

void Gate::waitFor(unsigned long mks) {
  if (mks <= MAX_MKS_DELAY) delayMicroseconds(mks);
  else delay(mks / 1000);
};

void Gate::finishSequence() {
  isSequenceInProgress = false;
};


bool Gate::isActive() {
  return isSequenceInProgress;
};

void Gate::startSequence() {
  if (isSequenceInProgress) return;

  Preset* p = Model::preset;

  isSequenceInProgress = true;
  switch (p->mode) {
    case OneImpulse:
      openFor(p->impulseLength);
      waitFor(p->cooldown);
      finishSequence();
      return;
    case DualImpulse:
      openFor(p->impulseLength);
      waitFor(p->impulseDelay);
      openFor(p->secondImpulseLength);
      waitFor(p->cooldown);
      finishSequence();
      return;
    case TripleImpulse:
      openFor(p->impulseLength);
      waitFor(p->impulseDelay);
      openFor(p->secondImpulseLength);
      waitFor(p->secondImpulseDelay);
      openFor(p->thirdImpulseLength);
      waitFor(p->cooldown);
      finishSequence();
      return;
    case Burst:
      for (int i = 0; i < p->burstLength; i++) {
        openFor(p->impulseLength);
        waitFor(p->impulseDelay);
      }
      waitFor(p->cooldown);
      finishSequence();
      return;
    case Meander:
      //todo implement
      return;
    case Linear:
      open();
      return;
  }
};

void Gate::cancelSequence() {
  finishSequence();
};

void Gate::tick() {
};
