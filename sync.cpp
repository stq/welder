#include "settings.h"
#include "view.h"
#include "controller.h"
#include "charger.h"
#include "gate.h"

#define UI_TIME_SHARE 100
#define SETTINGS_TIME_SHARE 10001
#define CONTROLLER_TIME_SHARE 10
#define CHARGE_TIME_SHARE 1000

int tickCtr = 0;

void tickAll() {

  tickCtr++;

  if ((tickCtr % SETTINGS_TIME_SHARE) == 0) Settings::tick();

  if ((tickCtr % UI_TIME_SHARE) == 0) View::tick();

  if ((tickCtr % CONTROLLER_TIME_SHARE) == 0) Controller::tick();

  if ((tickCtr % CHARGE_TIME_SHARE) == 0) Charge::tick();

  if (Gate::isActive()) Gate::tick();
};
