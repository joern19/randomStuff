#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "7902d746-7607-4757-940c-6bea6366";
const char* password = "W9-!@h[a!)d=pU$R\\yCN+s6V-nTzxR";
const char* mqtt_server = "192.168.178.32";
const char* mqtt_username = "door-monitor";
const char* mqtt_password = "xRm^<gRt$%ux9aQR7W7YVpAi<Zeu=W";
const char* mqtt_topic = "door-joern";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

bool doorOpen;

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("Wifi connected!");
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    Serial.println("client-id: " + clientId);
    if (mqttClient.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publish_state() {
  if (doorOpen) {
    if (!mqttClient.publish("door-joern", "open")) {
      Serial.println("Publish failed.");
    }
  } else {
    if (!mqttClient.publish("door-joern", "closed")) {
      Serial.println("Publish failed.");
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  mqttClient.setServer(mqtt_server, 1883);

  pinMode(D5, INPUT);
  if (digitalRead(D5) == HIGH) {
    doorOpen = true;
  } else {
    doorOpen = false;
  }
  publish_state();
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  if (digitalRead(D5) == HIGH) {
    if (!doorOpen) {
      doorOpen = true;
      publish_state();
    }
  } else {
    if (doorOpen) {
      doorOpen = false;
      publish_state();
    }
  }

}