#include <Arduino.h>
#include "button.h"
#include "lighting.h"
#include "led.h"
#include "user_config.h"

#define BOUNCING_TOLLERANCE 100
volatile unsigned long last_interrupt = 0;
bool long_click_sensor_state = false;
bool need_to_change_state = false;
bool is_button_still_down = false;



void handleButtonInterrupt() {
  if (is_button_still_down) {
    return;
  }
  Serial.println("button interrupt");
  if (need_to_change_state) {
    return;
  }
  if (millis() - last_interrupt <BOUNCING_TOLLERANCE) {
    //false alaram
    return;
  }
  need_to_change_state = true;
  is_button_still_down = true;
  last_interrupt = millis();
}




void setupButton() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);
  Serial.println("Button setup");
}

void triggerLongClickSensor() {
  if (!is_button_still_down) {
    long_click_sensor_state = false;
    return;
  }
  if (long_click_sensor_state) {
    return;
  }
  // if (!isLightsOn) {
  //   return;
  // }
  unsigned long l = millis() - last_interrupt;
  if (l<1000) {
    return;
  }
  blinkLed();
  // for (int i=0; i<4; i++) {
  //   lightingToggle();
  //   delay(50);
  // }
  long_click_sensor_state = true;
}

void loopButton() {
  if (!is_button_still_down) {
    return;
  }
  is_button_still_down = !digitalRead(BUTTON_PIN);
  triggerLongClickSensor();
}
