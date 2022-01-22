#include <Arduino.h>
#include "global.h"
#include "pins.h"

LiquidCrystal *Display::lcd;

void Display::dbg(String msg) {
    Serial.println(msg);
}

void Display::dbg(char *msg, ulong val) {
    Serial.print(msg);
    Serial.print(' ');
    Serial.println(val);
}

void Display::dbg(char *msg, float val) {
    Serial.print(msg);
    Serial.print(' ');
    Serial.println(val);
}

void Display::dbg(char *msg, int val) {
    Serial.print(msg);
    Serial.print(' ');
    Serial.println(val);
}

void Display::dbg(char *msg, ulong val, ulong val2) {
    Serial.print(msg);
    Serial.print(' ');
    Serial.print(val);
    Serial.print(' ');
    Serial.println(val2);
}

void Display::init() {
  Serial.begin(9600);
  Display::dbg("Initialization..");


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


int hash = 0;
bool checkHash(char* l1, char* l2){
  int sum = 0;
  for( int i = 0; i < 16; i++ ){
    sum += l1[i] + l2[i];
  }
  if( hash != sum ) {
    hash = sum;
    return false;
  };
  return true;
}


void Display::printLines(char *line1, char *line2) {
  if( checkHash(line1, line2) ) return;

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
