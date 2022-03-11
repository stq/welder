#pragma once

#include "params.h"

class Model {
public:
    static Property property;

    static int multiplierLog10;

    static bool isAutoCountdown;
    static bool saving;
    static bool loading;
    static long startAutoCountdownTime;
    static long remainingAutoCountdownTime;

    static long remainingCooldownTime;

    static void chooseNextProperty();

    static void choosePrevProperty();

    static void debug();
};
