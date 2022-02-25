#include <Arduino.h>
#include "global.h"
#include "params.h"
#include "storage.h"
#include "gate.h"

ulong Params::impulseLength;
ulong Params::impulseDelay;
ulong Params::secondImpulseLength;
ulong Params::secondImpulseDelay;
ulong Params::thirdImpulseLength;
ulong Params::burstImpulseLength;
ulong Params::burstImpulseDelay;
ulong Params::burstLength;
ulong Params::frequency;
ulong Params::cooldown;
ulong Params::contactDetectDelay;
Mode  Params::mode = OneImpulse;

bool Params::isPropertyApplicable(Property prop) {
  Mode mode = Params::mode;

  if (mode == Meander && prop == Frequency) return true;
  if (mode == Burst && (prop == BurstImpulseLength || prop == BurstImpulseDelay || prop == BurstLength) ) return true;

  if ((mode == TripleImpulse || mode == DualImpulse || mode == OneImpulse) && (prop == ImpulseLength || prop == ContactDetectDelay || prop == Cooldown)) return true;
  if ((mode == TripleImpulse || mode == DualImpulse) && (prop == ImpulseDelay || prop == SecondImpulseLength)) return true;
  if ( mode == TripleImpulse && (prop == SecondImpulseDelay || prop == ThirdImpulseLength)) return true;

  return false;
};
void Params::applyConstraints() {
  modify(ImpulseLength,         0, 1);
  modify(ImpulseDelay,          0, 1);
  modify(SecondImpulseLength,   0, 1);
  modify(SecondImpulseDelay,    0, 1);
  modify(ThirdImpulseLength,    0, 1);
  modify(BurstImpulseLength,    0, 1);
  modify(BurstImpulseDelay,     0, 1);
  modify(BurstLength,           0, 1);
  modify(Frequency,             0, 1);
  modify(Cooldown,              0, 1);
  modify(ContactDetectDelay,    0, 1);
  if( (int) mode < 0 || (int) mode >= MODES_AMOUNT ) mode = OneImpulse;
};
void Params::load() {
  Storage::from(MEM_START);
  Storage::read(impulseLength);
  Storage::read(impulseDelay);
  Storage::read(secondImpulseLength);
  Storage::read(secondImpulseDelay);
  Storage::read(thirdImpulseLength);
  Storage::read(burstImpulseLength);
  Storage::read(burstImpulseDelay);
  Storage::read(burstLength);
  Storage::read(frequency);
  Storage::read(cooldown);
  Storage::read(contactDetectDelay);
  ulong mode_int;
  Storage::read(mode_int);
  mode = static_cast<Mode>((int)mode_int);
  applyConstraints();
};

void Params::save() {
  Storage::from(MEM_START);
  Storage::write(impulseLength);
  Storage::write(impulseDelay);
  Storage::write(secondImpulseLength);
  Storage::write(secondImpulseDelay);
  Storage::write(thirdImpulseLength);
  Storage::write(burstImpulseLength);
  Storage::write(burstImpulseDelay);
  Storage::write(burstLength);
  Storage::write(frequency);
  Storage::write(cooldown);
  Storage::write(contactDetectDelay);
  Storage::write((ulong)mode);
};

bool Params::isContinous() {
  return mode == Meander;
};



Property Params::getNextProperty(Property base, bool backward){
  int i = (int)base;
  while(true){
    i = (i + PROPERTIES_AMOUNT + (backward ? -1 : 1)) % PROPERTIES_AMOUNT;
    Property nextProperty = static_cast<Property> (i);
    if(isPropertyApplicable(nextProperty)) return nextProperty;
  }
};

ulong modifyCooldownInterval(ulong val, int shift, ulong multiplier) {
  int scale = multiplier > 1 ? 10 : 100;
  return constrain( scale*(val/scale) + scale * shift * 1 MS, 10 MS, 9 S);
};

