#include <ArduinoOTA.h>
#include "user_config.h"

#define OTA_INITIAL_STATE true


bool is_ota_enabled;


void setupOta() {
  is_ota_enabled = OTA_INITIAL_STATE;
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
  });
  ArduinoOTA.onEnd([]() {
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  });
  ArduinoOTA.onError([](ota_error_t error) {
  });
  ArduinoOTA.begin();
}

void loopOta() {
  if (!is_ota_enabled) {
    return;
  }
  ArduinoOTA.handle();
}
