#include "src/global.h"
#include "src/model.h"
#include "src/display.h"
#include "src/view.h"
#include "src/gate.h"
#include "src/speaker.h"
#include "src/controller.h"

void setup() {
    Display::init();
    Gate::init();
    Speaker::init();
    Params::load();
    Controller::init();
}


void loop() {
    Controller::tick();
    Gate::tick();
    View::tick();
    Display::tick();
}

