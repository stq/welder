#include "src/global.h"
#include "src/params.h"
#include "src/model.h"
#include "src/view.h"
#include "src/gate.h"
#include "src/speaker.h"
#include "src/controller.h"
//#include "src/health.h"

void setup() {
//  Health::check();
  Display::init();
  Params::init();
  Gate::init();
  Speaker::init();
  Controller::init();
  View::init();
}

void loop() {
  Controller::tick();
  View::tick();

}

