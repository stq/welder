#pragma once

#include "preset.h"

#define MEM_ADDR 6
#define PRESETS_NUMBER 10

class Settings {
public:
    static Preset presets[PRESETS_NUMBER];
    static int currentPresetIndex;
    static bool changed;

    static void save();

    static void load();

    static void init();

    static Preset getCurrentPreset();

    static int getCurrentPresetIndex();

    static void chooseNextPreset();

    static void choosePrevPreset();

    static void tick();
};
