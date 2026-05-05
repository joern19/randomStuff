package main

import (
	"encoding/json"
	"fmt"
	"log"

	mqtt "github.com/eclipse/paho.mqtt.golang"
)

type Payload struct {
	Contact    bool `json:"contact"`
	LowBattery bool `json:"battery_low"`
}

func messageHandler(client mqtt.Client, msg mqtt.Message) {
	fmt.Printf("Received message on topic %s: %s\n", msg.Topic(), msg.Payload())

	var payload Payload
	err := json.Unmarshal(msg.Payload(), &payload)
	if err != nil {
		log.Printf("Error parsing JSON: %v\n", err)
		return
	}

	if !payload.Contact {
		payload := `{"id":"flur","src":"my-new-topic","method":"Switch.Set","params":{"id":0,"on":true,"toggle_after":1}}`
		token := client.Publish("shellies/treppenhaus/rpc", 1, false, payload)
		token.Wait()
		fmt.Println("Turned lamp on!")
	}
}

// TODO: Only turn on in the night. (When is night??)
func main() {
	opts := mqtt.NewClientOptions()
	opts.AddBroker("mqtt://kaboom.l.joern19.de:1883")
	opts.SetClientID("go-router")
	opts.SetDefaultPublishHandler(messageHandler)
	client := mqtt.NewClient(opts)

	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error())
	}
	if token := client.Subscribe("zigbee2mqtt/0xa4c1383901896965", 0, nil); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error())
	}
	log.Println("OK")
	select {} // block forever
}
