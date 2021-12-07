#include <Arduino.h>
#include "pins.h"
#include "display.h"

LiquidCrystal *Display::lcd;

void Display::dbg(String msg) {
  lcd->clear();
  lcd->print(msg);
  delay(100);
}

void Display::dbg(char *msg, int val) {
  char buff[16];
  sprintf(buff, "%s:%d", msg, val);
  lcd->clear();
  lcd->print(buff);
  delay(500);
}


#define CHAR_MODE_1IMP 1
#define CHAR_MODE_2IMP 2
#define CHAR_MODE_3IMP 3
#define CHAR_MODE_BURST 4
#define CHAR_MODE_MEANDER 5
#define CHAR_MODE_LINEAR 6

void Display::init() {
  lcd = new LiquidCrystal(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
  lcd->begin(16, 2);
  lcd->blink();
  lcd->setCursor(0, 0);
  lcd->noAutoscroll();

  const byte char_1i[] = {0x00, 0x09, 0x18, 0x09, 0x09, 0x09, 0x1D, 0x00};
  const byte char_2i[] = {0x00, 0x09, 0x14, 0x05, 0x09, 0x11, 0x1D, 0x00};
  const byte char_3i[] = {0x00, 0x09, 0x14, 0x05, 0x09, 0x05, 0x19, 0x00};
  const byte char_burst[] = {0x00, 0x15, 0x14, 0x09, 0x09, 0x15, 0x15, 0x00};
  const byte char_meander[] = {0x1C, 0x14, 0x17, 0x00, 0x00, 0x1C, 0x14, 0x17};

  lcd->createChar(CHAR_MODE_1IMP, char_1i);
  lcd->createChar(CHAR_MODE_2IMP, char_2i);
  lcd->createChar(CHAR_MODE_3IMP, char_3i);
  lcd->createChar(CHAR_MODE_BURST, char_burst);
  lcd->createChar(CHAR_MODE_MEANDER, char_meander);
}


void Display::printLines(char *line1, char *line2) {
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print(line1);
  lcd->setCursor(0, 1);
  lcd->print(line2);
}

void Display::focus(int x, int y) {
  lcd->setCursor(x, y);
  lcd->blink();
}
