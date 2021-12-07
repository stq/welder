#pragma once

#define COEF_VOLTAGE_PIN_TO_DISPLAY 0.1466275659824047
#define COEF_VOLTAGE_DISPLAY_TO_PIN 1.0/COEF_VOLTAGE_PIN_TO_DISPLAY
#define COEF_CURRENT_PIN_TO_DISPLAY 0.1
#define RELAY_HYSTERESIS 10
#define RELAY_INERTIA 10000

class Charge {
private:
    static int voltageReading;
    static int currentReading;
    static bool relayOn;
    static unsigned long lastRelayChangeTimestamp;

public:

    static void chargeOn();

    static void chargeOff();

    static void updateRelay();

    static void init();

    static void tick();
};
