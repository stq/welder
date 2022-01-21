#include <Arduino.h>
#include "global.h"
#include "model.h"
#include "storage.h"

Property Model::property = ModeSelector;
int Model::voltage = 0;
int Model::current = 0;
bool Model::isCharging = true;
Preset* Model::preset;

void Model::init(){
  Model::preset = new Preset();
}

void Model::chooseNextProperty() {
  property = preset->getNextProperty(property, false);
};

void Model::choosePrevProperty() {
  property = preset->getNextProperty(property, true);
};