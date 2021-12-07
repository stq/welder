#pragma once

#include <Arduino.h>

void render();

class View {
    static char line1[16];
    static char line2[16];

    static char* renderInterval(unsigned long interval);
    static char* renderVC(int vc);
    static void renderPresetPropertyValue();
    static void renderPresetDigest(char *buff, Preset p);
    static char getModeChar();
    static char *getModeString();
    static void renderBottomLine();
    static void renderTopLine();

public:
    static void init();
    static void tick();
};
