#include <ESP8266WiFi.h>
#include "wifi.h"
#include "user_config.h"
#include <ESP8266mDNS.h>

#define WIFI_CONNECTION_DELAY_MS 500
unsigned long last_wifi_connection_attempt = 0;
int failed_connections_counter = 0;

void reconnectToWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }
  if (millis() - last_wifi_connection_attempt < WIFI_CONNECTION_DELAY_MS) {
    return;
  }
  failed_connections_counter++;
  if (failed_connections_counter > 10) {
    ESP.restart();
  }
  if  (WiFi.waitForConnectResult() == WL_CONNECTED) {
    MDNS.begin(HOST_NAME);
    failed_connections_counter = 0;
  }
  last_wifi_connection_attempt = millis();
  return;
}


void loopWifi() {
  reconnectToWifi();
}


void setupWifi() {
  WiFi.hostname(HOST_NAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PSWD);
  reconnectToWifi();
}
