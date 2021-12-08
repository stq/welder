#include <Arduino.h>
#include "charger.h"
#include "model.h"
#include "pins.h"

int             Charger::voltageReading = 0;
int             Charger::currentReading = 0;
bool            Charger::relayOn = false;
unsigned long   Charger::lastRelayChangeTimestamp = 0;

void Charger::init() { pinMode(PIN_CHARGER_RELAY, OUTPUT); }

void Charger::chargeOn() {
  unsigned long now = millis();
  if (now - lastRelayChangeTimestamp > RELAY_INERTIA) {
    digitalWrite(PIN_CHARGER_RELAY, HIGH);
    lastRelayChangeTimestamp = now;
    relayOn = true;
  }
}

void Charger::chargeOff() {
  unsigned long now = millis();
  if ((now - lastRelayChangeTimestamp) > RELAY_INERTIA) {
    digitalWrite(PIN_CHARGER_RELAY, LOW);
    lastRelayChangeTimestamp = now;
    relayOn = false;
  }
}

void Charger::updateRelay() {
  int expectedLimitVoltageReading = round(COEF_VOLTAGE_DISPLAY_TO_PIN * Model::preset->chargeVoltageLimit);

  if (voltageReading < expectedLimitVoltageReading - RELAY_HYSTERESIS && !relayOn) chargeOn();
  else if (voltageReading > expectedLimitVoltageReading + RELAY_HYSTERESIS && relayOn) chargeOff();
}

void Charger::tick() {
  voltageReading = analogRead(PIN_VOLTAGE);
  currentReading = analogRead(PIN_CURRENT);
  Model::voltage = round(voltageReading * COEF_VOLTAGE_PIN_TO_DISPLAY);
  Model::current = round(currentReading * COEF_CURRENT_PIN_TO_DISPLAY);
  updateRelay();
  Model::isCharging = relayOn;
}
