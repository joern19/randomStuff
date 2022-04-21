// Please open settings.h for instructions and configuration.
// You do not have to change/read anything here.

#include "global.h"

#ifdef ENABLE_WEBSOCKET_SERVER
#include "websocketServer.h"
#endif

#if defined(ENABLE_TCP_ON_OPEN) || defined(ENABLE_TCP_ON_CLOSE)
#include "tcp.h"
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

  // setup websocket server
  #ifdef ENABLE_WEBSOCKET_SERVER
  setup_websocket_server();
  #endif

  // setup the switch
  pinMode(PIN_SWITCH, INPUT_PULLUP);
  state = digitalRead(PIN_SWITCH) == HIGH;
}

void loop() {
  #ifdef ENABLE_WEBSOCKET_SERVER
  websocket_server_loop();
  #endif

  bool oldState = state;
  state = digitalRead(PIN_SWITCH) == HIGH;

  if (state != oldState) {
    if (state) {
      Serial.println("The switch opened.");
      #ifdef ENABLE_TCP_ON_OPEN
      send_tcp(TCP_ON_OPEN_HOST, TCP_ON_OPEN_PORT, TCP_ON_OPEN_LINES);
      #endif
    } else {
      Serial.println("The switch closed.");
      #ifdef ENABLE_TCP_ON_CLOSE
      send_tcp(TCP_ON_CLOSE_HOST, TCP_ON_CLOSE_PORT, TCP_ON_CLOSE_LINES);
      #endif
    }
    #ifdef ENABLE_WEBSOCKET_SERVER
    websocket_server_on_state_change();
    #endif
  }
}