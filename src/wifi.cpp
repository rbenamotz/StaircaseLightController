#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "user_config.h"
#include "common.h"

static void onStationModeConnected(const WiFiEventStationModeConnected &event)
{
  writeToLog("WiFi Connected. SSID=%s", event.ssid.c_str());
}

static void onStationModeDisconnected(const WiFiEventStationModeDisconnected &event)
{
  writeToLog("WiFi Disconnected. Reason code=%d", event.reason);
  globalIsWifiConnected = false;
}

static void onStationModeGotIP(const WiFiEventStationModeGotIP &event)
{
  writeToLog("WiFi Got IP. localIP=%s, hostname=%s", event.ip.toString().c_str(), WiFi.hostname().c_str());
  globalIsWifiConnected = true;
  MDNS.begin(HOST_NAME);
}

static void onStationModeDHCPTimeout()
{
  writeToLog("WiFi DHCP timed out.");
  globalIsWifiConnected = false;
}

void setupWifi()
{
  static WiFiEventHandler e1 = WiFi.onStationModeConnected(onStationModeConnected);
  static WiFiEventHandler e2 = WiFi.onStationModeDisconnected(onStationModeDisconnected);
  static WiFiEventHandler e3 = WiFi.onStationModeGotIP(onStationModeGotIP);
  static WiFiEventHandler e4 = WiFi.onStationModeDHCPTimeout(onStationModeDHCPTimeout);
  WiFi.hostname(HOST_NAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PSWD);
}
void loopWifi()
{
}
