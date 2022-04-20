
// README:
// Configure the application using the variables below
// Connect the Arduino to power and have fun
// You should not have to touch any other file other than this.

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

#define PIN_SWITCH D6 // The pin, the switch is connected to. (The switch should be connected to Ground and this pin)

#define WIFI_SSID "<your ssid>" // The name of your wifi network
#define WIFI_PSK "<your password>" // The password of your wifi network
//#define WIFI_HOSTNAME "esp8266" // optionally uncomment this and set the hostname. (may not work: please look up, how hostnames work)

#define TCP_MAX_RETRIES 5 // The maximum number of tries, to send a http request

//#define ENABLE_TCP_ON_OPEN // uncomment this to send a tcp message, when the switch opens
#define TCP_ON_OPEN_HOST "httpbin.org" // the host to send the request to
#define TCP_ON_OPEN_PORT 443 // the port to connect to
const char* TCP_ON_OPEN_LINES[] = { // the lines to send to the host. May be http
  "GET /get HTTP/1.1", // http method ; path ; http version
  "Host: " TCP_ON_OPEN_HOST, // http header
  "user-agent: human/42",
  "", // empty line. I think http needs this.
  0 // This is important, to mark the end of the array
};

// same as above, but sends the lines on close, instead of when the switch opens
//#define ENABLE_TCP_ON_CLOSE // uncomment this to send a tcp message, when the switch closes
#define TCP_ON_CLOSE_HOST ""
#define TCP_ON_CLOSE_PORT 443
const char* TCP_ON_CLOSE_LINES[] = {
  0 // This is important, to mark the end of the array
};

//#define ENABLE_WEBSOCKET_SERVER // uncomment this to start a websocket server, that sends the strings below when the switch opens and closes.
#define WS_PORT 80
#define WS_ON_OPEN_MESSAGE "open" // text to send, when the switch opens
#define WS_ON_CLOSE_MESSAGE "closed" // text to send, when the switch closes

#endif