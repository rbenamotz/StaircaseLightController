#include <Arduino.h>
#include "lighting.h"
#include "led.h"
#include "button.h"
#include "wifi.h"
#include "ota.h"
#include "mqtt.h"

void setup() {
  Serial.begin(115200);
  setupLed();
  setupLighting();
  setupButton();
  setupWifi();
  setupOta();
  setupMqtt();
}

void loop() {
  loopWifi();
  loopOta();
  loopMqtt();
  loopLed();
  // loopStrip();
  loopButton();
  if (need_to_change_state) {
    lightingToggle();
    need_to_change_state = false;
  }
  delay(5);
}
