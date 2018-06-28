#include <Arduino.h>
#include "lighting.h"
#include "led.h"
#include "button.h"
#include "wifi.h"
#include "ota.h"
#include "mqtt.h"

void setup() {
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
  loopLighting();
  loopButton();
  delay(1);
}
