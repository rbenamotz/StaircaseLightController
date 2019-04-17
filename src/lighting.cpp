#include "user_config.h"
#include "lighting.h"
#include "common.h"
#include <Arduino.h>

void applyChanges()
{
  static bool actualIsLightOn = !globalIsLightsOn;
  if (actualIsLightOn == globalIsLightsOn)
  {
    return;
  }
  actualIsLightOn = globalIsLightsOn;
  digitalWrite(LED_STRIP_PIN, actualIsLightOn);
  digitalWrite(RELAY_PIN, actualIsLightOn);
}

void setupLighting()
{
  pinMode(LED_STRIP_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  applyChanges();
}

void loopLighting()
{
  applyChanges();
}