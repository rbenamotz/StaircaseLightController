#include "user_config.h"
#include <Arduino.h>
#include "led.h"
#include "lighting.h"

bool last_state = false;


void apply() {
  digitalWrite(LED_PIN,!isLightsOn);
  last_state = isLightsOn;
}

void setupLed() {
  pinMode(LED_PIN, OUTPUT);
}



void loopLed() {
  if (isLightsOn != last_state) {
    apply();
  }
}

void blinkLed() {
  for (int i=0; i<3; i++) {
    digitalWrite(LED_PIN,LOW);
    delay(20);
    digitalWrite(LED_PIN,HIGH);
    delay(20);
  }
  apply();
}
