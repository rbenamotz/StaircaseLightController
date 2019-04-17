#include "webserver.h"
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "common.h"
#include "user_config.h"
#include <ArduinoJson.h>
#include <Hash.h>

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
WebSocketsServer webSockets = WebSocketsServer(81);
StaticJsonDocument<500> doc;
volatile bool isLogDirty = false;

const char rootPage[] PROGMEM = {ROOT_PAGE};

void handleRoot()
{
  server.send_P(200, "text/html", rootPage);
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
  server.send(200, "plain/html", "restarting");
  ESP.restart();
}

void writeLogToSockets(String logLine)
{
  isLogDirty = true;
  // webSockets.broadcastTXT(logLine + "\n");
  // String s = readLogBuffer();
  // webSockets.broadcastTXT(s);
}

void setupWebServer()
{
  server.on("/", handleRoot);
  server.on("/log", handleLog);
  server.on("/rst", handleRestart);
  httpUpdater.setup(&server);
}

void loopWebServer()
{
  static bool b = true;
  if (b && globalIsWifiConnected)
  {
    server.begin();
    webSockets.begin();
    onLogLine(writeLogToSockets);
    writeToLog("Web server listening");
    b = !b;
  }

  static int totalWSConnectedClinets = 0;
  webSockets.loop();
  unsigned long start = millis();
  server.handleClient();
  unsigned long l = millis() - start;
  if (l>5) {
    writeToLog("handleClient took %lums",l);
  }
  int p = webSockets.connectedClients();
  if (p != totalWSConnectedClinets)
  {
    isLogDirty = true;
    totalWSConnectedClinets = p;
  }
  if (isLogDirty)
  {
    String s = readLogBuffer();
    webSockets.broadcastTXT(s);
    isLogDirty = false;
  }
}
