#include <LiquidCrystal.h>
#include "params.h"


class Display {

public:
    static void init();
    static LiquidCrystal *lcd;
    static void printLines(char *line1, char *line2);
    static void focus(int x, int y);
    static void displayMode(Mode mode);
    static void displayImpulseChar(int x, int y);
};
