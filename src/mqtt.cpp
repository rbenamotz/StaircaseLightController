#include "mqtt.h"
#include <ESP8266WiFi.h>
#include "user_config.h"
#include "common.h"
#include <AsyncMqttClient.h>

#define MAX_ATTEMPTS_FOR_EACH_RECONNECT 1
#define DELAY_AFTER_FAILED_CONNECTION_MS 500

WiFiClient espClient;
AsyncMqttClient mqttClient;
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
  writeToLog("Publishing light state (%s to %s)", state.c_str(), MQTT_TOPIC_OUT);
  mqttClient.publish(MQTT_TOPIC_OUT, 0, false, state.c_str());
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

void onMqttConnect(bool sessionPresent)
{
  writeToLog("Connected to MQTT %s as %s. SessionPresent: %d", MQTT_HOST_NAME, MQTT_USER, sessionPresent);
  uint16_t packetIdSub = mqttClient.subscribe(MQTT_TOPIC_IN, 0);
  writeToLog("Subscribed to %s. Packet ID: %d", MQTT_TOPIC_IN, packetIdSub);
  publishLightingState(true);
  // mqttClient.publish("test/lol", 0, true, "test 1");
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  writeToLog("Disconnected from MQTT. Reason: %d. Reconecting", reason);
  mqttClient.connect();
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
  writeToLog("Subscribe acknowledged. Packet ID: %d, QOS: %d", packetId, qos);
}

void onMqttUnsubscribe(uint16_t packetId)
{
  writeToLog("Unsubscribe acknowledged for packet ID: %d.", packetId);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  writeToLog("Recieved message for topic \"%s\", qos: %d, total: %d, len: %d, index: %d", topic, properties.qos, total, len, index);
  if (len == 0)
  {
    writeToLog("Toggling lights due to MQTT command");
    globalIsLightsOn = !globalIsLightsOn;
    return;
  }
  if (payload[1] == 'N')
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

void onMqttPublish(uint16_t packetId)
{
  writeToLog("Publish acknowledged. Packet ID: %d", packetId);
}
// writeToLog("Failed to connected to MQTT %s as %s. State is: %d", HOST_NAME, MQTT_USER,client.state());

void reconnectToMqtt()
{
  static unsigned long lastConectionAttempt = 0lu;
  unsigned long l = millis() - lastConectionAttempt;
  if (!globalIsWifiConnected || l < 500)
  {
    return;
  }
  writeToLog("Trying to connect to MQTT");
  mqttClient.connect();
  lastConectionAttempt = millis();
}

void setupMqtt()
{

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST_NAME, MQTT_PORT);
  mqttClient.setClientId(HOST_NAME);
  mqttClient.setCredentials(MQTT_USER, MQTT_PASS);
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
  mqttClient.publish(MQTT_LONG_CLICK_SENSOR, 0, false, payload.c_str());
#endif
  globalShouldPublishLongClick = false;
}

void loopMqtt()
{
  static bool b = true;
  if (b && globalIsWifiConnected)
  {
    writeToLog("Connecting to MQTT");
    mqttClient.connect();
    b = !b;
  }
  // reconnectToMqtt();
  // lastLoop = millis();
  // l = millis() - lastTimeLightingStatePublished;
  // if (l > 5000)
  // {
  //   publishLightingState();
  //   lastTimeLightingStatePublished = millis();
  // }
  publishLightingState();
  publishSensorState();
  // client.loop();
}
