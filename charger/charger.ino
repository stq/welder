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
    //SSD1306 display initialization fail handler, for example not enough memory
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { for (;;); }

    int limit;
    Storage::read(limit);
    limit = constrain(limit, 1, 150);
    chargeRelay.setLimit(limit);
}

float polynom(double x, double coefs[], int n) {
    double result = coefs[0];

    for (int i = 1; i < n; i++) {
        double m = coefs[i] * x;
        for (int j = 1; j < i; j++) {
            m = m * x;
        }
        result += m;
    }

    return (float) result;
}

double dCoefs[] = {
    2.7547296886729185e1,
    2.8170254390869892e-1,
    -7.3601814488326277e-3,
    8.6834934413509122e-5,
    -5.4413842952499501e-7,
    1.9295945071995852e-9,
    -3.8892818872383331e-12,
    4.1551271392288520e-15,
    -1.8273629187776181e-18
};
double vCoefs[] = {
    3.0107260077227869e0,
    -1.9105429881738145e-1,
    5.2056152232194224e-3,
    -6.7619019633917313e-5,
    5.2989544848105892e-7,
    -2.6448227351266528e-9,
    8.5914943478029932e-12,
    -1.8073695723421046e-14,
    2.3720416849754977e-17,
    -1.7637858283739181e-20,
    5.6701628949511019e-24
};


int current = 0;
int loopCount = 0;

void loop() {

    int limit = chargeRelay.getLimit();
    if (digitalRead(2)) limit++;
    if (digitalRead(3)) limit--;
    limit = constrain(limit, 1, 150);
    chargeRelay.setLimit(limit);

    loopCount = (loopCount + 1) % 200;
    if (loopCount == 0) {
        Storage::write(limit);
    }

    float voltageSensorValue = voltageReader.read();
    if (!chargeRelay.isCharging()) {
        float mod = polynom((double)voltageSensorValue, dCoefs, 9);
        voltageSensorValue += mod;
    }

    float voltage = polynom((double)voltageSensorValue, vCoefs, 11);

    chargeRelay.setVoltage(round(10.0 * voltage));
    chargeRelay.update();

    int currVal = currentReader.read();
    current = round(float(constrain(currVal - 512, 0, 120) * 0.37974F));
    if (!chargeRelay.isCharging()) current = 0;

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

    display.print(chargeRelay.getVoltage() / 10);
    display.print(".");
    display.print(chargeRelay.getVoltage() % 10);
    display.println("v");

    display.setCursor(0, 35);
    display.print(current / 10);
    display.print(".");
    display.print(current % 10);
    display.println("a");

    display.setCursor(64, 15);
    display.print(chargeRelay.getLimit() / 10);
    display.print(".");
    display.print(chargeRelay.getLimit() % 10);
    display.println("v");
    display.setCursor(64, 35);
    display.println(chargeRelay.isCharging() ? "ON" : "OFF");

    display.setTextSize(1);
    display.setCursor(2, 55);
    if (chargeRelay.isChargePending()) {
        display.println("pending");
        display.setCursor(64, 55);
        display.print(chargeRelay.getTimeBeforeCharge());
        display.println("s");
    } else {
        display.println("stable");
    }


    display.display();
}

