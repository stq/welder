#pragma once

#define COEF_VOLTAGE_PIN_TO_DISPLAY 0.25126
#define COEF_CURRENT_PIN_TO_DISPLAY 0.3333
#define COEF_CURRENT_PIN_SHIFT 512
#define COEF_VOLTAGE_DISPLAY_TO_PIN 1.0/COEF_VOLTAGE_PIN_TO_DISPLAY
#define RELAY_HYSTERESIS 0.2
#define RELAY_INERTIA 10000

class Charger {
private:
    static float voltageReading;
    static float currentReading;
    static bool relayOn;
    static ulong lastRelayChangeTimestamp;

public:

    static void chargeOn();

    static void chargeOff();

    static void updateRelay();

    static void init();

    static void tick();
};
