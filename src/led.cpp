#include "user_config.h"
#include <Arduino.h>
#include "led.h"
#include "lighting.h"

bool isLedOn = false;



void apply() {
  if (isLightsOn == isLedOn) {
    return;
  }
  digitalWrite(LED_PIN,!isLightsOn);
  isLedOn = isLightsOn;
}

void setupLed() {
  pinMode(LED_PIN, OUTPUT);
}



void loopLed() {
  apply();
}

void blinkLed() {
  for (int i=0; i<5; i++) {
    digitalWrite(LED_PIN,LOW);
    delay(50);
    digitalWrite(LED_PIN,HIGH);
    delay(50);
  }
  apply();
}
