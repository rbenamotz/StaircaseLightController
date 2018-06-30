#include "user_config.h"
#include "lighting.h"
#include <Arduino.h>
#define LIGHT_ON_OFF_STEP 20

bool isLightsOn = true;

void applyChanges() {
  if (isLightsOn) {
    for (int i=0; i<1024; i+=LIGHT_ON_OFF_STEP) {
      analogWrite(LED_STRIP_PIN,i);
      delay(1);
    }
  } else {
    for (int i=1023; i>=0; i-=LIGHT_ON_OFF_STEP) {
      analogWrite(LED_STRIP_PIN,i);
      delay(1);
    }

  }
  digitalWrite(LED_STRIP_PIN,isLightsOn);
  digitalWrite(RELAY_PIN,isLightsOn);
}

void setupLighting() {
  pinMode(LED_STRIP_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  applyChanges();
}

void loopLighting() {
}


void lightingOn() {
  if (isLightsOn) {
    return;
  }
  isLightsOn = true;
  applyChanges();
}
void lightingOff() {
  if (!isLightsOn) {
    return;
  }
  isLightsOn = false;
  applyChanges();
}

void lightingToggle() {
  isLightsOn = !isLightsOn;
  applyChanges();
}
