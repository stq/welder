#include <Arduino.h>
#include "global.h"
#include "preset.h"
#include "storage.h"
#include "gate.h"

Preset::Preset(){
  load();
};

Preset::~Preset(){
  save();
};

void Preset::load() {
  Storage::from(MEM_START);
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
  Storage::from(MEM_START);
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
  if (prop == ModeSelector || prop == ChargeVoltageLimit ) return true;
  if (prop == Frequency && mode == Meander) return true;
  if ((prop == ImpulseDelay || prop == BurstLength) && mode == Burst) return true;
  if ((prop == SecondImpulseDelay || prop == ThirdImpulseLength) && mode == TripleImpulse) return true;
  if ((prop == ImpulseDelay || prop == SecondImpulseLength) && (mode == TripleImpulse || mode == DualImpulse)) return true;
  if ((prop == ImpulseLength) && (mode == TripleImpulse || mode == DualImpulse || mode == OneImpulse || mode == Burst)) return true;
  if (prop == EnableContactDetect &&( mode == TripleImpulse || mode == DualImpulse || mode == OneImpulse)) return true;
  if (prop == ContactDetectDelay && enableContactDetect && ( mode == TripleImpulse || mode == DualImpulse || mode == OneImpulse)) return true;
  if (prop == Cooldown && ( mode == TripleImpulse || mode == DualImpulse || mode == OneImpulse)) return true;

  return false;
};

Property Preset::getNextProperty(Property base, bool backward){
  int i = (int)base;
  while(true){
    i = (i + PROPERTIES_AMOUNT + (backward ? -1 : 1)) % PROPERTIES_AMOUNT;
    Property nextProperty = static_cast<Property> (i);
    Display::dbg(isPropertyApplicable(nextProperty) ? "1" : "0");
    if(isPropertyApplicable(nextProperty)) return nextProperty;
  }
};

ulong modifyInterval(ulong val, int shift, int interval, bool slow, ulong minVal, ulong maxVal) {
  float changeSpeed = slow ? 1 : (1000.0/interval);
  int diff = 0;

  if (val < 1000) { //100 mks - 999 mks
    diff = round(shift * 1.0 * changeSpeed * 2.0);
  } else if (val < 10000) { //0.1 ms - 9.9 ms111
    diff = round(shift * 100 * changeSpeed * 0.5);
    if( val + diff < 1000 ) diff = 1000 - val;
  } else if (val < 1000000) { //1ms - 999ms
    diff = round(shift * 1000 * changeSpeed * 1.0);
    if( val + diff < 10000 ) diff = 10000 - val;
  } else if (val < 10000000) { //0.1s - 9.9s
    diff = round(shift * 100000 * changeSpeed * 1.0);
    if( val + diff < 1000000 ) diff = 1000000 - val;
  } else { //1s - 999s
    diff = round(shift * 1000000 * changeSpeed * 5.0);
    if( val + diff < 10000000 ) diff = 10000000 - val;
  };
  
  ulong result = constrain(val + diff, minVal, maxVal);

  ulong base = 1;
  if (val < 1000) { 
    base = 1;
  } else if (val < 10000) {
    base = 100;
  } else if (val < 1000000) {
    base = 1000;
  } else if (val < 10000000) {
    base = 100000;
  } else {
    base = 1000000;
  };

  return result - (result%base);
};

ulong modifyFrequency(ulong freq, int shift, int interval, bool slow) {

  int iterations = slow ? 1 : min(max(1, round(200.0/(float)interval)), 100);
  Display::dbg("iterations", iterations);

  float result = (float)freq;

  for( int i = 0; i < iterations; i++ ){

    float diff = result*0.001;
    diff = diff * diff * 0.1;
    diff = diff < 1 ? 1 : diff;
    Display::dbg("diff", diff);

    if( diff < 1 ) diff = 1;

    if( shift < 0 ) {
      result = result - diff;
      if( result < 1.0 ) return 1;
    } else {
      result = result + diff;
      if( result > 4000000.0 ) return 4000000UL;
    }
  }

  return (ulong)result;
};

void Preset::modify(Property property, int shift, int interval, bool slow) {
  switch (property) {
    case ModeSelector:    mode = (6 + (int)mode + shift)%6;
    case ImpulseLength:   impulseLength = modifyInterval(impulseLength, shift, interval, slow, 100 MKS, 200 MS); return;
    case ImpulseDelay:    impulseDelay = modifyInterval(impulseDelay, shift, interval, slow, 100 MKS, 999 S); return;
    case SecondImpulseLength:   secondImpulseLength = modifyInterval(secondImpulseLength, shift, interval, slow, 100 MKS, 200 MS); return;
    case SecondImpulseDelay:    secondImpulseDelay = modifyInterval(secondImpulseDelay, shift, interval, slow, 100 MKS, 999 S); return;
    case ThirdImpulseLength:    thirdImpulseLength = modifyInterval(thirdImpulseLength, shift, interval, slow, 100 MKS, 200 MS); return;
    case Frequency:     frequency = modifyFrequency(frequency, shift, interval, slow);return;
    case BurstLength:   burstLength = constrain(burstLength + shift*(slow ? 1 : 10), 1, 100); return;
    case Cooldown:      cooldown = modifyInterval(cooldown, shift, interval, slow, 400 MS, 999 S); return;
    case ChargeVoltageLimit:  chargeVoltageLimit = constrain(chargeVoltageLimit + shift*(slow ? 1 : 10), 33, 150); return;
    case EnableContactDetect: enableContactDetect = !enableContactDetect; return;
    case ContactDetectDelay:  contactDetectDelay = modifyInterval(contactDetectDelay, shift, interval, slow, 100 MS, 999 S); return;
  }
};

void Preset::applyConstraints() {
  modify(ModeSelector,          0, 1000, false);
  modify(ImpulseLength,         0, 1000, false);
  modify(ImpulseDelay,          0, 1000, false);
  modify(SecondImpulseLength,   0, 1000, false);
  modify(SecondImpulseDelay,    0, 1000, false);
  modify(ThirdImpulseLength,    0, 1000, false);
  modify(BurstLength,           0, 1000, false);
  modify(Frequency,             0, 1000, false);
  modify(ChargeVoltageLimit,    0, 1000, false);
  modify(Cooldown,              0, 1000, false);
  modify(EnableContactDetect,   0, 1000, false);
  modify(ContactDetectDelay,    0, 1000, false);
};
