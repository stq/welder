#pragma once

#define GATE_PIN 9
#define MAX_MKS_DELAY 16383

class Gate {
public:
    static bool breakFlag;

    static void open();

    static void close();

    static void openFor(long mks);

    static void waitFor(long mks);

    static void finishSequence();

    static bool isSequenceInProgress;

    static void init();
    static void tick();
    static void startCooldown();

    static void startSequence();

    static bool isActive();

    static void cancelSequence();

    static long meander(long freq, bool test);
};
