#include <Arduino.h>
#include "button.h"
#include "lighting.h"
#include "led.h"
#include "common.h"
#include "user_config.h"

bool buttonDown = false;
unsigned long buttonDownSince = 0;

void setupButton()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loopButton()
{
  static unsigned long last_button_status_check = 0;
  unsigned long l = millis() - last_button_status_check;
  static bool longClickInitated = false;
  if (l < 10)
  {
    return;
  }
  bool state = !digitalRead(BUTTON_PIN);
  last_button_status_check = millis();
  //first click
  if (state && !buttonDown)
  {
    writeToLog("Button down - toggling lights");
    globalIsLightsOn = !globalIsLightsOn;
    buttonDown = true;
    buttonDownSince = millis();
    return;
  }

  //long click?
  l = millis() - buttonDownSince;
  if (state && buttonDown &&  l >= 600 && !longClickInitated)
  {
    writeToLog("Button long click");

    blinkLed();
    globalShouldPublishLongClick = true;
    longClickInitated = true;
    return;
  }

  //button up
  if (!state && buttonDown)
  {
    writeToLog("Button up");
    buttonDown = false;
    longClickInitated = false;
  }
}
