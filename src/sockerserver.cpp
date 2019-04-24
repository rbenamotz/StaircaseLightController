#include "common.h"
#include "user_config.h"
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

WebSocketsServer webSockets = WebSocketsServer(81);

volatile bool isLogDirty = false;

void newLogLineCallBack(String logLine)
{
  isLogDirty = true;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_TEXT:
    const size_t capacity = 2 * JSON_OBJECT_SIZE(2) + 5 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + 2 * JSON_OBJECT_SIZE(6) + 970;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, payload);
    const char *messageType = doc["type"];
    writeToLog("Recieved messages %s from WS", messageType);
    if (strcmp("lights_on", messageType) == 0)
    {
      globalIsLightsOn = true;
    }
    if (strcmp("lights_off", messageType) == 0)
    {
      globalIsLightsOn = false;
    }
    break;
  }
}

void setupSocketServer()
{
  onLogLine(newLogLineCallBack);
  webSockets.onEvent(webSocketEvent);
}

void loopSocketServer()
{
  static bool b = true;
  if (b && globalIsWifiConnected)
  {
    webSockets.begin();
    writeToLog("Socket server listening");
    b = !b;
  }

  static int totalWSConnectedClinets = 0;
  webSockets.loop();
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
