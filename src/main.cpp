#include <Arduino.h>
#include "lighting.h"
#include "led.h"
#include "button.h"
#include "wifi.h"
#include "ota.h"
#include "mqtt.h"
#include "common.h"
#include "webserver.h"

void (*loopWorkItem)();

void executeLoopWork(const char* operation, void (*loopWorkItem)())
{
  unsigned long l = millis();
  loopWorkItem();
  l = millis() - l;
  if (l>2) {
    writeToLog("Looping %s took %lu ms to complete",operation,l);
  }
}

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
  executeLoopWork("WiFi",loopWifi);
  executeLoopWork("OTA",loopOta);
  executeLoopWork("Button", loopButton);
  executeLoopWork("Led",loopLed);
  executeLoopWork("Lighting",loopLighting);
  executeLoopWork("MQTT",loopMqtt);
  executeLoopWork("WebServer",loopWebServer);
}
