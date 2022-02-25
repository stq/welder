#pragma once

#define MKS *1UL
#define MS *1000UL
#define S *1000000UL

#define MEM_START 10

enum Mode {
    OneImpulse, DualImpulse, TripleImpulse, Burst, Meander
};
#define MODES_AMOUNT 5

enum Property {
    ImpulseLength,
    ImpulseDelay,
    SecondImpulseLength,
    SecondImpulseDelay,
    ThirdImpulseLength,
    BurstImpulseLength,
    BurstImpulseDelay,
    BurstLength,
    Frequency,
    Cooldown,
    ContactDetectDelay
};
#define PROPERTIES_AMOUNT 11

class Params {

private:
  static void applyConstraints();
  static bool isPropertyApplicable(Property prop);

public:

  static ulong impulseLength;
  static ulong impulseDelay;
  static ulong secondImpulseLength;
  static ulong secondImpulseDelay;
  static ulong thirdImpulseLength;
  static ulong burstImpulseLength;
  static ulong burstImpulseDelay;
  static ulong burstLength;
  static ulong frequency;
  static ulong cooldown;
  static ulong contactDetectDelay;
  static Mode  mode;

  static void init();
  static Property getNextProperty(Property base, bool backward);
  static void modify(Property property, int shift, ulong multiplier);
  static void save();
  static void load();
  static bool isContinous();
  static ulong getValue(Property prop);
  static ulong getMaxMultiplier(Property);
};
