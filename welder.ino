#include "global.h"

#include "model.h"
#include "view.h"
#include "controller.h"

void setup() {
  Display::init();
  Model::init();
  Controller::init();
}

void loop() {
  updateView();
  Controller::tick();
}


#define FRAME_RATE 25
ulong lastViewUpdate = 0;
void updateView(){
  ulong now = millis();
  if( now - lastViewUpdate > 25){
    View::tick();
    lastViewUpdate = now;
  }
}