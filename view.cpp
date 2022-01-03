#include <string.h>
#include "global.h"

#include "pins.h"
#include "model.h"
#include "view.h"
#include "gate.h"
#include "charger.h"

char line1[26];
char line2[26];

char* getModeString() {
  switch (Model::preset->mode) {
  case OneImpulse:      return "1 x Impulse";
  case DualImpulse:     return "2 x Impulse";
  case TripleImpulse:   return "3 x Impulse";
  case Burst:           return "Burst";
  case Meander:         return "Meander";
  case Linear:          return "Linear";
  }
};

char* renderVC(char* buff, int val) {
  sprintf(buff, "%2d.%1d", val/10, val%10);
  return buff;
};


char* renderInterval(char* buff, ulong interval) {
  if (interval < 1000) {
    sprintf(buff, "%dU", interval);
  } else if (interval < 10000) {
    sprintf(buff, "%d", interval / 1000);
    sprintf(buff+sizeof(char), ".%dM", (interval % 1000) / 100);
  } else if (interval < 1000000) {
    sprintf(buff, "%dM", interval / 1000);
  } else if (interval < 10000000) {
    sprintf(buff, "%d", interval / 1000000);
    sprintf(buff+sizeof(char), ".%dS", (interval % 1000000) / 100000);
  } else if (interval < 1000000000) {
    sprintf(buff, "%dS", interval / 1000000);
  }
  return buff;
}

void renderPropertyValue() {
  Preset* p = Model::preset;
  char b[5];
  switch (Model::property) {
    case ModeSelector: sprintf(line2, "Mode:%s", getModeString()); return;
    case ImpulseLength:sprintf(line2, "Impulse1: %s", renderInterval(b, p->impulseLength));return;
    case ImpulseDelay:sprintf(line2, "Delay1: %s", renderInterval(b, p->impulseDelay));return;
    case SecondImpulseLength:sprintf(line2, "Impulse2: %s", renderInterval(b, p->secondImpulseLength));return;
    case SecondImpulseDelay:sprintf(line2, "Delay2: %s", renderInterval(b, p->secondImpulseDelay));return;
    case ThirdImpulseLength:sprintf(line2, "Impulse3: %s", renderInterval(b, p->thirdImpulseLength));return;
    case BurstLength:sprintf(line2, "Amount:%03d", p->burstLength);return;
    case Frequency:sprintf(line2, "Freq:%07luHz", p->frequency);return;
    case ChargeVoltageLimit:sprintf(line2, "MaxCharge:%sv", renderVC(b, p->chargeVoltageLimit));return;
    case Cooldown:sprintf(line2, "Cooldown: %s", renderInterval(b, p->cooldown));return;
    case EnableContactDetect:sprintf(line2, "Auto Impulse: %s", p->enableContactDetect ? "Yes" : "No");return;
    case ContactDetectDelay:sprintf(line2, "AutoDelay: %s", renderInterval(b, p->contactDetectDelay));return;
  }
};

char getModeChar() {
  switch (Model::preset->mode) {
    case OneImpulse: return CHAR_MODE_1IMP;
    case DualImpulse: return CHAR_MODE_2IMP;
    case TripleImpulse: return CHAR_MODE_3IMP;
    case Burst: return CHAR_MODE_BURST;
    case Meander: return CHAR_MODE_MEANDER;
    case Linear: return 'L';
  }
};




void renderBottomLine() {
  char b1[10];
  char b2[10];
  if (!Model::isIdle) {
    renderPropertyValue();
  } else { 
    if (Gate::isActive()) {
      if (Model::preset->isContinous()) sprintf(line2, "[Active]");
      else sprintf(line2, "Impulse!Wait..");
    } else {
      if (!Model::isCharging) sprintf(line2, "%s [CHARGED]", renderVC(b1, Model::voltage));
      else sprintf(line2, "%sv In:sa", renderVC(b1, Model::voltage), renderVC(b2, Model::current));
    }
  }
};


void renderTopLine() {
  char b1[10];
  char b2[10];
  char b3[10];
  char tplr[20];
  Preset* p = Model::preset;
  switch (p->mode) {
    case OneImpulse:    sprintf(tplr, "%s",         renderInterval(b1, p->impulseLength));break;
    case DualImpulse:   sprintf(tplr, "%s %s", renderInterval(b1, p->impulseLength), renderInterval(b2, p->secondImpulseLength));break;
    case TripleImpulse: sprintf(tplr, "%s%s%s", renderInterval(b1, p->impulseLength), renderInterval(b2, p->secondImpulseLength), renderInterval(b3, p->thirdImpulseLength));break;
    case Burst:         sprintf(tplr, "%s%s x%d ", renderInterval(b1, p->impulseLength), renderInterval(b2, p->impulseDelay), p->burstLength);break;
    case Meander:       sprintf(tplr, "%07luHz", p->frequency);break;
    case Linear:        sprintf(tplr, "%sv", renderVC(b1,p->chargeVoltageLimit));break;
  }
  sprintf(line1, "#%d %c %s", Model::presetIndex, getModeChar(), tplr);

};


void View::tick() {
  renderTopLine();
  renderBottomLine();

  Display::printLines(line1, line2);

  if (Model::isPropertyMode) Display::focus(0, 1);
  else Display::focus(0, 0);
};