ulong modifyWelderInterval(ulong val, int shift, int multiplier) {
  int scale = multiplier > 1 ? 1 : 10;
  return constrain( scale*(val/scale) + scale * shift * 1 MS, 1 MS, 200 MS);
};

ulong modifyBurstInterval(ulong val, int shift, int multiplier) {
  return constrain( val + multiplier * shift, 1 MKS, 1 S);
};


ulong modifyFrequency(ulong freq, int shift, ulong multiplier) {

  if( freq == 4000000UL && shift > 0 || freq == 1UL && shift < 0) return;

  ulong currFreq = freq;
  for( ulong i = 0; i < multiplier; i++ ){

    for( int f = freq; f>0 && f <= 4000000UL ; f += shift){
      ulong newFreq = Gate::meander(f);
      if( newFreq != currFreq ) {
        currFreq = newFreq;
        break;
      }
    }

    if( currFreq == 4000000UL || currFreq == 1UL) break;
  }

  return (ulong)currFreq;
};

void Params::modify(Property property, int shift, ulong multiplier) {
  switch (property) {
    case ImpulseLength:       impulseLength       = modifyWelderInterval(impulseLength,       shift, multiplier); return;
    case SecondImpulseLength: secondImpulseLength = modifyWelderInterval(secondImpulseLength, shift, multiplier); return;
    case ThirdImpulseLength:  thirdImpulseLength  = modifyWelderInterval(thirdImpulseLength,  shift, multiplier); return;

    case ImpulseDelay:        impulseDelay        = modifyWelderInterval(impulseDelay,        shift, multiplier); return;
    case SecondImpulseDelay:  secondImpulseDelay  = modifyWelderInterval(secondImpulseDelay,  shift, multiplier); return;

    case Frequency:           frequency = modifyFrequency(frequency, shift, multiplier);return;

    case BurstImpulseLength:  burstImpulseLength = modifyBurstInterval(burstImpulseLength, shift, multiplier); return;
    case BurstImpulseDelay:   burstImpulseDelay  = modifyBurstInterval(burstImpulseDelay,  shift, multiplier); return;

    case BurstLength:   burstLength = constrain(burstLength + shift, 1, 1000); return;

    case Cooldown:      cooldown = modifyCooldownInterval(cooldown, shift, multiplier); return;

    case ContactDetectDelay:  contactDetectDelay = modifyCooldownInterval(contactDetectDelay, shift, multiplier); return;
  }
};



ulong Params::getValue(Property prop){
  switch (prop) {
    case ImpulseLength:         return Params::impulseLength/1000;
    case ImpulseDelay:          return Params::impulseLength/1000;
    case SecondImpulseLength:   return Params::secondImpulseLength/1000;
    case SecondImpulseDelay:    return Params::secondImpulseDelay/1000;
    case ThirdImpulseLength:    return Params::thirdImpulseLength/1000;
    case BurstImpulseLength:    return Params::burstImpulseLength;
    case BurstImpulseDelay:     return Params::burstImpulseDelay;
    case BurstLength:           return Params::burstLength;
    case Frequency:             return Params::frequency;
    case Cooldown:              return Params::cooldown/100000;
    case ContactDetectDelay:    return Params::contactDetectDelay/100000;
  }
}

ulong Params::getMaxMultiplier(Property prop){
  switch (prop) {
    case ImpulseLength:
    case ImpulseDelay:
    case SecondImpulseLength:
    case SecondImpulseDelay:
    case ThirdImpulseLength:    return 10;
    case BurstImpulseLength:    return Params::burstImpulseLength;
    case BurstImpulseDelay:     return Params::burstImpulseDelay;
    case BurstLength:           return Params::burstLength;
    case Frequency:             return Params::frequency;
    case Cooldown:              return Params::cooldown/100000;
    case ContactDetectDelay:    return Params::contactDetectDelay/100000;
  }
}

void Params::init() {
  load();
};