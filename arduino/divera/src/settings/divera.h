
// README:
// Connect the switch to D6 and ground
// Configure the application using the variables below
// Connect the Arduino to power and have fun

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

#define PIN_SWITCH D6 // The pin, the switch is connected to. (The switch should be connected to Ground and this pin)

//#define WIFI_HOSTNAME "esp8266-door-monitor"

#define ENABLE_MQTT // uncomment this, to connect to the mqtt broker below and publish the respective messages when the switch opens or closes.
#define MQTT_HOST "10.1.128.1"
#define MQTT_PORT 1883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define MQTT_ON_OPEN_TOPIC "null"
#define MQTT_ON_OPEN_MESSAGE "open"
#define MQTT_ON_CLOSE_TOPIC "dme/relay"
#define MQTT_ON_CLOSE_MESSAGE "close"
#define MQTT_CLIENT_ID "esp8266_441abf85-c1ec-4455-9085-af43eae9f655"

//#define DEBUG_WIFI_GENERIC(fmt, ...) Serial.printf( fmt, ##__VA_ARGS__ )

#endif