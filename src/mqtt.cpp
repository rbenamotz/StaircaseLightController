#include "mqtt.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "button.h"
#include "user_config.h"
#include "common.h"

#define MAX_ATTEMPTS_FOR_EACH_RECONNECT 1
#define DELAY_AFTER_FAILED_CONNECTION_MS 500

WiFiClient espClient;
PubSubClient client(espClient);
bool lastPublishedSensorState = false;

void publishLightingState(bool force = false)
{
  static bool lastPublishedState = false;
  if (!force && lastPublishedState == globalIsLightsOn)
  {
    return;
  }

  String state = "OFF";
  if (globalIsLightsOn)
  {
    state = "ON";
  }
#ifdef ENV_DEV
  writeToLog("Fake publishing light state (%s to %s)", state.c_str(), MQTT_TOPIC_OUT);
#else
  client.publish(MQTT_TOPIC_OUT, state.c_str());
#endif

  lastPublishedState = globalIsLightsOn;
}

void callback(char *topic, byte *payload, unsigned int length)
{
  if (length <= 0)
  {
    writeToLog("Toggling lights due to MQTT command");
    globalIsLightsOn = !globalIsLightsOn;
    return;
  }
  if (((char *)payload)[1] == 'N')
  {
    writeToLog("Turning lights ON due to MQTT command");
    globalIsLightsOn = true;
  }
  else
  {
    writeToLog("Turning lights OFF due to MQTT command");
    globalIsLightsOn = false;
  }
}

void reconnectToMqtt()
{
  static unsigned long lastConectionAttempt = 0lu;
  unsigned long l = millis() - lastConectionAttempt;
  if (!globalIsWifiConnected)
  {
    return;
  }
  if (l < 500)
  {
    return;
  }
  if (client.connected())
  {
    return;
  }
  bool output = client.connect(HOST_NAME, MQTT_USER, MQTT_PASS);
  if (output)
  {
    writeToLog("Connected to MQTT %s as %s", MQTT_HOST_NAME, MQTT_USER);
    client.subscribe(MQTT_TOPIC_IN);
  }
  else
  {
    writeToLog("Failed to connected to MQTT %s as %s. State is: %d", HOST_NAME, MQTT_USER,client.state());
  }
  lastConectionAttempt = millis();
}

void setupMqtt()
{
  client.setServer(MQTT_HOST_NAME, MQTT_PORT);
  client.setCallback(callback);
}

void publishSensorState()
{
  if (!globalShouldPublishLongClick)
  {
    return;
  }
  String payload = "OFF";
  if (globalIsLightsOn)
  {
    payload = "ON";
  }
#ifdef ENV_DEV
  writeToLog("Fake publishing sensor state (%s to %s)", payload.c_str(), MQTT_LONG_CLICK_SENSOR);
#else
  client.publish(MQTT_LONG_CLICK_SENSOR, payload.c_str());
#endif
  globalShouldPublishLongClick = false;
}

void loopMqtt()
{
  reconnectToMqtt();
  // lastLoop = millis();
  // l = millis() - lastTimeLightingStatePublished;
  // if (l > 5000)
  // {
  //   publishLightingState();
  //   lastTimeLightingStatePublished = millis();
  // }
  publishLightingState();
  publishSensorState();
  client.loop();
}
