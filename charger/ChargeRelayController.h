#pragma once

#include <Arduino.h>

#define RELAY_HYSTERESIS 3
#define PIN_CHARGER_RELAY 12
#define CHARGE_BASE_THROTTLE 180000L
#define CHARGE_MIN_THROTTLE 15000L

class ChargeRelayController {
private:
    long switchedOffAt;
    bool charging = true;
    int voltage;
    int limit;
public:
    ChargeRelayController();

    void update();

    int getVoltage();

    void setVoltage(int _voltage);

    int getLimit();

    void setLimit(int _limit);

    bool isCharging();

    bool isChargePending();

    long getChargeThrottle();

    long getTimeBeforeCharge();
};

