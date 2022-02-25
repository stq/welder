#include <Arduino.h>
#include "global.h"
#include "pins.h"
#include "model.h"
#include "view.h"
#include "gate.h"

char topLine[14];
char bottomLine[14];

void View::init(){
}

void renderPropertyName() {
  Display::lcd->setCursor(2, 0);
  switch (Model::property) {
    case ImpulseLength:         Display::lcd->print("Length of i#1 "); Display::displayImpulseChar(13, 0);return;
    case ImpulseDelay:          Display::lcd->print("Wait after i#1"); Display::displayImpulseChar(14, 0);return;
    case SecondImpulseLength:   Display::lcd->print("Length of i#2 "); Display::displayImpulseChar(13, 0);return;
    case SecondImpulseDelay:    Display::lcd->print("Wait after i#1"); Display::displayImpulseChar(14, 0);return;
    case ThirdImpulseLength:    Display::lcd->print("Length of i#3 "); Display::displayImpulseChar(13, 0);return;
    case BurstImpulseLength:    Display::lcd->print("Impulse length"); Display::displayImpulseChar(13, 0);return;
    case BurstImpulseDelay:     Display::lcd->print("Pause betweeni"); Display::displayImpulseChar(15, 0);return;
    case BurstLength:           Display::lcd->print("Number of i   "); Display::displayImpulseChar(12, 0);return;
    case Frequency:             Display::lcd->print("Meander freq. "); return;
    case Cooldown:              Display::lcd->print("Cooldown after"); return;
    case ContactDetectDelay:    Display::lcd->print("Auto i delay  "); Display::displayImpulseChar(8, 0); return;
  }
};

char buff[16]
void renderAutoCountdown(){
  Display::lcd->setCursor(0, 0);
  Display::lcd->print("Contact detected");
  sprintf(buff, "Impulse in %l.%ls ", Model::remainingAutoCountdownTime/1000, (Model::remainingAutoCountdownTime%1000)/100);
  Display::lcd->setCursor(0, 1);
  Display::lcd->print(buff);
}

void renderCooldown(){
  Display::lcd->setCursor(0, 0);
  Display::lcd->print("Cooldown... Avai");
  sprintf(buff, "lable in %l.%ls...", Model::remainingCooldownTime/1000, (Model::remainingCooldownTime%1000)/100);
  Display::lcd->setCursor(0, 1);
  Display::lcd->print(buff);
}

char* getPropertyMetric(){
  switch(Model::property){
    case ImpulseLength:
    case ImpulseDelay:
    case SecondImpulseLength:
    case SecondImpulseDelay:
    case ThirdImpulseLength:    return "ms";
    case BurstImpulseLength:
    case BurstImpulseDelay:     return "us";
    case Cooldown:
    case ContactDetectDelay:    return "ms";
    case BurstLength:           return "  ";
    case Frequency:             return "Hz";
  }
}

ulong lastViewUpdate = 0;
void View::tick() {
  ulong now = millis();
  if (now - lastViewUpdate > 25) {
    if( Model::isAutoCountdown ) {
      renderAutoCountdown();
    } else if( Model::remainingCooldownTime > 0 ) {
      renderCooldown();
    } else{
      Display::displayMode(Params::mode);
      renderPropertyName();
      if( Model::property == BurstLength && Params::burstLength == 0 ) {
        Display::lcd->setCursor(2, 1);
        Display::lcd->print("till cancelled");
      } else {
        sprintf(buff, "    %7lu %s", Params::getValue(Model::property), getPropertyMetric());
        Display::lcd->setCursor(2, 1);
        Display::lcd->print(buff);
        Display::lcd->setCursor(13-logMultiplier, 1);
        Display::lcd->blink();
      }
    }


    Display::printLines(line1, line2);
    lastViewUpdate = now;
  }
};
