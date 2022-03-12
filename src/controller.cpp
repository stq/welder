#include <Arduino.h>
#include <GyverEncoder.h>
#include <GyverButton.h>
#include "global.h"
#include "pins.h"
#include "params.h"
#include "model.h"
#include "controller.h"
#include "gate.h"
#include "speaker.h"
#include "sensor.h"

GButton impulseButton(PIN_IMPULSE_BTN);
GButton leftButton(PIN_BTN_LEFT);
GButton rightButton(PIN_BTN_RIGHT);
GButton saveButton(PIN_BTN_SAVE);

volatile long lastCancel = 0;
void cancelGateSequence() {
    if (Gate::isActive()) {
        long now = millis();
        if( now - lastCancel < 1000 ) return;
        lastCancel = now;
        Speaker::play(3000, 50);
        Gate::cancelSequence();
    }
}

volatile bool isEncTurn = false;
volatile bool encoderInitialState = false;
volatile bool encoderLastState = false;
volatile int turns = 0;
int updateEnc() {
    encoderInitialState = digitalRead(PIN_AUX_ENCODER_B);
    if (encoderInitialState != encoderLastState) {
        isEncTurn = !isEncTurn;
        if (isEncTurn) {
         turns += digitalRead(PIN_AUX_ENCODER_A) == encoderLastState ? -1 : 1;
        }
        encoderLastState = encoderInitialState;
    }
    isEncTurn = false;
};


void Controller::init() {
    pinMode(PIN_AUX_ENCODER_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_AUX_ENCODER_B), updateEnc, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_IMPULSE_BTN), cancelGateSequence, CHANGE);
    leftButton.setType(LOW_PULL);
    rightButton.setType(LOW_PULL);
};

void changeMode(int shift) {
    Params::mode = (MODES_AMOUNT + (int) Params::mode + shift) % MODES_AMOUNT;
    Model::property = Params::getNextProperty(Model::property, false);
    Model::property = Params::getNextProperty(Model::property, true);
    Model::multiplierLog10 = 1;
}

int pass = 0;

void Controller::tick() {
    int turnsLocal = turns;
    turns = 0;

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
    if (pass < 5) {
        pass++;
        delay(100);
        return;
    }

    if( Gate::isActive() ) {
        return;
    }

    if (impulseButtonPressed) Speaker::play(1000, 100);
    if (leftButtonPressed) Speaker::play(500 + (altButtonPressed ? 1000 : 0), 25);
    if (rightButtonPressed) Speaker::play(600 + (altButtonPressed ? 1000 : 0), 25);
    if (turnsLocal != 0) Speaker::play(800 + (turnsLocal < 0 ? -100 : 100) + (altButtonPressed ? 1000 : 0), 25);
    if (saveButtonPressed) Speaker::play(900 + (altButtonPressed ? 1000 : 0), 25);

    if (altButtonPressed) {
        if (leftButtonPressed) changeMode(-1);
        else if (rightButtonPressed) changeMode(+1);
        else if (turnsLocal != 0) Model::multiplierLog10 = constrain(Model::multiplierLog10 - turnsLocal, 1, 7);
        else if (saveButtonPressed) {
            Speaker::play(1000, 500);
            Params::save();
        }

        if( rightButton.isHold() && leftButton.isHold() ) {
            Speaker::midi();
            delay(1000);
            Model::fuse = !Model::fuse;
        }
    } else {
        if (leftButtonPressed) Model::choosePrevProperty();
        else if (rightButtonPressed) Model::chooseNextProperty();
        else if (turnsLocal != 0) Params::modify(Model::property, turnsLocal, Model::multiplierLog10);
        else if (saveButtonPressed) {
            Speaker::play(2000, 500);
            Params::load();
        }
    }

    Sensor::tick();

    if (Params::isWeldingMode()) {

        if (Model::isAutoCountdown) {
            if (!Sensor::isContacted) {
                Model::isAutoCountdown = false;
            } else {
                Model::remainingAutoCountdownTime = Params::contactDetectDelay/1000 - (millis() - Model::startAutoCountdownTime);
                if (Model::remainingAutoCountdownTime <= 0) {
                    Gate::startSequence();
                    Model::isAutoCountdown = false;
                }
            }
        } else if (Sensor::isContacted) {
            Model::isAutoCountdown = true;
            Model::startAutoCountdownTime = millis();
        }
    } else {
        Model::isAutoCountdown = false;
        Model::remainingCooldownTime = 0;
    }

    if (impulseButtonPressed) {
        Gate::startSequence();
    }
};
