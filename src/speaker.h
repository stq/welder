#pragma once

class Speaker {

public:
    static void midi();
    static void init();

    static void play(int freq, int time);
};
