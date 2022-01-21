#include <Arduino.h>
#include <GyverButton.h>
#include "global.h"
#include "pins.h"
#include "preset.h"
#include "model.h"
#include "controller.h"
#include "gate.h"
#include "charger.h"

GButton impulseButton(13);
GButton mainEncButton(11);
bool impulseButtonPressed = false;
bool isContact = false;
bool isEncoderButtonClick = false;
bool isEncoderButtonLongpress = false;
int mainEncoderPos = 0;
int auxEncoderPos = 0;


bool mainState0, mainLastState, mainTurnFlag;
void readModeEncoder() {
  mainState0 = digitalRead(PIN_MODE_ENCODER_A);//todo use bitRead
  if (mainState0 != mainLastState) {
    mainTurnFlag = !mainTurnFlag;
    if (mainTurnFlag) {
      mainEncoderPos = (digitalRead(PIN_MODE_ENCODER_B) != mainLastState) ? -1 : 1;
    }
    mainLastState = mainState0;
  }
};

bool auxState0, auxLastState, auxTurnFlag;
int auxLastDirection;
ulong auxChangedTimestamp = millis();
ulong auxChangedInterval;

int readAuxEncoder() {
  auxState0 = bitRead(PIND, PIN_AUX_ENCODER_A);
  if (auxState0 != auxLastState) {
    auxTurnFlag = !auxTurnFlag;
    if (auxTurnFlag) {
      auxEncoderPos = (bitRead(PIND, PIN_AUX_ENCODER_B) != auxLastState) ? 1 : -1;

      ulong now = millis();
      auxChangedInterval = now - auxChangedTimestamp;
      auxChangedTimestamp = now;

      //Reset activity watch if direction was changed
      if (auxEncoderPos != auxLastDirection) {
        auxChangedInterval = 1000; 
        auxLastDirection = auxEncoderPos;
      }

    }
    auxLastState = auxState0;
  }
};

void Controller::init() {
  attachInterrupt(0, readAuxEncoder, CHANGE);
  impulseButton.setType(LOW_PULL);
  mainEncButton.setType(LOW_PULL);
  Charger::init();
  Gate::init();
};

ulong contactCountdown = 0;

void Controller::tick() {
  Preset* p = Model::preset;

  readModeEncoder();
  impulseButton.tick();
  mainEncButton.tick();

  bool isEncPress = mainEncButton.isHold();

  Display::dbg(isEncPress ? "1" : "0");

  if (mainEncoderPos != 0) {
    if (mainEncoderPos > 0) Model::chooseNextProperty();
    else Model::choosePrevProperty();

    mainEncoderPos = 0;
  }

  if (auxEncoderPos != 0) {
    p->modify(Model::property, auxEncoderPos, auxChangedInterval, isEncPress);

    if( Model::property == Frequency ) {
      Gate::meander(p->frequency);
    }

    auxEncoderPos = 0;
  }

  if (isContact && p->enableContactDetect && !p->isContinous())
    contactCountdown = millis() + p->contactDetectDelay;

  if (contactCountdown != 0 && contactCountdown < millis()) {
    impulseButtonPressed = true;
    contactCountdown = 0;
  }

  if (impulseButtonPressed) {
    if (p->isContinous()) {
      if (Gate::isActive()) Gate::cancelSequence();
      else Gate::startSequence();
    } else if (!Gate::isActive()) {
      Gate::startSequence();
    }
    impulseButtonPressed = false;
  }

  Charger::tick();
  Gate::tick();
};
