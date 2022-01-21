#pragma once
#include "preset.h"

class Model {
public:
    static Property property;
    static int voltage;
    static int current;
    static bool isCharging;
    static Preset* preset;

    static void chooseNextProperty();
    static void choosePrevProperty();
    static void init();
};
