#include <Arduino.h>
#include "global.h"
#include "pins.h"
#include "display.h"
#include "speaker.h"
#include "model.h"
#include "view.h"
#include "gate.h"

void renderPropertyName() {
    switch (Model::property) {
        case ImpulseLength:
            Display::print(0, "Length: #1");
            return;
        case ImpulseDelay:
            Display::print(0, "Pause: #1..#2");
            return;
        case SecondImpulseLength:
            Display::print(0, "Length: #2");
            return;
        case BurstImpulseLength:
            Display::print(0, "Length: On");
            return;
        case BurstImpulseDelay:
            Display::print(0, "Length: Off");
            return;
        case BurstLength:
            Display::print(0, "Repeats");
            return;
        case Frequency:
            Display::print(0, "Frequency");
            return;
        case Cooldown:
            Display::print(0, "Cooldown time");
            return;
        case ContactDetectDelay:
            Display::print(0, "Auto wait time");
            return;
    }
};

char buffer[16];
void renderAutoCountdown() {
    Display::print(0, "Contact detect");
    int decimals = (Model::remainingAutoCountdownTime % 1000) / 100;
    sprintf(buffer, "Impulse in %d.%ds", int(Model::remainingAutoCountdownTime / 1000), decimals);
    Display::print(1, buffer);
    if (decimals % 2 == 0) Speaker::play(1000, 100);
}

void renderCooldown() {
    Display::print(0, "Cooldown...");
    sprintf(buffer, "Ends in:%d.%ds", int(Model::remainingCooldownTime / 1000), int(Model::remainingCooldownTime % 1000) / 100);
    Display::print(1,  buffer);
    if (Model::remainingCooldownTime < 100) Speaker::play(2000, 100);
}

char *getPropertyMetric() {
    switch (Model::property) {
        case ImpulseLength:
        case ImpulseDelay:
        case SecondImpulseLength:
        case BurstImpulseLength:
        case BurstImpulseDelay:
        case Cooldown:
        case ContactDetectDelay:
            return "mks";
        case BurstLength:
            return "   ";
        case Frequency:
            return "Hz ";
    }
}

char modes[] = {'1', '2', '3', char(3), 'M'};

void renderGateActive() {
    Display::print(0, "Gate is active");
    Display::print(1, "");
}

void View::tick() {

    if( Gate::isActive() && Model::remainingCooldownTime <= 0) {
        Display::print(0, 0, Params::isWeldingMode() ? char(1) : char(2));
        Display::print(0, 1, modes[int(Params::mode)]);
        renderGateActive();
    } else if (Model::isAutoCountdown) {
        renderAutoCountdown();
    } else if (Model::remainingCooldownTime > 0) {
        renderCooldown();
    } else {
        Display::print(0, 0, Params::isWeldingMode() ? char(1) : char(2));
        Display::print(0, 1, modes[int(Params::mode)]);
        renderPropertyName();
        if (Model::property == BurstLength && Params::burstLength == 0) {
            Display::print(1, "till cancelled");
        } else {
            sprintf(buffer, "  %7lu %s  ", Params::getValue(Model::property), getPropertyMetric());
            Display::print(1, buffer);
            Display::blink(10 - Model::multiplierLog10, 1);
        }
    }
};
