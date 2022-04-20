#include <WebSocketsServer.h>
#include "global.h"

WebSocketsServer webSocketServer = WebSocketsServer(WS_PORT);

void setup_websocket_server() {
  webSocketServer.begin();
  webSocketServer.onEvent(webSocketHandler);
}

void webSocketHandler(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: 
      {
        IPAddress ip = webSocketServer.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        if (state) {
          #ifdef WS_ON_OPEN_MESSAGE
          webSocketServer.sendTXT(num, WS_ON_OPEN_MESSAGE);
          #endif
        } else {
          #ifdef WS_ON_CLOSE_MESSAGE
          webSocketServer.sendTXT(num, WS_ON_CLOSE_MESSAGE);
          #endif
        }
      }
      break;
    default:
      break;
  }
}

void onStateChange() {
  if (state) {
    #ifdef WS_ON_OPEN_MESSAGE
    webSocketServer.broadcastTXT(WS_ON_OPEN_MESSAGE);
    #endif
  } else {
    #ifdef WS_ON_CLOSE_MESSAGE
    webSocketServer.broadcastTXT(WS_ON_CLOSE_MESSAGE);
    #endif
  }
}
