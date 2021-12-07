#pragma once

#include "model.h"

#define SNAPSHOT_SIZE 84
#define MEM_START 10


class Preset {
public:
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

    void modify(PresetProperty property, int shift, int mult);

    void saveIfChanged(int shift, unsigned long value);

    void applyConstraints();

    void load(int index);

    void save(int index);

    bool isContinous();

    bool isPropertyApplicable(PresetProperty prop);
};
