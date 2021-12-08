#include <Arduino.h>
#include <GyverButton.h>
#include "display.h"
#include "pins.h"
#include "preset.h"
#include "model.h"
#include "controller.h"
#include "gate.h"
#include "charger.h"

GButton impulseButton(13);
GButton mainEncButton(11);
bool impulseButtonPressed;
bool isContact;
bool isEncoderButtonClick;
boolean isEncoderButtonLongpress;
int mainEncoderPos;
int auxEncoderPos;


volatile boolean mainState0, mainLastState, mainTurnFlag;

void readModeEncoder() {
  mainState0 = digitalRead(PIN_MODE_ENCODER_A);//todo use bitRead
  if (mainState0 != mainLastState) {
    mainTurnFlag = !mainTurnFlag;
    if (mainTurnFlag) //todo use bitRead
      mainEncoderPos = (digitalRead(PIN_MODE_ENCODER_B) != mainLastState) ? -1 : 1;
    mainLastState = mainState0;
  }
};

volatile boolean auxState0, auxLastState, auxTurnFlag;
volatile unsigned long lastAuxActiveTime = millis();
volatile int auxLastDirection;
volatile unsigned long totalRestTimeInterval;
volatile unsigned long lastActivityStart;
#define ACTIVITY_RESET_TIME 1000
#define ACTIVITY_MULTIPLIER_TIME 1000
volatile int auxEncoderMultiplier = 1;

int readAuxEncoder() {
  auxState0 = bitRead(PIND, PIN_AUX_ENCODER_A);
  if (auxState0 != auxLastState) {
    auxTurnFlag = !auxTurnFlag;
    if (auxTurnFlag) {
      auxEncoderPos = (bitRead(PIND, PIN_AUX_ENCODER_B) != auxLastState) ? -1 : 1;

      unsigned long now = millis();

      //Reset activity watch if direction was changed
      if (auxEncoderPos != auxLastDirection) {
        lastActivityStart = now;
        auxLastDirection = auxEncoderPos;
      }

      unsigned long totalRestTimeInterval = now - lastAuxActiveTime;
      if (totalRestTimeInterval < ACTIVITY_RESET_TIME) {

        int activityTotalTime = now - lastActivityStart;
        auxEncoderMultiplier = activityTotalTime / ACTIVITY_MULTIPLIER_TIME;

      } else {
        lastActivityStart = now;
      }

    }
    auxLastState = auxState0;
  }
};

void Controller::init() {
  impulseButton.setType(LOW_PULL);
  mainEncButton.setType(LOW_PULL);
  Charger::init();
  Gate::init();
};

unsigned long contactCountdown = 0;

void Controller::tick() {
  Preset* p = Model::preset;

  readModeEncoder();
  readAuxEncoder();
  impulseButton.tick();
  mainEncButton.tick();

  if (mainEncButton.isPress()) {
    Model::isPropertyMode = !Model::isPropertyMode;
  }

  if (mainEncoderPos != 0) {
    if (Model::isPropertyMode) {
      if (mainEncoderPos > 0) Model::chooseNextProperty();
      else Model::choosePrevProperty();
    } else {
      if (mainEncoderPos > 0) Model::chooseNextPreset();
      else Model::choosePrevPreset();
    }
  }

  if (auxEncoderPos != 0) {
    if (Model::isPropertyMode) {
      p->modify(Model::property, auxEncoderPos, auxEncoderMultiplier);
    }
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
