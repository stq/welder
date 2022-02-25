#pragma once
#include "params.h"

class Model {
public:
    static Property property;


    static ulong multiplier;

    static bool isContacted;
    static bool isAutoCountdown;
    static ulong startAutoCountdownTime;
    static ulong remainingAutoCountdownTime;

    static ulong remainingCooldownTime;

    static void chooseNextProperty();
    static void choosePrevProperty();

    static void debug();
};
