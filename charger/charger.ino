#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AnalogReader.h"
#include "ChargeRelayController.h"
#include "storage.h"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
AnalogReader currentReader(A6);
AnalogReader voltageReader(A7);
ChargeRelayController chargeRelay;


void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {Serial.println("SSD1306 allocation failed");for(;;);}

  int limit;
  Storage::read(limit);
  limit = constrain(limit, 1, 150);
  chargeRelay.setLimit(limit);
}

int current = 0;
int loopCount = 0;

void loop() {

  int limit = chargeRelay.getLimit();
  if( digitalRead(2) ) limit++;
  if( digitalRead(3) ) limit--;
  limit = constrain(limit, 1, 150);
  chargeRelay.setLimit(limit);

  loopCount = (loopCount + 1) % 200;
  if( loopCount == 0 ) {
    Storage::write(limit);
  }

  chargeRelay.setVoltage(round(voltageReader.read()*0.2497F));
  chargeRelay.update();

int currVal = currentReader.read();
  current = round(float(constrain(currVal - 512, 0, 120) * 0.37974F)); //591
  if( !chargeRelay.isCharging() ) current = 0;
    Serial.println(currVal - 512);
    Serial.println(constrain(currVal - 512, 0, 120) * 0.37974F);

  draw();

  delay(50);
}

void draw() {
  display.clearDisplay();
  display.drawLine(60, 1, 60, 45, SSD1306_WHITE);
  display.drawLine(0, 52, 127, 52, SSD1306_WHITE);

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Input");

  display.setCursor(70, 0);
  display.println("Limit");

  display.setTextSize(2);
  display.setCursor(0, 15);

  display.print(String(chargeRelay.getVoltage()/10));
  display.print(".");
  display.print(String(chargeRelay.getVoltage()%10));
  display.println("v");

  display.setCursor(0, 35);
  display.print(String(current/10));
  display.print(".");
  display.print(String(current%10));
  display.println("a");

  display.setCursor(64, 15);
  display.print(String(chargeRelay.getLimit()/10));
  display.print(".");
  display.print(String(chargeRelay.getLimit()%10));
  display.println(F("v"));
  display.setCursor(64, 35);
  display.println(chargeRelay.isCharging() ? "ON" : "OFF" );

  display.setTextSize(1);
  display.setCursor(2, 55);
  if( chargeRelay.isChargePending() ) {
    display.println("pending");
    display.setCursor(64, 55);
    display.print(String(chargeRelay.getTimeBeforeCharge()));
    display.println("s");
  } else {
    display.println("stable");
  }


  display.display();
}

