package main

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"strings"
	"time"

	mqtt "github.com/eclipse/paho.mqtt.golang"
	suncalc "github.com/sixdouglas/suncalc"
)

type Payload struct {
	Contact    bool `json:"contact"`
	LowBattery bool `json:"battery_low"`
}

var LowBatteryNotified = false

func handleDiscord(context *AppContext, msg mqtt.Message) {
	split := strings.SplitN(msg.Topic(), "/", 2)

	context.discord.send(split[len(split)-1], string(msg.Payload()))
}

func handleDoorSensor(context *AppContext, msg mqtt.Message) {
	var payload Payload
	err := json.Unmarshal(msg.Payload(), &payload)
	if err != nil {
		log.Printf("Error parsing JSON: %v\n", err)
		return
	}

	if payload.LowBattery && !LowBatteryNotified {
		if context.discord.send("zigbee2mqtt/0xa4c1383901896965", "The battery of the door sensor is low!") {
			LowBatteryNotified = true
		}
	}

	if !payload.Contact {
		if !isDark() {
			fmt.Println("It is not Dark, not turning lamp on.")
			return
		}
		payload := `{"id":"flur","src":"my-new-topic","method":"Switch.Set","params":{"id":0,"on":true,"toggle_after":1}}`
		token := context.client.Publish("shellies/treppenhaus/rpc", 1, false, payload)
		token.Wait()
		fmt.Println("Turned lamp on!")
	}
}

func isDark() bool {
	var now = time.Now()
	lat, long := 52., 9.
	var times = suncalc.GetTimes(now.UTC(), lat, long)

	sunriseEnd := times[suncalc.SunriseEnd].Value
	sunsetStart := times[suncalc.SunsetStart].Value

	return now.Before(sunriseEnd) || now.After(sunsetStart)
}

type AppContext struct {
	client  mqtt.Client
	discord DiscordWebhook
}

func lookupEnvOrFail(key string) string {
	value, found := os.LookupEnv(key)
	if !found {
		log.Fatalln("env with not found:", key)
	}
	return strings.TrimSpace(value)
}

func main() {
	opts := mqtt.NewClientOptions()
	opts.AddBroker("mqtt://kaboom.l.joern19.de:1883")
	opts.SetClientID("go-router")
	client := mqtt.NewClient(opts)
	client.AddRoute("#", func(_ mqtt.Client, msg mqtt.Message) {
		log.Printf("Received message on topic %s: %s\n", msg.Topic(), msg.Payload())
	})
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error())
	}

	appContext := &AppContext{
		client,
		DiscordWebhook{
			lookupEnvOrFail("DISCORD_WEBHOOK"),
		},
	}

	subscribe := func(topic string, handler func(*AppContext, mqtt.Message)) {
		log.Println("Subscribing to:", topic)
		if token := client.Subscribe(topic, 0, func(c mqtt.Client, m mqtt.Message) {
			handler(appContext, m)
		}); token.Wait() && token.Error() != nil {
			log.Fatal(token.Error())
		}
	}

	subscribe("zigbee2mqtt/0xa4c1383901896965", handleDoorSensor)
	subscribe("discord/#", handleDiscord)
	log.Println("OK")
	select {} // block forever
}
