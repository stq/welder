#include <Arduino.h>
#include <EEPROM.h>
#include "Preset.h"
#include "Settings.h"
#include "display.h"

int Settings::currentPresetIndex = 0;
bool Settings::changed = false;
Preset Settings::presets[PRESETS_NUMBER];

void Settings::save() {

  if (!changed) return;

  int currentValue;
  EEPROM.get(MEM_ADDR, currentValue);
  if (currentValue != currentPresetIndex) {
    dbg("SavingSetting");
    //EEPROM.put(MEM_ADDR, currentPresetIndex);
  }

  changed = false;
};


void Settings::init() {
  dbg("Loading Settings");
  load();
  dbg("CurrPresetInd", currentPresetIndex);
};

void Settings::load() {
  for (int i = 0; i < PRESETS_NUMBER; i++) {
    presets[i] = Preset();
    presets[i].load(i);
  }
  EEPROM.get(MEM_ADDR, currentPresetIndex);
  currentPresetIndex = constrain(currentPresetIndex, 0, 9);
};

Preset Settings::getCurrentPreset() {
  return presets[currentPresetIndex];
};

int Settings::getCurrentPresetIndex() {
  return currentPresetIndex;
};

void Settings::chooseNextPreset() {
  if (currentPresetIndex < (PRESETS_NUMBER - 1)) {
    currentPresetIndex++;
    changed = true;
  }
};

void Settings::choosePrevPreset() {
  if (currentPresetIndex > 0) {
    currentPresetIndex--;
    changed = true;
  }
};


void Settings::tick() {
  save();
};
