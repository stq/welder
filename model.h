#pragma once
#include "preset.h"

#define MODEL_MEM_ADDR 6
#define PRESETS_NUMBER 10



class Model {
public:
    static Property property;
    static bool isPropertyMode;
    static bool isIdle;
    static int voltage;
    static int current;
    static bool isCharging;
    static Preset* preset;
    static int presetIndex;

    static void chooseNextProperty();
    static void choosePrevProperty();
    static void chooseNextPreset();
    static void choosePrevPreset();
    static void init();
};
