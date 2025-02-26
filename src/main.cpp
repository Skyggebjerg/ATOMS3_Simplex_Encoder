#include <Arduino.h>
#include "M5AtomS3.h"
#include <M5GFX.h>

M5GFX display;
M5Canvas canvas(&display);

const int encoderAPin = 5;     // GPIO 5: Encoder A (SCM010 Pin 5 Brown)
const int encoderBPin = 6;     // GPIO 6: Encoder B (SCM010 Pin 6 Yellow)

volatile long encoderCount = 0;
unsigned long lastTime = 0;
int rpm = 0;

void updateEncoder() {
  if (digitalRead(encoderBPin) == HIGH) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}

void setup() {

  pinMode(encoderAPin, INPUT_PULLUP);
  pinMode(encoderBPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderAPin), updateEncoder, RISING);

  auto cfg = M5.config();
  AtomS3.begin(cfg);
  AtomS3.Display.setTextColor(WHITE);
  AtomS3.Display.setTextSize(3);
  AtomS3.Display.clear();
}

void loop() {

    // Calculate RPM every second
    if (millis() - lastTime >= 1000) {
      AtomS3.Display.clear();
      rpm = (encoderCount * 60) / 1024; // Assuming encoder PPR
      encoderCount = 0;
      lastTime = millis();
 
      AtomS3.Display.drawString(String(rpm), 10, 30);
      //Serial.print("RPM: ");
      //Serial.println(rpm);
    }

}