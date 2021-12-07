#include <Arduino.h>
#include "display.h"
#include "preset.h"
#include "gate.h"
#include "settings.h"
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

  Preset preset;// = Settings.getCurrentPreset();

  isSequenceInProgress = true;
  switch (preset.mode) {
    case OneImpulse:
      openFor(preset.impulseLength);
      waitFor(preset.cooldown);
      finishSequence();
      return;
    case DualImpulse:
      openFor(preset.impulseLength);
      waitFor(preset.impulseDelay);
      openFor(preset.secondImpulseLength);
      waitFor(preset.cooldown);
      finishSequence();
      return;
    case TripleImpulse:
      openFor(preset.impulseLength);
      waitFor(preset.impulseDelay);
      openFor(preset.secondImpulseLength);
      waitFor(preset.secondImpulseDelay);
      openFor(preset.thirdImpulseLength);
      waitFor(preset.cooldown);
      finishSequence();
      return;
    case Burst:
      for (int i = 0; i < preset.burstLength; i++) {
        openFor(preset.impulseLength);
        waitFor(preset.impulseDelay);
      }
      waitFor(preset.cooldown);
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
