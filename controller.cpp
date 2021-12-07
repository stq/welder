#include <Arduino.h>
#include <GyverButton.h>
#include "display.h"
#include "pins.h"
#include "preset.h"
#include "controller.h"
#include "settings.h"
#include "gate.h"

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
  dbg("Init Controller");
  impulseButton.setType(LOW_PULL);
  mainEncButton.setType(LOW_PULL);
  dbg("Controller OK");
};

unsigned long contactCountdown = 0;
Preset preset;

void Controller::tick() {
  readModeEncoder();
  readAuxEncoder();
  impulseButton.tick();
  mainEncButton.tick();
  preset = Settings::getCurrentPreset();

  if (mainEncButton.isPress()) {
    View::isPresetSettingsEditMode = !View::isPresetSettingsEditMode;
  }

  if (mainEncoderPos != 0) {
    if (Model::isPresetSettingsEditMode) {
      if (mainEncoderPos > 0) Model::chooseNextPresetProperty();
      else Model::choosePrevPresetProperty();
    } else {
      if (mainEncoderPos > 0) Settings::chooseNextPreset();
      else Settings::choosePrevPreset();
    }
  }

  if (auxEncoderPos != 0) {
    if (Model::isPresetSettingsEditMode) {
      Preset::modify(Model::selectedPresetProperty, auxEncoderPos, auxEncoderMultiplier);
    } else {
    }
  }


  if (isContact && preset.enableContactDetect && !preset.isContinous())
    contactCountdown = millis() + preset.contactDetectDelay;

  if (contactCountdown != 0 && contactCountdown < millis()) {
    impulseButtonPressed = true;
    contactCountdown = 0;
  }

  if (impulseButtonPressed) {
    if (preset.isContinous()) {
      if (Gate::isActive()) Gate::cancelSequence();
      else Gate::startSequence();
    } else if (!Gate::isActive()) {
      Gate::startSequence();
    }
    impulseButtonPressed = false;
  }
};
