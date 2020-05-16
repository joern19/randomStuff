const fs = require('fs'),
      http = require('http');

const filePath = "./index.html";
var index = null;

fs.readFile(filePath, function(error, file) {
  if (error) {
    console.log("Error while reading File.");
    return;
  }
  index = file;
});

http.createServer(function (request, response) {
  response.writeHead(200, {'Content-Type': 'text/html'});
  response.write(index);
  response.end();
}).listen(8000);

console.log("Server running on Port 8000");
