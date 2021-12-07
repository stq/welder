#pragma once

#include <Arduino.h>

void render();

class View {
public:
    static char line1[16];
    static char line2[16];
    static byte cursorPosX;
    static byte cursorPosY;

    static void renderPresetPropertyValue();

    static void renderPresetDigest(char *buff, Preset p);

    static char getModeChar();

    static char *getModeString();

    static void renderBottomLine();

    static void renderTopLine();

    static void init();

    static void tick();
};
