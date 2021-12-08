#include "display.h"
#include "model.h"
#include "view.h"
#include "controller.h"

void setup() {
  Display::init();
  Model::init();
  Controller::init();
}

void loop() {
  View::tick();
  Controller::tick();
}
