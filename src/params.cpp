#include <Arduino.h>
#include "global.h"
#include "model.h"
#include "params.h"
#include "storage.h"
#include "gate.h"

long Params::impulseLength;
long Params::impulseDelay;
long Params::secondImpulseLength;
long Params::secondImpulseDelay;
long Params::thirdImpulseLength;
long Params::burstImpulseLength;
long Params::burstImpulseDelay;
long Params::burstLength;
long Params::frequency;
long Params::cooldown;
long Params::contactDetectDelay;
Mode  Params::mode = OneImpulse;

bool Params::isPropertyApplicable(Property prop) {
    Mode mode = Params::mode;

    if (mode == Meander && prop == Frequency) return true;
    if (mode == Burst && (prop == BurstImpulseLength || prop == BurstImpulseDelay || prop == BurstLength)) return true;

    if ((mode == TripleImpulse || mode == DualImpulse || mode == OneImpulse) && (prop == ImpulseLength || prop == ContactDetectDelay || prop == Cooldown)) return true;
    if ((mode == TripleImpulse || mode == DualImpulse) && (prop == ImpulseDelay || prop == SecondImpulseLength)) return true;
    if (mode == TripleImpulse && (prop == SecondImpulseDelay || prop == ThirdImpulseLength)) return true;

    return false;
};

void Params::applyConstraints() {
    modify(ImpulseLength, 0, 1);
    modify(ImpulseDelay, 0, 1);
    modify(SecondImpulseLength, 0, 1);
    modify(SecondImpulseDelay, 0, 1);
    modify(ThirdImpulseLength, 0, 1);
    modify(BurstImpulseLength, 0, 1);
    modify(BurstImpulseDelay, 0, 1);
    modify(BurstLength, 0, 1);
    modify(Frequency, 0, 1);
    modify(Cooldown, 0, 1);
    modify(ContactDetectDelay, 0, 1);
    if ((int) mode < 0 || (int) mode >= MODES_AMOUNT) mode = OneImpulse;
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
    long mode_int;
    Storage::read(mode_int);
    mode = static_cast<Mode>((int) mode_int);
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
    Storage::write((long) mode);
};

bool Params::isContinous() {
    return mode == Meander;
};

Property Params::getNextProperty(Property base, bool backward) {
    int i = (int) base;
    while (true) {
        i = (i + PROPERTIES_AMOUNT + (backward ? -1 : 1)) % PROPERTIES_AMOUNT;
        Property nextProperty = static_cast<Property> (i);
        if (isPropertyApplicable(nextProperty)) return nextProperty;
    }
};

long modifyInterval(long val, int shift, long multiplier, long minVal, long maxVal) {
    return constrain(val + multiplier * shift, minVal, maxVal);
};

long modifyFrequency(long freq, int shift, long multiplier) {

    if (freq == 4000000L && shift > 0 || freq == 1L && shift < 0) return;

    long requestedFreq = freq + shift * multiplier;

    if( requestedFreq > 4000000L ) requestedFreq = 4000000L;
    if( requestedFreq < 1L ) requestedFreq = 1L;

    long newFreq = Gate::meander(requestedFreq, true);

    return  newFreq;
};


long pow10(int n){
    switch(n) {
        case 0: return 1L;
        case 1: return 10L;
        case 2: return 100L;
        case 3: return 1000L;
        case 4: return 10000L;
        case 5: return 100000L;
        case 6: return 1000000L;
    }
}

void Params::modify(Property property, int shift, int multiplierLog10) {
    long multiplier = pow10(multiplierLog10-1);
    long minVal =  1 MKS;
    long maxImpulseVal =  Model::fuse ? 30 MS : 9 S;
    long maxDelayVal =  Model::fuse ? 200 MS : 9 S;
    long minCdDelayVal =  Model::fuse ? 1 S : 1 MS;
    switch (property) {
        case ImpulseLength:
            impulseLength = modifyInterval(impulseLength, shift, multiplier, minVal, maxImpulseVal);
            return;
        case SecondImpulseLength:
            secondImpulseLength = modifyInterval(secondImpulseLength, shift, multiplier, minVal, maxImpulseVal);
            return;
        case ThirdImpulseLength:
            thirdImpulseLength = modifyInterval(thirdImpulseLength, shift, multiplier, minVal, maxImpulseVal);
            return;

        case ImpulseDelay:
            impulseDelay = modifyInterval(impulseDelay, shift, multiplier, minVal, maxDelayVal);
            return;
        case SecondImpulseDelay:
            secondImpulseDelay = modifyInterval(secondImpulseDelay, shift, multiplier, minVal, maxDelayVal);
            return;

        case Frequency:
            frequency = modifyFrequency(frequency, shift, multiplier);
            return;

        case BurstImpulseLength:
            burstImpulseLength = modifyInterval(burstImpulseLength, shift, multiplier, minVal, maxImpulseVal);
            return;
        case BurstImpulseDelay:
            burstImpulseDelay = modifyInterval(burstImpulseDelay, shift, multiplier, minVal, maxDelayVal);
            return;

        case BurstLength:
            burstLength = constrain(burstLength + multiplier * shift, 0, 9999999);
            return;

        case Cooldown:
            cooldown = modifyInterval(cooldown, shift, multiplier, minCdDelayVal, 9 S);
            return;

        case ContactDetectDelay:
            contactDetectDelay = modifyInterval(contactDetectDelay, shift, multiplier, minCdDelayVal, 9 S);
            return;
    }
};


long Params::getValue(Property prop) {
    switch (prop) {
        case ImpulseLength:
            return Params::impulseLength;
        case ImpulseDelay:
            return Params::impulseLength;
        case SecondImpulseLength:
            return Params::secondImpulseLength;
        case SecondImpulseDelay:
            return Params::secondImpulseDelay;
        case ThirdImpulseLength:
            return Params::thirdImpulseLength;
        case BurstImpulseLength:
            return Params::burstImpulseLength;
        case BurstImpulseDelay:
            return Params::burstImpulseDelay;
        case BurstLength:
            return Params::burstLength;
        case Frequency:
            return Params::frequency;
        case Cooldown:
            return Params::cooldown;
        case ContactDetectDelay:
            return Params::contactDetectDelay;
    }
};

bool Params::isWeldingMode() {
    return mode == OneImpulse || mode == DualImpulse || mode == TripleImpulse;
}
