#include <LiquidCrystal.h>

#define CHAR_MODE_1IMP 1
#define CHAR_MODE_2IMP 2
#define CHAR_MODE_3IMP 3
#define CHAR_MODE_BURST 4
#define CHAR_MODE_MEANDER 5
#define CHAR_MODE_LINEAR 6

class Display {
    static LiquidCrystal *lcd;

public:
    static void dbg(String msg);

    static void dbg(char *msg, int val);

    static void init();

    static void printLines(char *line1, char *line2);

    static void focus(int x, int y);
};

