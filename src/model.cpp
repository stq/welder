#include <Arduino.h>
#include "global.h"
#include "model.h"
#include "storage.h"

Property  Model::property = Cooldown;

int      Model::multiplierLog10 = 1;

bool     Model::isAutoCountdown = false;
long     Model::startAutoCountdownTime = 0L;
long     Model::remainingAutoCountdownTime = 0L;

long     Model::remainingCooldownTime = 0L;

void Model::chooseNextProperty() {
    property = Params::getNextProperty(property, false);
};

void Model::choosePrevProperty() {
    property = Params::getNextProperty(property, true);
};
