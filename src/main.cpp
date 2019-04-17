#include <Arduino.h>
#include "lighting.h"
#include "led.h"
#include "button.h"
#include "wifi.h"
#include "ota.h"
#include "mqtt.h"
#include "common.h"
#include "webserver.h"

void setup()
{
  setupCommon();
  setupLed();
  setupLighting();
  setupButton();
  setupWifi();
  setupOta();
  setupMqtt();
  setupWebServer();
}

void loop()
{
  loopWifi();
  loopOta();
  loopButton();
  loopLed();
  loopLighting();
  loopMqtt();
  loopWebServer();
}
