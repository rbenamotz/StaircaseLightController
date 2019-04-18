#include "common.h"
#include "user_config.h"
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>

WebSocketsServer webSockets = WebSocketsServer(81);

volatile bool isLogDirty = false;

void newLogLineCallBack(String logLine)
{
  isLogDirty = true;
}

void setupSocketServer()
{
  onLogLine(newLogLineCallBack);
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
