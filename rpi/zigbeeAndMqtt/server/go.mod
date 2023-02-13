module github.com/joern19/zigbeeAndMqtt/server

go 1.20

replace github.com/joern19/zigbeeAndMqtt/sdk => ../zigbeesdk

require github.com/joern19/zigbeeAndMqtt/sdk v0.0.0-00010101000000-000000000000

require (
	github.com/eclipse/paho.mqtt.golang v1.4.2 // indirect
	github.com/gorilla/websocket v1.5.0 // indirect
	golang.org/x/net v0.6.0 // indirect
	golang.org/x/sync v0.1.0 // indirect
)
