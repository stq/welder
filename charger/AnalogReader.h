#pragma once
#include <Arduino.h>
#define LOG_SIZE 30

class AnalogReader {
private:
  int pin;
  int log[LOG_SIZE];
  int index;

public:
  AnalogReader(int _pin);
  float read();
};
