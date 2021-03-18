const http = require('http');

const API_KEY = "552F5AF776";

const GET_STATE = "raspberrypi.fritz.box/api/" + API_KEY + "/lights/2";

function isOn() {
  return 
}

var sw = false;

http.createServer(function (request, response) {
  response.writeHead(200, {'Content-Type': 'text/plain'});
  if (request.url == "/lamp/on") {
    sw = false;
    console.log("Lamp is on.");
  } else if (request.url == "/lamp/off") {
    sw = true;
    console.log("Lamp is off.");
  } else if (request.url == "/lamp/state") {
    response.writeHead(200, {'Content-Type': 'text/json'});
    if (sw) {
      response.write("{'state': 'off'}");
    } else {
      response.write("{'state': 'on'}");
    }
  }

  response.end();
}).listen(8080);

console.log("Server running on Port 8080");