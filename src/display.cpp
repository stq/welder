#include <Arduino.h>
#include "global.h"
#include "pins.h"

LiquidCrystal *Display::lcd;

void Display::init() {
  Serial.begin(9600);

  lcd = new LiquidCrystal(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
  lcd->begin(16, 2);
  lcd->setCursor(0, 0);
  lcd->noAutoscroll();
  char c_impulse1[] = {0x00,0x00,0x1C,0x14,0x14,0x17,0x00,0x00};
  char c_impulse2[] = {0x00,0x00,0x0E,0x0A,0x0A,0x1B,0x00,0x00};
  char c_impulse3[] = {0x00,0x00,0x07,0x05,0x05,0x1D,0x00,0x00};
  lcd->createChar(5, c_impulse1);
  lcd->createChar(6, c_impulse1);
  lcd->createChar(7, c_impulse1);
  printLines("<Impulse Welder>", "initialization..");
  delay(2000);
}

ulong lastTstamp = millis();
void Display::displayImpulseChar(int x, int y){
  int phase = (int ((lastTstamp - millis())/200UL)) % 3;
  lcd->setCursor(x, y);
  lcd->print((char)(phase+5);
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


void Display::displayMode(Mode mode){
  char c_TL_digit1[] = { 0x1F, 0x10, 0x12, 0x16, 0x12, 0x12, 0x12, 0x17};
  char c_TL_digit2[] = { 0x1F, 0x10, 0x17, 0x15, 0x11, 0x12, 0x14, 0x17};
  char c_TL_digit3[] = { 0x1F, 0x10, 0x17, 0x11, 0x17, 0x11, 0x11, 0x17};
  char c_TL_gen[] = { 0x1F, 0x10, 0x13, 0x17, 0x16, 0x16, 0x16, 0x16};
  char c_TR_letterI[] = { 0x1F, 0x01, 0x09, 0x01, 0x1D, 0x09, 0x09, 0x1D};
  char c_TL_letterX[] = { 0x1F, 0x10, 0x15, 0x15, 0x12, 0x12, 0x15, 0x15};
  char c_BL_impulseSignLeft[] = {0x10, 0x13, 0x12, 0x12, 0x12, 0x16, 0x10, 0x1F};
  char c_BL_gen[] = { 0x16, 0x16, 0x16, 0x16, 0x17, 0x13, 0x10, 0x1F};
  char c_TR_gen[] = { 0x01, 0x1D, 0x1D, 0x0D, 0x1D, 0x19, 0x01, 0x1F};
  char c_BR_impulseSignRight[] = {0x01, 0x19, 0x09, 0x09, 0x09, 0x0D, 0x01, 0x1F};
  char c_BR_gen[] = { 0x1F, 0x01, 0x19, 0x1D, 0x0D, 0x0D, 0x01, 0x01};


  switch(mode){
    case OneImpulse:
      lcd->createChar(1, c_TL_digit1);
      lcd->createChar(2, c_TR_letterI);
      lcd->createChar(3, c_BL_impulseSignLeft);
      lcd->createChar(4, c_BR_impulseSignRight);
    case DualImpulse:
      lcd->createChar(1, c_TL_digit2);
      lcd->createChar(2, c_TR_letterI);
      lcd->createChar(3, c_BL_impulseSignLeft);
      lcd->createChar(4, c_BR_impulseSignRight);
    case TripleImpulse:
      lcd->createChar(1, c_TL_digit3);
      lcd->createChar(2, c_TR_letterI);
      lcd->createChar(3, c_BL_impulseSignLeft);
      lcd->createChar(4, c_BR_impulseSignRight);
    case Burst:
      lcd->createChar(1, c_TR_letterX);
      lcd->createChar(2, c_TR_letterI);
      lcd->createChar(3, c_BL_impulseSignLeft);
      lcd->createChar(4, c_BR_impulseSignRight);
    case Burst:
      lcd->createChar(1, c_TR_gen);
      lcd->createChar(2, c_TR_gen);
      lcd->createChar(3, c_BL_gen);
      lcd->createChar(4, c_BR_gen);
  }
  lcd->setCursor(0,0);
  lcd->print((char)1);
  lcd->setCursor(1,0);
  lcd->print((char)2);
  lcd->setCursor(0,1);
  lcd->print((char)3);
  lcd->setCursor(1,1);
  lcd->print((char)4);
}