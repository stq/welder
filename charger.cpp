#include <Arduino.h>
#include "global.h"
#include "charger.h"
#include "model.h"
#include "pins.h"

int             Charger::voltageReading = 0;
int             Charger::currentReading = 0;
bool            Charger::relayOn = false;
ulong           Charger::lastRelayChangeTimestamp = 0;

void Charger::init() { pinMode(PIN_CHARGER_RELAY, OUTPUT); }

void Charger::chargeOn() {
  ulong now = millis();
  if (now - lastRelayChangeTimestamp > RELAY_INERTIA) {
    digitalWrite(PIN_CHARGER_RELAY, HIGH);
    lastRelayChangeTimestamp = now;
    relayOn = true;
  }
}

void Charger::chargeOff() {
  ulong now = millis();
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


#define MAX_HISTORY 200
int average(int pin, int& cursor, int* history){
  cursor = (cursor + 1) % MAX_HISTORY;
  history[cursor] = analogRead(pin);
  float avg = 0;
  for( int i = 0; i < MAX_HISTORY; i++ ){
    avg += (float)history[i];
  }
  return round(avg/(float)MAX_HISTORY);
};


int voltageHistory[MAX_HISTORY];
int voltageCursor = 0;
int currentHistory[MAX_HISTORY];
int currentCursor = 0;
void Charger::tick() {
  voltageReading = average(PIN_VOLTAGE, voltageCursor, voltageHistory);
  currentReading = average(PIN_CURRENT, currentCursor, currentHistory);

  updateRelay();
  Model::isCharging = relayOn;
}
