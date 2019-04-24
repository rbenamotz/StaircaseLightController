#include "webserver.h"
// #include <ESP8266HTTPUpdateServer.h>
#include "common.h"
#include "user_config.h"
#include <ArduinoJson.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
// ESP8266HTTPUpdateServer httpUpdater;
StaticJsonDocument<500> doc;

const char rootPage[] PROGMEM = {ROOT_PAGE};

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void handleRoot(AsyncWebServerRequest *request)
{
  request->send_P(200, "text/html", rootPage);
}

void handleLog(AsyncWebServerRequest *request)
{
  request->send(200, "plain/text", readLogBuffer());
}

void setupWebServer()
{
  server.on("/", HTTP_GET, handleRoot);
  server.on("/log", HTTP_GET, handleLog);
  server.on("/api/rst", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Restarting");
    ESP.restart();
  });
  server.onNotFound(notFound);
  // httpUpdater.setup(&server);
}

void loopWebServer()
{
  static bool b = true;
  if (b && globalIsWifiConnected)
  {
    server.begin();
    writeToLog("Web server listening");
    b = !b;
  }
}
