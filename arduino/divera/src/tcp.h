#include "global.h"

WiFiClientSecure wifiClient;

void send_tcp(const char* host, uint16_t port, const char** lines) {
  wifiClient.setInsecure();

  int retries = TCP_MAX_RETRIES;
  while (!wifiClient.connect(host, port) && retries > 0) {
    retries--;
    Serial.printf("Failed to connect to %s\n", host);
    delay(100);
  }
  if (retries < 1) {
    return;
  }

  Serial.println("Sending TCP message to server.");

  for (int i = 0; lines[i]; i++) {
    Serial.println(lines[i]);
    wifiClient.println(lines[i]);
  }

  delay(500); // Can be changed
  while (wifiClient.connected()) {
    String line = wifiClient.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      Serial.println("First line of Body:");
      break;
    }
  }
  Serial.println(wifiClient.readStringUntil('\n'));
  wifiClient.stop(); // DISCONNECT FROM THE SERVER
  Serial.println();
  Serial.println("closing connection");
  delay(5000);
}