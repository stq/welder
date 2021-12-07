#pragma once
#define MKS *1
#define MS *1000
#define S *1000000

enum Mode {
    OneImpulse, DualImpulse, TripleImpulse, Burst, Meander, Linear
};

enum PresetProperty {
    ModeSelector, //common
    ImpulseLength, //for 1,2,3 impulses
    ImpulseDelay, SecondImpulseLength, //for 2,3 impulses and for 1 with repeats
    SecondImpulseDelay, ThirdImpulseLength, //for 3 impulses
    BurstLength,//for 1 impulse with specified delay and specified number of repeats
    Frequency, //for meander mode
    ChargeVoltageLimit, Cooldown, EnableContactDetect, ContactDetectDelay
};//common

class Model {
public:
    static PresetProperty selectedPresetProperty;
    static bool isPresetSettingsEditMode;
    static bool isIdle;
    static int voltage;
    static int current;
    static bool isCharging;

    static void chooseNextPresetProperty();
    static void choosePrevPresetProperty();

};
