#include <Arduino.h>
#include "preset.h"
#include "storage.h"

Preset::Preset(int _index){
  index = _index;
  load();
};

Preset::~Preset(){
  save();
};

void Preset::load() {
  Storage::from(MEM_START + index * SNAPSHOT_SIZE);
  Storage::read(impulseLength);
  Storage::read(impulseDelay);
  Storage::read(secondImpulseLength);
  Storage::read(secondImpulseDelay);
  Storage::read(thirdImpulseLength);
  Storage::read(burstLength);
  Storage::read(frequency);
  Storage::read(chargeVoltageLimit);
  Storage::read(cooldown);
  Storage::read(enableContactDetect);
  Storage::read(contactDetectDelay);
  int mode_int;
  Storage::read(mode_int);
  mode = static_cast<Mode>(mode_int);
  applyConstraints();
};

void Preset::save() {
  Storage::from(MEM_START + index * SNAPSHOT_SIZE);
  Storage::write(impulseLength);
  Storage::write(impulseDelay);
  Storage::write(secondImpulseLength);
  Storage::write(secondImpulseDelay);
  Storage::write(thirdImpulseLength);
  Storage::write(burstLength);
  Storage::write(frequency);
  Storage::write(chargeVoltageLimit);
  Storage::write(cooldown);
  Storage::write(enableContactDetect);
  Storage::write(contactDetectDelay);
  Storage::write((int)mode);
};

bool Preset::isContinous() {
  return mode == Meander || mode == Linear;
};

bool Preset::isPropertyApplicable(Property prop) {
  if (prop == ModeSelector || prop == ChargeVoltageLimit || prop ==  Cooldown || prop == EnableContactDetect ) return true;
  if (prop == ContactDetectDelay && enableContactDetect) return true;
  if (prop == Frequency && mode == Meander) return true;
  if ((prop == ImpulseDelay || prop == BurstLength) && mode == Burst) return true;
  if ((prop == SecondImpulseDelay || prop == ThirdImpulseLength) && mode == TripleImpulse) return true;
  if ((prop == ImpulseDelay || prop == SecondImpulseLength) && (mode == TripleImpulse || mode == DualImpulse)) return true;
  if ((prop == ImpulseLength) && (mode == TripleImpulse || mode == DualImpulse || mode == OneImpulse)) return true;
  return false;
};

Property Preset::getNextProperty(Property base, bool backward){
  for( int i = (int)base; ; backward ? i-- : i++ ){
    Property nextProperty = static_cast<Property> (i);
    if(isPropertyApplicable(nextProperty)) return nextProperty;
  };
};

unsigned long Preset::modifyInterval(unsigned long val, int shift, int mul, int minVal, long maxVal) {
  int baseStep = 1;
  int scale = mul == 1 ? 1 : mul == 2 ? 10 : 100;
  if (val < 1000) {
    baseStep = 1; //100 mks - 999 mks
  } else if (val < 10000) {
    baseStep = 100; //0.1 ms - 9.9 ms
    scale = min(scale, 10);
  } else if (val < 1000000) {
    baseStep = 1000; //1ms - 999ms
  } else if (val < 10000000) {
    baseStep = 100000;//0.1s - 9.9s
    scale = min(scale, 10);
  } else {
    baseStep = 1000000;//1s - 999s
  };
  return constrain(val + baseStep * shift * scale, minVal, maxVal);
};

unsigned long Preset::modifyFrequency(unsigned long val, int shift, int mul, int minVal, long maxVal) {
  int scale = (int) pow(10, constrain(mul - 1, 0, 6));
  return constrain(val + shift * scale, minVal, maxVal);
};

void Preset::modify(Property property, int shift, int mult) {
  switch (property) {
    case ModeSelector: mode = constrain(mode + shift, 0, 5); return;
    case ImpulseLength: impulseLength = modifyInterval(impulseLength, shift, mult, 100 MKS, 200 MS); return;
    case ImpulseDelay: impulseDelay = modifyInterval(impulseDelay, shift, mult, 100 MKS, 999 S); return;
    case SecondImpulseLength: secondImpulseLength = modifyInterval(secondImpulseLength, shift, mult, 100 MKS, 200 MS); return;
    case SecondImpulseDelay: secondImpulseDelay = modifyInterval(secondImpulseDelay, shift, mult, 100 MKS, 999 S); return;
    case ThirdImpulseLength: thirdImpulseLength = modifyInterval(thirdImpulseLength, shift, mult, 100 MKS, 200 MS); return;
    case Frequency: frequency = modifyFrequency(frequency, shift, mult, 1, 4000000);return;
    case BurstLength: burstLength = constrain(burstLength + shift, 1, 100); return;
    case Cooldown: cooldown = modifyInterval(cooldown, shift, mult, 400 MS, 999 S); return;
    case ChargeVoltageLimit: chargeVoltageLimit = constrain(chargeVoltageLimit + shift, 33, 150); return;
    case EnableContactDetect: enableContactDetect = !enableContactDetect; return;
    case ContactDetectDelay: contactDetectDelay = modifyInterval(contactDetectDelay, shift, mult, 100 MS, 999 S); return;
  }
};

void Preset::applyConstraints() {
  modify(ModeSelector, 0, 1);
  modify(ImpulseLength, 0, 1);
  modify(ImpulseDelay, 0, 1);
  modify(SecondImpulseLength, 0, 1);
  modify(SecondImpulseDelay, 0, 1);
  modify(ThirdImpulseLength, 0, 1);
  modify(BurstLength, 0, 1);
  modify(Frequency, 0, 1);
  modify(ChargeVoltageLimit, 0, 1);
  modify(Cooldown, 0, 1);
  modify(EnableContactDetect, 0, 1);
  modify(ContactDetectDelay, 0, 1);
};
