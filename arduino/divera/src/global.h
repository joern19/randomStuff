#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "settings.h"

bool state = false;

#ifdef USE_SSL
WiFiClientSecure wifiClient;
wifiClient.setInsecure();
#else
WiFiClient wifiClient;
#endif

#endif