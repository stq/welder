#include <Arduino.h>
#include <EEPROM.h>
#include "model.h"
#include "preset.h"

void Preset::load(int index) {
  int shift = MEM_START + index * SNAPSHOT_SIZE;
  EEPROM.get(shift + 0, impulseLength);
  EEPROM.get(shift + 4, impulseDelay);
  EEPROM.get(shift + 8, secondImpulseLength);
  EEPROM.get(shift + 12, secondImpulseDelay);
  EEPROM.get(shift + 16, thirdImpulseLength);
  EEPROM.get(shift + 20, burstLength);
  EEPROM.get(shift + 24, frequency);
  EEPROM.get(shift + 28, chargeVoltageLimit);
  EEPROM.get(shift + 32, cooldown);
  EEPROM.get(shift + 36, enableContactDetect);
  EEPROM.get(shift + 40, contactDetectDelay);
  EEPROM.get(shift + 44, mode);
  applyConstraints();
};

void Preset::save(int index) {
  int shift = MEM_START + index * SNAPSHOT_SIZE;
  saveIfChanged(shift, impulseLength);
  saveIfChanged(shift + 4, impulseDelay);
  saveIfChanged(shift + 8, secondImpulseLength);
  saveIfChanged(shift + 12, secondImpulseDelay);
  saveIfChanged(shift + 16, thirdImpulseLength);
  saveIfChanged(shift + 20, burstLength);
  saveIfChanged(shift + 24, frequency);
  saveIfChanged(shift + 28, chargeVoltageLimit);
  saveIfChanged(shift + 32, cooldown);
  saveIfChanged(shift + 36, enableContactDetect);
  saveIfChanged(shift + 40, contactDetectDelay);
  saveIfChanged(shift + 44, (int) mode);
};

void Preset::saveIfChanged(int shift, unsigned long value) { //extends memory chip life
  unsigned long currentValue;
  EEPROM.get(shift, currentValue);
  if (currentValue != value) {
    dbg("SavingPreset");
    EEPROM.put(shift, value);
  }
}


bool Preset::isContinous() {
  return mode == Meander || mode == Linear;
};

bool Preset::isPropertyApplicable(PresetProperty prop) {

  switch (prop) {
    case ModeSelector:
    case ChargeVoltageLimit:
    case Cooldown:
    case EnableContactDetect:
      return true;
  }

  if (prop == ContactDetectDelay && enableContactDetect) return true;

  if (prop == Frequency && mode == Meander) return true;

  if (prop == BurstLength && mode == Burst) return true;

  if ((prop == SecondImpulseDelay || prop == ThirdImpulseLength) && mode == TripleImpulse) return true;

  if ((prop == ImpulseDelay || prop == SecondImpulseLength) && (mode == TripleImpulse || mode == DualImpulse))
    return true;

  if ((prop == ImpulseLength) && (mode == TripleImpulse || mode == DualImpulse || mode == OneImpulse)) return true;

  return false;
};

unsigned long modifyInterval(unsigned long oldVal, int shift, int mul, int minVal, long maxVal) {

  int baseStep = 1;
  int scale = mul == 1 ? 1 : mul == 2 ? 10 : 100;

  if (val < 1000) {
    baseStep = 1; //100 mks - 999 mks
  } else if (val < 10000) {
    baseStep = 100; //0.1 ms - 9.9 ms
    scale = min(mulScale, 10);
  } else if (val < 1000000) {
    baseStep = 1000; //1ms - 999ms
  } else if (val < 10000000) {
    baseStep = 100000;//0.1s - 9.9s
    scale = min(mulScale, 10);
  } else {
    baseStep = 1000000;//1s - 999s
  }

  return constrain(oldVal + baseStep * shift * scale, minVal, maxVal);
}

unsigned long modifyFrequency(unsigned long oldVal, int shift, int mul, int minVal, long maxVal) {

  int baseStep = 1;
  int scale = (int) pow(10, constrain(mul - 1, 0, 6);

  return constrain(oldVal + baseStep * shift * scale, minVal, maxVal);
}


void Preset::modify(PresetProperty property, int shift, int mult) {

  switch (property) {
    case ModeSelector:
      mode = constrain(mode + shift, 0, 5);
      return;

    case ImpulseLength:
      impulseLength = modifyInterval(impulseLength, shift, mult, 100 MKS, 200 MS);
      return;
    case ImpulseDelay:
      impulseDelay = modifyInterval(impulseDelay, shift, mult, 100 MKS, 999 S);
      return;
    case SecondImpulseLength:
      secondImpulseLength = modifyInterval(secondImpulseLength, shift, mult, 100 MKS, 200 MS);
      return;
    case SecondImpulseDelay:
      secondImpulseDelay = modifyInterval(secondImpulseDelay, shift, mult, 100 MKS, 999 S);
      return;
    case ThirdImpulseLength:
      thirdImpulseLength = modifyInterval(thirdImpulseLength, shift, mult, 100 MKS, 200 MS);
      return;
    case Cooldown:
      cooldown = modifyInterval(cooldown, shift, mult, 400 MS, 999 S);
      return;
    case ContactDetectDelay:
      contactDetectDelay = modifyInterval(contactDetectDelay, shift, mult, 100 MS, 999 S);
      return;

    case Frequency:
      frequency = modifyFrequency(frequency, shift, mult, 1, 4000000);
      return;

    case BurstLength:
      burstLength = constrain(burstLength + shift, 1, 100);
      return;
    case ChargeVoltageLimit:
      chargeVoltageLimit = constrain(chargeVoltageLimit + shift, 33, 150);
      return;
    case EnableContactDetect:
      enableContactDetect = !enableContactDetect;
      return;
  }

}

void Preset::applyConstraints() {
  impulseLength = constrain(impulseLength, 100 MKS, 200 MS);
  impulseDelay = constrain(impulseDelay, 100 MKS, 10 S);
  secondImpulseLength = constrain(secondImpulseLength, 100 MKS, 200 MS);
  secondImpulseDelay = constrain(secondImpulseDelay, 100 MKS, 10 S);
  thirdImpulseLength = constrain(thirdImpulseLength, 100 MKS, 200 MS);
  burstLength = constrain(burstLength, 1, 100);
  frequency = constrain(frequency, 1, 4000000);
  chargeVoltageLimit = constrain(chargeVoltageLimit, 33, 150);
  cooldown = constrain(cooldown, 400 MS, 60 S);
  enableContactDetect = constrain(enableContactDetect, 100 MS, 10 S);
  contactDetectDelay = constrain(contactDetectDelay, 100 MS, 10 S);
  mode = constrain(mode, 0, 5);
}
