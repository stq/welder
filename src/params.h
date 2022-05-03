#pragma once

#define MKS *1L
#define MS *1000L
#define S *1000000L

#define MEM_START 10

enum Mode {
    OneImpulse, DualImpulse, Burst, Meander
};
#define MODES_AMOUNT 4

enum Property {
    ImpulseLength,
    ImpulseDelay,
    SecondImpulseLength,
    BurstImpulseLength,
    BurstImpulseDelay,
    BurstLength,
    Frequency,
    Cooldown,
    ContactDetectDelay
};
#define PROPERTIES_AMOUNT 11

class Params {

private:

    static bool isPropertyApplicable(Property prop);

public:
    static void applyConstraints();

    static long impulseLength;
    static long impulseDelay;
    static long secondImpulseLength;
    static long burstImpulseLength;
    static long burstImpulseDelay;
    static long burstLength;
    static long frequency;
    static long cooldown;
    static long contactDetectDelay;
    static Mode mode;

    static Property getNextProperty(Property base, bool backward);

    static void modify(Property property, int shift, int multiplier);

    static void save();

    static void load();

    static bool isContinous();

    static long getValue(Property prop);

    static bool isWeldingMode();
};
