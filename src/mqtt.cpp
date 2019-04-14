#include "mqtt.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "lighting.h"
#include "button.h"
#include "user_config.h"
#include "common.h"

#define MAX_ATTEMPTS_FOR_EACH_RECONNECT 1
#define DELAY_AFTER_FAILED_CONNECTION_MS 500

WiFiClient espClient;
PubSubClient client(espClient);
bool lastPublishedState = false;
bool lastPublishedSensorState = false;

void publishLightingState()
{
  String state = "OFF";
  if (isLightsOn)
  {
    state = "ON";
  }
#ifdef ENV_DEV
  writeToLog("Fake publiushing %s to %s", state.c_str(), MQTT_TOPIC_OUT);
#else
  client.publish(MQTT_TOPIC_OUT, state.c_str());
#endif

  lastPublishedState = isLightsOn;
}

void callback(char *topic, byte *payload, unsigned int length)
{
  if (length <= 0)
  {
    writeToLog("Toggling lights due to MQTT command");
    lightingToggle();
    publishLightingState();
    return;
  }
  if (((char *)payload)[1] == 'N')
  {
    writeToLog("Turning lights ON due to MQTT command");
    lightingOn();
    publishLightingState();
  }
  else
  {
    writeToLog("Turning lights OFF due to MQTT command");
    lightingOff();
    publishLightingState();
  }
}

void reconnectToMqtt()
{
  if (client.state() == MQTT_CONNECTED)
  {
    return;
  }
  bool output = client.connect(HOST_NAME, MQTT_USER, MQTT_PASS);
  if (output)
  {
    writeToLog("Connected to MQTT %s as %s", HOST_NAME, MQTT_USER);
    client.subscribe(MQTT_TOPIC_IN);
  }
  else
  {
    writeToLog("Failed to connected to MQTT %s as %s", HOST_NAME, MQTT_USER);
  }
}

void setupMqtt()
{
  client.setServer(MQTT_HOST_NAME, MQTT_PORT);
  client.setCallback(callback);
}

void publishSensorState()
{
  if (!long_click_sensor_state)
  {
    return;
  }
  String payload = "OFF";
  if (isLightsOn)
  {
    payload = "ON";
  }
#ifdef ENV_DEV
  writeToLog("Fake publiushing %s to %s", payload.c_str(), MQTT_LONG_CLICK_SENSOR);
#else
  client.publish(MQTT_LONG_CLICK_SENSOR, payload.c_str());
#endif
  long_click_sensor_state = false;
}

void loopMqtt()
{
  static unsigned long lastLoop = 0;
  static unsigned long lastTimeLightingStatePublished = 0;
  if (!globalIsWifiConnected)
  {
    return;
  }
  unsigned long l = millis() - lastLoop;
  if (l < 10)
  {
    return;
  }
  reconnectToMqtt();
  client.loop();
  lastLoop = millis();
  l = millis() - lastTimeLightingStatePublished;
  if (l > 5000)
  {
    publishLightingState();
    lastTimeLightingStatePublished = millis();
  }
  publishSensorState();
}
