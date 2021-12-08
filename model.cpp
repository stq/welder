#include <Arduino.h>
#include "model.h"
#include "storage.h"

Property Model::property = ModeSelector;
bool Model::isPropertyMode = false;
bool Model::isIdle = false;
int Model::voltage = 0;
int Model::current = 0;
bool Model::isCharging = true;
Preset* Model::preset;
int Model::presetIndex;

void Model::init(){
  Storage::from(MODEL_MEM_ADDR);
  Storage::read(presetIndex);

  presetIndex = constrain(presetIndex, 0, 9);
  Model::preset = new Preset(presetIndex);
}

void Model::chooseNextProperty() {
//  property = preset->getNextProperty(property, false);
};

void Model::choosePrevProperty() {
//  property = preset->getNextProperty(property, true);
};

void Model::chooseNextPreset() {
  if (presetIndex < PRESETS_NUMBER - 1) {
    delete Model::preset;
    presetIndex++;
    Model::preset = new Preset(presetIndex);
    Storage::from(MODEL_MEM_ADDR);
    Storage::write(presetIndex);
  }
};

void Model::choosePrevPreset() {
  if (presetIndex > 0) {
    delete Model::preset;
    presetIndex--;
    Model::preset = new Preset(presetIndex);
    Storage::from(MODEL_MEM_ADDR);
    Storage::write(presetIndex);
  }
};
