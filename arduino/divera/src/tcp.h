#include "global.h"

void send_tcp(const char* host, uint16_t port, const char** lines) {
  wifiClient.setInsecure();

  int retries = TCP_MAX_RETRIES;
  while (!wifiClient.connect(host, port) && retries > 0) {
    retries--;
    Serial.printf("Failed to connect to %s", host);
    delay(100);
  }
  if (retries < 1) {
    return;
  }

  Serial.print("Sending TCP message to server.");

  for (size_t i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
    wifiClient.println(lines[i]);
  }

  delay(500); // Can be changed
  while (wifiClient.connected()) {
    String line = wifiClient.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      Serial.println("Breaking...");
      break;
    }
  }
  Serial.println(wifiClient.readStringUntil('\n'));
  wifiClient.stop(); // DISCONNECT FROM THE SERVER
  Serial.println();
  Serial.println("closing connection");
  delay(5000);
}