#pragma once

#define MKS *1UL
#define MS *1000UL
#define S *1000000UL

#define SNAPSHOT_SIZE 84
#define MEM_START 10

enum Mode {
    OneImpulse, DualImpulse, TripleImpulse, Burst, Meander, Linear
};

enum Property {
    ModeSelector,
    ImpulseLength,
    ImpulseDelay, SecondImpulseLength,
    SecondImpulseDelay, ThirdImpulseLength,
    BurstLength,
    Frequency,
    ChargeVoltageLimit, Cooldown, EnableContactDetect, ContactDetectDelay
};
#define PROPERTIES_AMOUNT 12

class Preset {
private:
    int index;
    void applyConstraints();
    void saveIfChanged(int shift, ulong value);
    bool isPropertyApplicable(Property prop);
    void load();

public:
    Preset(int index);
    ~Preset();

    ulong impulseLength = 20000;
    ulong impulseDelay = 10000;
    ulong secondImpulseLength = 30000;
    ulong secondImpulseDelay = 20000;
    ulong thirdImpulseLength = 10000;
    int burstLength = 99;
    ulong frequency = 100;
    int chargeVoltageLimit = 149;
    ulong cooldown = 1000;
    int enableContactDetect = 0;
    ulong contactDetectDelay = 2000;
    Mode mode;

    Property getNextProperty(Property base, bool backward);
    void modify(Property property, int shift, int mult);
    void save();
    bool isContinous();
};
