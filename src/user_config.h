#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_



//Hardware
#define LED_PIN 5
#define LED_STRIP_PIN 14
#define BUTTON_PIN 0
#define RELAY_PIN 13


//WiFi

#define HOST_NAME  "stairway"
#define WIFI_SSID "my wifi SSID"
#define WIFI_PSWD "my very secret password"

//MQTT

#define MQTT_HOST_NAME "MQTT server IP"
#define MQTT_PORT 1883
#define MQTT_USER "MQTT user"
#define MQTT_PASS "MQTT password"
#define MQTT_TOPIC_OUT "home-assistant/switch/stairway/state"
#define MQTT_TOPIC_IN "home-assistant/switch/stairway/set"
#define MQTT_LONG_CLICK_SENSOR "home-assistant/switch/all-basement-indication/state"


#endif
