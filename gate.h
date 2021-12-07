#pragma once

#define GATE_PIN 9
#define MAX_MKS_DELAY 16383

class Gate {
public:
    static void open();

    static void close();

    static void openFor(unsigned long mks);

    static void waitFor(unsigned long mks);

    static void finishSequence();

    static bool isSequenceInProgress;

    static void init();

    static void startSequence();

    static bool isActive();

    static void cancelSequence();

    static void tick();
};
