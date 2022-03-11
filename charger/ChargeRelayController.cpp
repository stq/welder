#include "ChargeRelayController.h"

ChargeRelayController::ChargeRelayController() {
  pinMode(PIN_CHARGER_RELAY, OUTPUT);
};

bool ChargeRelayController::isChargePending(){
  return !charging && voltage < (limit - RELAY_HYSTERESIS);
};

long ChargeRelayController::getChargeThrottle(){
  if( voltage >= limit ) return 0;
  return max(CHARGE_MIN_THROTTLE, CHARGE_BASE_THROTTLE/long(limit - voltage));
}



long ChargeRelayController::getTimeBeforeCharge(){
  long now = millis();
  long timePassed = now - switchedOffAt;
  long timeRemaining = (getChargeThrottle() - timePassed)/1000;
  return timeRemaining < 0 ? 0 : timeRemaining;
};

void ChargeRelayController::update(){

  if ( !charging && isChargePending() ) {
      if( getTimeBeforeCharge() == 0 ) {
        charging = true;
        digitalWrite(PIN_CHARGER_RELAY, LOW);
      }
  } else if (charging && voltage >= limit ) {
    charging = false;
    switchedOffAt = millis();
    digitalWrite(PIN_CHARGER_RELAY, HIGH);
  }
};

int ChargeRelayController::getVoltage(){ return voltage; };
void ChargeRelayController::setVoltage(int _voltage) { voltage = _voltage; };
int ChargeRelayController::getLimit() { return limit; };
void ChargeRelayController::setLimit(int _limit) { limit = _limit; };
bool ChargeRelayController::isCharging() { return charging; };
