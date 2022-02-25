#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  draw();



}

int limit = 150;

void loop() {

  int current = analogRead(A6);
  int voltage = analogRead(A7);
  int buttonUp = digitalRead(3);
  int buttonDown = digitalRead(2);

  delay(1000);
}

void draw() {
  display.clearDisplay();
  display.drawLine(60, 1, 60, 62, SSD1306_WHITE);

  display.setTextColor(SSD1306_WHITE); // Draw white text

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setCursor(0, 0);
  display.println(F("Input"));

  display.setCursor(70, 0);
  display.println(F("Limit"));

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setCursor(0, 15);

  int displayVoltage = 130;
  display.print(String(displayVoltage/10));
  display.print(F("."));
  display.print(String(displayVoltage%10));
  display.println(F("v"));

  display.setCursor(0, 35);
  display.println(F("10.0a"));

  display.setCursor(64, 15);
  display.print(String(limit/10));
  display.print(F("."));
  display.print(String(limit%10));
  display.println(F("v"));
  display.setCursor(64, 35);
  display.println(F("ON"));

  display.display();
}