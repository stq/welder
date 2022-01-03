#pragma once

#define GATE_PIN 9
#define MAX_MKS_DELAY 16383

class Gate {
public:
    static void open();

    static void close();

    static void openFor(ulong mks);

    static void waitFor(ulong mks);

    static void finishSequence();

    static bool isSequenceInProgress;

    static void init();

    static void startSequence();

    static bool isActive();

    static void cancelSequence();

    static void tick();

    static ulong meander(ulong freq);
};
