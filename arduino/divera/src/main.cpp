// Please open settings.h for instructions and configuration.
// You do not have to change/read anything here.

#include "global.h"

#ifdef ENABLE_WEBSOCKET_SERVER
#include "websocketServer.h"
#endif

#if defined(ENABLE_TCP_ON_OPEN) || defined(ENABLE_TCP_ON_CLOSE) || defined(ENABLE_TCP_HEALTH_REPORT)
#include "tcp.h"
#endif

#ifdef ENABLE_MQTT
#include "mqttClient.h"
#endif

void setup() {
  Serial.begin(115200);

  // setup wifi
  Serial.println();
  Serial.print("Connecting to wifi");

  #ifdef WIFI_HOSTNAME
  WiFi.hostname(WIFI_HOSTNAME);
  #endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("Wifi connected!");
  Serial.println("local IP: " + WiFi.localIP().toString());

  #ifdef ENABLE_WEBSOCKET_SERVER
  setup_websocket_server();
  #endif

  #ifdef ENABLE_MQTT
  mqtt_connect();
  #endif

  pinMode(PIN_SWITCH, INPUT_PULLUP);
  state = digitalRead(PIN_SWITCH) == HIGH;
}

#ifdef ENABLE_TCP_HEALTH_REPORT
unsigned long lastExec = 0;
#endif
void loop() {
  delay(1); // May reduce power consumption: https://hackaday.com/2022/10/28/esp8266-web-server-saves-60-power-with-a-1-ms-delay/

  #ifdef ENABLE_WEBSOCKET_SERVER
  websocket_server_loop();
  #endif

  #ifdef ENABLE_MQTT
  mqttClient.loop();
  #endif

  #ifdef ENABLE_TCP_HEALTH_REPORT
  if ((millis() - lastExec) > TCP_HEALTH_REPORT_INTERVAL) {
    send_tcp(TCP_HEALTH_HOST, TCP_HEALTH_PORT, TCP_HEALTH_LINES);
    Serial.println("HEALTH REPORT sent.");
    lastExec = millis();
  }
  #endif

  bool oldState = state;
  state = digitalRead(PIN_SWITCH) == HIGH;

  if (state != oldState) {
    if (state) {
      Serial.println("The switch opened.");
      #ifdef ENABLE_TCP_ON_OPEN
      send_tcp(TCP_ON_OPEN_HOST, TCP_ON_OPEN_PORT, TCP_ON_OPEN_LINES);
      #endif
      #ifdef ENABLE_MQTT
      mqtt_publish(MQTT_ON_OPEN_TOPIC, MQTT_ON_OPEN_MESSAGE);
      #endif
    } else {
      Serial.println("The switch closed.");
      #ifdef ENABLE_TCP_ON_CLOSE
      send_tcp(TCP_ON_CLOSE_HOST, TCP_ON_CLOSE_PORT, TCP_ON_CLOSE_LINES);
      #endif
      #ifdef ENABLE_MQTT
      mqtt_publish(MQTT_ON_CLOSE_TOPIC, MQTT_ON_CLOSE_MESSAGE);
      #endif
    }
    #ifdef ENABLE_WEBSOCKET_SERVER
    websocket_server_on_state_change();
    #endif
  }
}