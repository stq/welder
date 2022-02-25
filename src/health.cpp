#include <Arduino.h>
#include "pins.h"
#include "health.h"

void Health::check(){
  Serial.begin(9600);
  Serial.println("Starting health. Press any key to skip test..");

  while(Serial.available() == 0) {
    delay(1000);
    Serial.println("Waiting...");
  }
  Serial.read();
  Serial.println("Test contact detector");
  while(Serial.available() == 0) {
    Serial.print("Contact sensor value: ");
    Serial.println(analogRead(PIN_CONTACT_SENSOR));
    delay(1000);
  }
  Serial.read();
  Serial.println("Test speaker");
  pinMode(PIN_SPEAKER, OUTPUT);
  int f = 0;
  while(Serial.available() == 0) {
    f ++;
    f = f % 10000;
    tone(PIN_SPEAKER, 100 + f, 10);
    delay(10);
  }
  Serial.read();
  Serial.println("Gate Test");
  pinMode(PIN_GATE, OUTPUT);
  while(Serial.available() == 0) {
    digitalWrite(PIN_GATE, HIGH);
    Serial.println("Gate is ON");
    delay(1000);
    digitalWrite(PIN_GATE, LOW);
    Serial.println("Gate is OFF");
    delay(1000);
  }
  Serial.read();
  Serial.println("Test Encoder pins: A, B");
  while(Serial.available() == 0) {
    Serial.println(digitalRead(PIN_AUX_ENCODER_A));
    Serial.println(digitalRead(PIN_AUX_ENCODER_B));
    delay(500);
  }
  Serial.read();

  Serial.println("Test impulse button");
  while(Serial.available() == 0) {
    Serial.println(digitalRead(PIN_IMPULSE_BTN));
    delay(500);
  }
  Serial.read();
  Serial.println("Test Left button");
  while(Serial.available() == 0) {
    Serial.println(digitalRead(PIN_BTN_LEFT));
    delay(500);
  }
  Serial.read();

  Serial.println("Test Right button");
  while(Serial.available() == 0) {
    Serial.println(digitalRead(PIN_BTN_RIGHT));
    delay(500);
  }
  Serial.read();

  Serial.println("Test Alt button");
  while(Serial.available() == 0) {
    Serial.println(analogRead(PIN_BTN_ALT));
    delay(500);
  }
  Serial.read();

  Serial.println("Test Save/Load button");
  while(Serial.available() == 0) {
    Serial.println(analogRead(PIN_BTN_SAVE));
    delay(500);
  }
  Serial.read();


}

