#include "webserver.h"
#include <ESP8266WebServer.h>
#include "common.h"
#include "user_config.h"
#include <ArduinoJson.h>

ESP8266WebServer server(80);
StaticJsonDocument<500> doc;

void handleRoot()
{
  server.send(200, "text/html", ROOT_PAGE);
}

void buildJsonInt(char *output, String name, int value)
{
  char temp[100];
  sprintf(temp, ", \"%s\" : %d", name.c_str(), value);
  strcat(output, temp);
}

void handleData()
{
  server.send(200, "application/json", "{}");
}

void handleLog()
{
  server.send(200, "plain/text", readLogBuffer());
}

void handleRestart()
{
  writeToLog("Restarting due to API request");
  ESP.restart();
}

void setupWebServer()
{
  server.on("/", handleRoot);
  server.on("/log", handleLog);
  server.on("/rst", handleRestart);
  server.begin();
  writeToLog("Web server listening");
}

void loopWebServer()
{
  server.handleClient();
}
