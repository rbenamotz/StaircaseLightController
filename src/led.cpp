#include "user_config.h"
#include <Arduino.h>
#include "led.h"
#include "common.h"

void apply(bool force = false)
{
  static bool ledState = !globalIsLightsOn;
  if (!force && ledState != globalIsLightsOn)
  {
    return;
  }
  ledState = !globalIsLightsOn;
  digitalWrite(LED_PIN, ledState);
}

void setupLed()
{
  pinMode(LED_PIN, OUTPUT);
  apply(true);
}

void loopLed()
{
  apply();
}

void blinkLed()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(LED_PIN, LOW);
    delay(50);
    digitalWrite(LED_PIN, HIGH);
    delay(50);
  }
  apply(true);
}
