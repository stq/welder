#include <LiquidCrystal.h>
#include "display.h"
#include "model.h"
#include "settings.h"
#include "charger.h"
#include "gate.h"
#include "view.h"
#include "controller.h"
#include "sync.h"


void setup() {
  Display::init();
  Settings::init();
  Charge::init();
  Gate::init();
  Controller::init();
  View::init();
}

void loop() {
  // tickAll();
}
