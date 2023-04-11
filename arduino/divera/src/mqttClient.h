#include "global.h"
#include <PubSubClient.h>

PubSubClient mqttClient(wifiClient);

void mqtt_callback(const char *topic, const byte *payload, unsigned int length) {
    Serial.print("Unexpected MQTT message on topic: ");
    Serial.println(topic);
    Serial.print("Message: -----");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println("-----");
}

void mqtt_connect() {
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCallback(mqtt_callback);
    Serial.println("Connecting to MQTT broker.");
    while (!mqttClient.connected()) {
        String client_id = MQTT_CLIENT_ID;
        if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
            Serial.println("Successfully connected to MQTT broker.");
        } else {
            Serial.print("Failed to connect to MQTT broker: ");
            Serial.println(mqttClient.state());
            delay(2000);
        }
    }
}

boolean mqtt_publish(const char* topic, const char* message) {
    return mqttClient.publish(topic, message);
}
