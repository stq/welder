#include "AnalogReader.h"

AnalogReader::AnalogReader(int _pin) {
  pin = _pin;
  index = 0;
}

float AnalogReader::read() {
  index = (index + 1) % LOG_SIZE;
  log[index] = analogRead(pin);

  float avg = 0;

  for (int i = 0; i < LOG_SIZE; i++) avg += (float) log[i];

  return round(avg / (float) LOG_SIZE);
}
