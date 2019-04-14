#include <Arduino.h>
#include "lighting.h"
#include "led.h"
#include "button.h"
#include "wifi.h"
#include "ota.h"
#include "mqtt.h"
#include "webserver.h"

void setup()
{
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
  loopMqtt();
  loopLed();
  loopLighting();
  loopButton();
  loopWebServer();
  delay(1);
}
