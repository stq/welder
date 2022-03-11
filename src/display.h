#pragma once

#include <LiquidCrystal.h>
#include "params.h"


class Display {
public:
    static void init();

    static void tick();

    static void print(int x, int y, int length, char *text);

    static void print(int x, int y, char ch);

    static void blink(int x, int y);

    static void blinkOff();
};
