#include <LiquidCrystal.h>

class Display {
    static LiquidCrystal *lcd;

    static void dbg(String msg);

    static void dbg(char *msg, int val);

    static void init();

    static void printLines(char *line1, char *line2);

    static void focus(int x, int y);
};

