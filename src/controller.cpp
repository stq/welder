#include <Arduino.h>
#include <GyverButton.h>
#include "global.h"
#include "pins.h"
#include "params.h"
#include "model.h"
#include "controller.h"
#include "gate.h"
#include "speaker.h"

GButton impulseButton(PIN_IMPULSE_BTN);
GButton leftButton(PIN_BTN_LEFT);
GButton rightButton(PIN_BTN_RIGHT);
GButton saveButton(PIN_BTN_SAVE);

int encoderPos = 0;
bool encoderInitialState, encoderLastState, encoderTurn;
int readAuxEncoder() {
  encoderInitialState = bitRead(PIND, PIN_AUX_ENCODER_A);
  if (encoderInitialState != encoderLastState) {
    encoderTurn = !encoderTurn;
    if (encoderTurn) encoderPos = (bitRead(PIND, PIN_AUX_ENCODER_B) != encoderLastState) ? 1 : -1;
    encoderLastState = encoderInitialState;
  }
};

void Controller::init() {
  attachInterrupt(0, readAuxEncoder, CHANGE);
};

void changeMode(int shift) {
  Params::mode = (MODES_AMOUNT + (int)Params::mode + shift) % MODES_AMOUNT;
  Model::property = Params::getNextProperty(Model::property, false);
  Model::property = Params::getNextProperty(Model::property, true);
}

ulong startCooldownTime;

int pass = 0;
void Controller::tick() {

  impulseButton.tick();
  leftButton.tick();
  rightButton.tick();
  saveButton.tick();

  bool impulseButtonPressed = impulseButton.isPress();
  bool leftButtonPressed = leftButton.isPress();
  bool rightButtonPressed = rightButton.isPress();
  bool altButtonPressed = analogRead(PIN_BTN_ALT) > 100;
  bool saveButtonPressed = saveButton.isPress();

  //Reset erroneous status of buttons as 'pressed' at start
  if( pass < 5 ) { pass++; delay(100); return; }

  if( impulseButtonPressed ) Serial.println("impulseButtonPressed");
  if( leftButtonPressed ) Serial.println("leftButtonPressed");
  if( rightButtonPressed ) Serial.println("rightButtonPressed");
  if( altButtonPressed ) Serial.println("altButtonPressed");
  if( saveButtonPressed ) Serial.println("saveButtonPressed");

  if( impulseButtonPressed ) Speaker::play(1000 + (altButtonPressed ? 1000 : 0), 100);
  if( leftButtonPressed ) Speaker::play(500 + (altButtonPressed ? 1000 : 0) , 25);
  if( rightButtonPressed ) Speaker::play(600 + (altButtonPressed ? 1000 : 0), 25);
  if( encoderPos != 0 ) Speaker::play(800 + encoderPos*100 + (altButtonPressed ? 1000 : 0) , 25);
  if( saveButtonPressed ) Speaker::play(900 + (altButtonPressed ? 1000 : 0), 25);

  if(altButtonPressed ) {
    if( leftButtonPressed ) changeMode(-1);
    else if ( rightButtonPressed ) changeMode(+1);
    else if( encoderPos != 0 ) Model::multiplier == constrain(encoderPos > 0 ? Model::multiplier * 10UL : Model::multiplier / 10UL, 1UL, 1000000UL);
    else if ( saveButtonPressed ) { Speaker::play(1000, 500); Params::save(); }
  } else {
    if( leftButtonPressed ) Model::choosePrevProperty();
    else if ( rightButtonPressed ) Model::chooseNextProperty();
    else if( encoderPos != 0 ) Params::modify(Model::property, encoderPos, Model::multiplier);
    else if ( saveButtonPressed ) { Speaker::play(2000, 500); Params::load(); }
  }
  encoderPos = 0;

  if( Model::remainingCooldownTime > 0 ) Model::remainingCooldownTime = Params::cooldown - (millis() - startCooldownTime);
  Model::isContacted = analogRead(PIN_CONTACT_SENSOR) > 700;

  if( Model::isAutoCountdown ) {
    if( !Model::isContacted ) {
      Model::isAutoCountdown = false;
    } else {
      Model::remainingAutoCountdownTime = Params::contactDetectDelay - (millis() - Model::startAutoCountdownTime);
      if( Model::remainingAutoCountdownTime <= 0 ) {
        Gate::startSequence();
        Model::isAutoCountdown = false;
      }
    }
  } else if( Model::isContacted ) {
    Model::isAutoCountdown = true;
    Model::startAutoCountdownTime = millis();
  }

  if (impulseButtonPressed) {
    if (Params::isContinous()) {
      if (Gate::isActive()) Gate::cancelSequence();
      else Gate::startSequence();
    } else if (!Gate::isActive()) {
      Gate::startSequence();
      Model::remainingCooldownTime = Params::cooldown;
      startCooldownTime = millis();
    }
    impulseButtonPressed = false;
  }

};
