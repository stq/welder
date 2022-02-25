#include <Arduino.h>
#include "global.h"
#include "model.h"
#include "storage.h"

Property  Model::property = Cooldown;

ulong       Model::multiplier = 1;

bool      Model::isContacted = false;
bool      Model::isAutoCountdown = false;
ulong     Model::startAutoCountdownTime = 0UL;
ulong     Model::remainingAutoCountdownTime = 0UL;

ulong     Model::remainingCooldownTime = 0UL;

void Model::chooseNextProperty() {
  property = Params::getNextProperty(property, false);
};

void Model::choosePrevProperty() {
  property = Params::getNextProperty(property, true);
};
