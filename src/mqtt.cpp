#include "mqtt.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "lighting.h"
#include "button.h"
#include "user_config.h"

#define MAX_ATTEMPTS_FOR_EACH_RECONNECT 1
#define DELAY_AFTER_FAILED_CONNECTION_MS 500

unsigned long lastConnectionAttempt = 0;
unsigned long lastTimeLightingStatePublished = 0;
unsigned long lastMqttConectionCheck = 0;

WiFiClient espClient;
PubSubClient client(espClient);
bool lastPublishedState = false;
bool lastPublishedSensorState = false;

void callback(char* topic, byte* payload, unsigned int length) {
  if (length<=0) {
    lightingToggle();
    return;
  }
  if (((char*) payload)[1]=='N') {
    lightingOn();
  } else {
    lightingOff();
  }
}


bool reconnect() {
  if (client.connected()) {
    return true;
  }
  if (millis() - lastConnectionAttempt < DELAY_AFTER_FAILED_CONNECTION_MS) {
    return false;
  }
  bool output = client.connect(HOST_NAME,MQTT_USER,MQTT_PASS);
  if (output) {
    client.subscribe(MQTT_TOPIC_IN);
  }
  lastConnectionAttempt = millis();
  return output;

}

void setupMqtt() {
  client.setServer(MQTT_HOST_NAME, MQTT_PORT);
  client.setCallback(callback);
}

void publishLightingState() {
  String state = "OFF";
  if (isLightsOn) {
    state = "ON";
  }
  client.publish(MQTT_TOPIC_OUT , state.c_str());
  lastPublishedState = isLightsOn;
}

void publishSensorState() {
  String state = "OFF";
  if (long_click_sensor_state) {
    state = "ON";
  }
  client.publish(MQTT_LONG_CLICK_SENSOR , state.c_str());
  lastPublishedSensorState = long_click_sensor_state;
}



void loopMqtt() {
  unsigned long l = millis() - lastMqttConectionCheck;
  if (!reconnect()) {
    return;
  }
  l = millis() - lastTimeLightingStatePublished;
  if (isLightsOn!=lastPublishedState || l>5000) {
    publishLightingState();
    lastTimeLightingStatePublished = millis();
  }
  if (long_click_sensor_state!=lastPublishedSensorState) {
    publishSensorState();
  }
  client.loop();
  lastMqttConectionCheck = millis();
}
