#include <Arduino.h>
#include "global.h"
#include "pins.h"
#include "display.h"

LiquidCrystal *lcd;
char buff1[16];
char buff2[16];
char *db[] = {buff1, buff2};
bool changed = true;
bool blinks = false;
int blinkX = 0;
int blinkY = 0;

void Display::init() {
    Serial.begin(9600);

    lcd = new LiquidCrystal(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
    lcd->begin(16, 2);
    lcd->setCursor(0, 0);
    lcd->noAutoscroll();
    char c_welder[] = {0x1F, 0x0E, 0x04, 0x00, 0x00, 0x04, 0x0E, 0x1F};
    char c_gen[] = {0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
    char c_impulse[] = {0x00, 0x00, 0x0E, 0x0A, 0x0A, 0x1B, 0x00, 0x00};
    lcd->createChar(1, c_welder);
    lcd->createChar(2, c_gen);
    lcd->createChar(3, c_impulse);
    lcd->setCursor(0, 0);
    lcd->print("<Impulse Welder>");
    lcd->setCursor(0, 1);
    lcd->print("initialization..");
    delay(1000);
}

void Display::print(int x, int y, char ch) {
    if( x > 15 || y > 1 ) return;
    if (db[y][x] != ch) {
        db[y][x] = ch;
        changed = true;
    }
}

void Display::print(int x, int y, int length, char *text) {
    for (int i = 0; i < length && (x+i) < 16; i++) {
        char c = text[i] != '#' ? text[i] : char(3);
        if (db[y][x + i] != c) {
            db[y][x + i] = c;
            changed = true;

        }
    }

}

void Display::tick() {
    if (changed) {
        for( int i = 0; i < 16; i ++ ){
            lcd->setCursor(i, 0);
            lcd->write(buff1[i] == 0 ? ' ' : buff1[i]);
            lcd->setCursor(i, 1);
            lcd->write(buff2[i] == 0 ? ' ' : buff2[i]);
        }

        changed = false;
        if (blinks) {
            lcd->setCursor(blinkX, blinkY);
            lcd->blink();
        } else lcd->noBlink();
    }

}

void Display::blink(int x, int y) {
    if( x > 15 || y > 1 ) return;

    if (blinkX != x || blinkY != y) {
        blinkX = x;
        blinkY = y;
        changed = true;
    }
    blinks = true;
};

void Display::blinkOff() {
    blinks = false;
};