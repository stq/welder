#include "display.h"
#include "pins.h"

#include "settings.h"
#include "view.h"
#include "gate.h"
#include "charger.h"

Preset p;


char View::line1[16];
char View::line2[16];
byte View::cursorPosX;
byte View::cursorPosY;

void View::init() {
  tick();
};

void View::renderPresetPropertyValue() {
  p = Settings::getCurrentPreset();
  switch (Model::selectedPresetProperty) {
    case ModeSelector:
      sprintf(line2, "Mode:%s", getModeString());
      return;
    case ImpulseLength:
      sprintf(line2, "Impulse1:%03dms", p.impulseLength);
      return;
    case ImpulseDelay:
      sprintf(line2, "Delay1-2:%03dms", p.impulseDelay);
      return;
    case SecondImpulseLength:
      sprintf(line2, "Impulse2:%03dms", p.secondImpulseLength);
      return;
    case SecondImpulseDelay:
      sprintf(line2, "Delay2-3:%03dms", p.secondImpulseDelay);
      return;
    case ThirdImpulseLength:
      sprintf(line2, "Impulse3:%03dms", p.thirdImpulseLength);
      return;
    case BurstLength:
      sprintf(line2, "Amount:%03dtimes", p.burstLength);
      return;
    case Frequency:
      sprintf(line2, "Freq:%08Hz", p.frequency);
      return;
    case ChargeVoltageLimit:
      sprintf(line2, "MaxCharge:%02d.%02dv", p.chargeVoltageLimit / 100, p.chargeVoltageLimit % 100);
      return;
    case Cooldown:
      sprintf(line2, "Cooldown:%06dms", p.cooldown);
      return;
    case EnableContactDetect:
      sprintf(line2, "Auto Impulse:%s", p.enableContactDetect ? "Yes" : "No");
      return;
    case ContactDetectDelay:
      sprintf(line2, "AutoDelay:%06dms", p.contactDetectDelay);
      return;
  }
};

char View::getModeChar() {
  p = Settings::getCurrentPreset();
  switch (p.mode) {
    case OneImpulse:
      return CHAR_MODE_1IMP;
    case DualImpulse:
      return CHAR_MODE_2IMP;
    case TripleImpulse:
      return CHAR_MODE_3IMP;
    case Burst:
      return CHAR_MODE_BURST;
    case Meander:
      return CHAR_MODE_MEANDER;
    case Linear:
      return 'C';
  }
};

char *View::getModeString() {
  p = Settings::getCurrentPreset();
  switch (p.mode) {
    case OneImpulse:
      return "1 x Impulse";
    case DualImpulse:
      return "2 x Impulse";
    case TripleImpulse:
      return "3 x Impulse";
    case Burst:
      return "Burst";
    case Meander:
      return "Meander";
    case Linear:
      return "Linear";
  }
};

void char *renderVC(int vc) {
  char buff[5];
  sprintf(buff, "%d02.%d02", vc / 100, vc % 100);
  return buff;
}

void View::renderBottomLine() {
  if (isDisplayStatusMode) {
    if (!Gate::isActive()) {
      if (Charge::isCharged) sprintf(line2, "%s [CHARGED]", renderVC(Charge::voltage));
      else sprintf(line2, "%sv In:sa", renderVC(Charge::voltage), renderVC(Charge::chargeCurrent));
    } else if (p.isContinous()) {
      sprintf(line2, "On!Push to stop");
    } else {
      sprintf(line2, "Impulse!Wait..");
    }
  } else {
    renderPresetPropertyValue();
  }
};


void char *renderInterval(unsigned long interval) {
  char ibff[4];
  if (interval < 1000) {
    sprintf(ibff, "%dU", interval);
  } else if (interval < 10000) {
    sprintf(ibff, "%d.%dM", interval / 1000, (interval / 100) % 10);
  } else if (interval < 1000000) {
    sprintf(ibff, "%dM", interval / 1000);
  } else if (interval < 10000000) {
    sprintf(ibff, "%d.%dS", interval / 1000000, (interval / 100000) % 10);
  } else if (interval < 1000000000) {
    sprintf(ibff, "%dS", interval / 1000000);
  }
  return ibff;
}

char tplr[13];

void View::renderTopLine() {
  p = Settings::getCurrentPreset();

  switch (p.mode) {
    case OneImpulse:
      sprintf(tplr, "1)%s", renderInterval(p.impulseLength));
      return;
    case DualImpulse:
      sprintf(tplr, "1)%s 2)%s", renderInterval(p.impulseLength), renderInterval(p.secondImpulseLength));
      return;
    case TripleImpulse:
      sprintf(tplr, "%s%s%s", renderInterval(p.impulseLength), renderInterval(p.secondImpulseLength),
              renderInterval(p.thirdImpulseLength));
      return;
    case Burst:
      sprintf(tplr, "%s%s x%d ", renderInterval(p.impulseLength), renderInterval(p.impulseDelay), p.burstLength);
      return;
    case Meander:
      sprintf(tplr, "%010dHz", p.frequency);
      return;
    case Linear:
      sprintf(tplr, "%sv", renderVC(p.chargeVoltageLimit));
      return;
  }

  sprintf(line1, "%d:%c %s", Settings::getCurrentPresetIndex(), getModeChar(), splr);
};

void View::tick() {
  renderBottomLine();
  renderTopLine();
  out_printLines(line1, line2);
  if (Model::isPresetSettingsEditMode) {
    out_focus(0, 0);
  } else {
    out_focus(0, 1);
  }
};
