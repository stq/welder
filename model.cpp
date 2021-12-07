#include "model.h"
#include "settings.h"
#include "preset.h"

bool            Model::isPresetSettingsEditMode = false;
bool            Model::isIdle = false;
PresetProperty  Model::selectedPresetProperty = ModeSelector;


void Model::chooseNextPresetProperty() {
  Preset p = Settings::getCurrentPreset();
  Mode m = p.mode;
  PresetProperty r;
  switch (selectedPresetProperty) {

    case ModeSelector:
      switch (m) {
        case OneImpulse:
        case DualImpulse:
        case TripleImpulse:
        case Burst:
          r = ImpulseLength;
          return;
        case Meander:
          r = Frequency;
          return;
        case Linear:
          r = ChargeVoltageLimit;
          return;
      }
    case ImpulseLength:
      switch (m) {
        case OneImpulse:
          r = ChargeVoltageLimit;
          return;
        case DualImpulse:
        case TripleImpulse:
          r = ImpulseDelay;
          return;
        case Burst:
          r = BurstLength;
          return;
      }
    case ImpulseDelay:
      switch (m) {
        case DualImpulse:
        case TripleImpulse:
          r = SecondImpulseLength;
          return;
        case Burst:
          r = BurstLength;
          return;
      }
    case SecondImpulseLength:
      switch (m) {
        case DualImpulse:
          r = ChargeVoltageLimit;
          return;
        case TripleImpulse:
          r = SecondImpulseDelay;
          return;
      }
    case SecondImpulseDelay:
      r = ThirdImpulseLength;
      return;

    case ThirdImpulseLength:
    case BurstLength:
    case Frequency:
      r = ChargeVoltageLimit;
      return;

    case ChargeVoltageLimit:
      r = Cooldown;
      return;

    case Cooldown:
      r = EnableContactDetect;
      return;

    case EnableContactDetect:
      r = p.enableContactDetect ? ContactDetectDelay : ModeSelector;
      return;

    case ContactDetectDelay:
      r = ModeSelector;
      return;
  };
  selectedPresetProperty = r;
};


void Model::choosePrevPresetProperty() {
  Preset p = Settings::getCurrentPreset();
  Mode m = p.mode;
  PresetProperty r;
  switch (selectedPresetProperty) {

    case ModeSelector:
      r = p.enableContactDetect ? ContactDetectDelay : EnableContactDetect;
      return;

    case ImpulseLength:
      r = ModeSelector;
      return;

    case ImpulseDelay:
      r = ImpulseLength;
      return;

    case SecondImpulseLength:
      r = ImpulseDelay;
      return;

    case SecondImpulseDelay:
      r = SecondImpulseLength;
      return;

    case ThirdImpulseLength:
      r = SecondImpulseDelay;
      return;

    case BurstLength:
      r = ImpulseDelay;
      return;

    case Frequency:
      r = ModeSelector;
      return;

    case ChargeVoltageLimit:
      switch (m) {
        case OneImpulse:
          r = ImpulseLength;
          return;
        case DualImpulse:
          r = SecondImpulseLength;
          return;
        case TripleImpulse:
          r = ThirdImpulseLength;
          return;
        case Burst:
          r = BurstLength;
          return;
        case Meander:
          r = Frequency;
          return;
        case Linear:
          r = ModeSelector;
          return;
      }

    case Cooldown:
      r = ChargeVoltageLimit;
      return;

    case EnableContactDetect:
      r = Cooldown;
      return;

    case ContactDetectDelay:
      r = EnableContactDetect;
      return;
  };

  selectedPresetProperty = r;
};