
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
//#define USE_SSL // Uncomment this, to use SSL for all request

//#define ENABLE_TCP_ON_OPEN // uncomment this to send a tcp message, when the switch opens
#define TCP_ON_OPEN_HOST "httpbin.org" // the host to send the request to
#define TCP_ON_OPEN_PORT 443 // the port to connect to
const char* TCP_ON_OPEN_LINES[] = { // the lines to send to the host. MAY be http
  "GET /get HTTP/1.1", // http method ; path ; http version
  "Host: " TCP_ON_OPEN_HOST, // http header
  "user-agent: human/42",
  "", // An empty line after headers. See RFC 2616 4.1
  0 // A zero is used to indicate the end of the array
};

// same as above, but sends the lines on close, instead of when the switch opens
//#define ENABLE_TCP_ON_CLOSE // uncomment this to send a tcp message, when the switch closes
#define TCP_ON_CLOSE_HOST ""
#define TCP_ON_CLOSE_PORT 443
const char* TCP_ON_CLOSE_LINES[] = {
  0
};

// Because I want to use this program in a somewhat crictical use-case, I want to know if something is wrong. Therefore I send a http every x hours to see if the esp8266 is still online
// To reduce the complexity, this program will not be able to send two requests at once. Therefore I want to reduce the time spend sending the health request to a minimum. I will not check the response etc.
//#define ENABLE_TCP_HEALTH_REPORT // uncomment this to send a tcp message every x milliseconds (see below)
#define TCP_HEALTH_REPORT_INTERVAL 3600000 // (ms) the interval, in which the request should be made in milliseconds. 1h = 3600000ms
// See comments on TCP_ON_OPEN above
#define TCP_HEALTH_HOST "httpbin.org"
#define TCP_HEALTH_PORT 443
const char* TCP_HEALTH_LINES[] = {
  "POST /post HTTP/1.1",
  "",
  0
};

//#define ENABLE_WEBSOCKET_SERVER // uncomment this to start a websocket server, that sends the strings below when the switch opens and closes.
#define WS_PORT 80
#define WS_ON_OPEN_MESSAGE "open" // text to send, when the switch opens
#define WS_ON_CLOSE_MESSAGE "closed" // text to send, when the switch closes

#endif