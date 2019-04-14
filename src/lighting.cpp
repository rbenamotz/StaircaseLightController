#include "user_config.h"
#include "lighting.h"
#include <Arduino.h>

bool isLightsOn = true;

void applyChanges()
{
  digitalWrite(LED_STRIP_PIN, isLightsOn);
  digitalWrite(RELAY_PIN, isLightsOn);
}

void setupLighting()
{
  pinMode(LED_STRIP_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  applyChanges();
}

void loopLighting()
{
}

void lightingOn()
{
  if (isLightsOn)
  {
    return;
  }
  isLightsOn = true;
  applyChanges();
}

void lightingOff()
{
  if (!isLightsOn)
  {
    return;
  }
  isLightsOn = false;
  applyChanges();
}

void lightingToggle()
{
  isLightsOn = !isLightsOn;
  applyChanges();
}
