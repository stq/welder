#pragma once

#define MKS *1
#define MS *1000
#define S *1000000

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

class Preset {
private:
    int index;

    unsigned long modifyInterval(unsigned long val, int shift, int mul, int minVal, long maxVal);
    unsigned long modifyFrequency(unsigned long val, int shift, int mul, int minVal, long maxVal);
    void applyConstraints();
    void saveIfChanged(int shift, unsigned long value);
    bool isPropertyApplicable(Property prop);
    void load();

public:
    Preset(int index);
    ~Preset();

    unsigned long impulseLength = 20000;
    unsigned long impulseDelay = 10000;
    unsigned long secondImpulseLength = 30000;
    unsigned long secondImpulseDelay = 20000;
    unsigned long thirdImpulseLength = 10000;
    unsigned long burstLength = 99;
    unsigned long frequency = 432;
    unsigned long chargeVoltageLimit = 149;
    unsigned long cooldown = 1000;
    unsigned long enableContactDetect = 0;
    unsigned long contactDetectDelay = 2000;
    Mode mode;

    Property getNextProperty(Property base, bool backward);
    void modify(Property property, int shift, int mult);
    void save();
    bool isContinous();
};
