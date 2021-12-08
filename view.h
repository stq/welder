#pragma once

#include <Arduino.h>

class View {
    static char line1[16];
    static char line2[16];

    static char* renderInterval(unsigned long interval);
    static char* renderVC(int vc);
    static void renderPropertyValue();
    static void renderPresetDigest(char *buff, Preset p);
    static char getModeChar();
    static char *getModeString();
    static void renderBottomLine();
    static void renderTopLine();

public:
    static void tick();
};
