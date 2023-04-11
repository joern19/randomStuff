package main

import (
	"fmt"
	"net/http"
	"strings"
	"time"

	"github.com/joern19/zigbeeAndMqtt/sdk"
	"github.com/joern19/zigbeeAndMqtt/sdk/zigbeedevice"
)

const Broker = "10.1.128.1:1883"
const Remote = "0x842e14fffe586da3"
const Panel = "0x680ae2fffea3eb13"

// The service connects the mqtt devices together: remote -> zigbee2mqtt -> this -> zigbee2mqtt -> lamp
func main() {
	mqttHelper := sdk.NewMqttHelper(Broker)
	mqttHelper.Connect()
	listenOnPhysicalRemote(mqttHelper)
	listenForAlarm(mqttHelper)
	<-make(chan int)
}

func listenOnPhysicalRemote(mqttHelper sdk.MqttHelper) {
	newDevice := func() zigbeedevice.E1810 {
		return zigbeedevice.E1810{}
	}
	handler := func(device zigbeedevice.E1810) {
		switch device.Action {
		case "toggle":
			fmt.Println("toggle")
			sdk.PubToZigbeeDevice(mqttHelper, Panel, zigbeedevice.L1527{
				State: "TOGGLE",
			})
		case "toggle_hold":
			fmt.Println("Blinking")
			sdk.PubToZigbeeDevice(mqttHelper, Panel, zigbeedevice.L1527{
				Effect: "blink",
			})
		case "brightness_up_click":
			fmt.Println("bright")
			sdk.PubToZigbeeDevice(mqttHelper, Panel, zigbeedevice.L1527{
				Brightness: 253,
				Transition: 0,
			})
		case "brightness_down_click":
			fmt.Println("dimm")
			sdk.PubToZigbeeDevice(mqttHelper, Panel, zigbeedevice.L1527{
				Brightness: 1,
				Transition: 0,
			})
		case "arrow_left_hold":
			fmt.Println("Breathing")
			sdk.PubToZigbeeDevice(mqttHelper, Panel, zigbeedevice.L1527{
				Effect: "breathe",
			})
		case "arrow_left_click":
			fmt.Println("cold")
			sdk.PubToZigbeeDevice(mqttHelper, Panel, zigbeedevice.L1527{
				ColorTemp:  250,
				Transition: 0,
			})
		case "arrow_right_click":
			fmt.Println("warm")
			sdk.PubToZigbeeDevice(mqttHelper, Panel, zigbeedevice.L1527{
				ColorTemp:  454,
				Transition: 0,
			})
		default:
			fmt.Println("Unhandled action: " + device.Action)
		}
	}
	sdk.SubToZigbeeDevice(mqttHelper, Remote, newDevice, handler)
}

func callDivera() {
	// body := `{"type": "DME", "text": "Automatische Alarmierung durch digitalen Meldeempfänger. Details können aus technischen Gründen nicht bereitgestellt werden."}`
}

func isTestAlarm() bool {
	// The test alarm is every month on the 15th at 19 o'clock.
	now := time.Now()
	if now.Day() == 15 {
		if now.Hour() == 19 && now.Minute() < 3 { // Tolerance of + 2
			return true
		}
		if now.Hour() == 18 && now.Minute() > 58 { // Tolerance of -1
			return true
		}
	}
	return false
}

// This is stuff to happen, when I get a call from my volunteer fire department.
func listenForAlarm(mqttHelper sdk.MqttHelper) {
	handleAlarm := func() {
		fmt.Println("[Alarm] triggered!")

		if isTestAlarm() {
			fmt.Println("[Alarm] is a test alarm. Nothing will be done.")
			return
		}

		callDivera()
		sdk.PubToZigbeeDevice(mqttHelper, Panel, zigbeedevice.L1527{
			ColorTemp:  250,
			Transition: 10, // Maybe this will not flashbang me?
			Brightness: 253,
		})

		time.Sleep(3 * time.Minute) // 3 Minutes shall be more then enogh. HURRY!
		fmt.Println("[Alarm] Turning lights off.")
		sdk.PubToZigbeeDevice(mqttHelper, Panel, zigbeedevice.L1527{
			State: "OFF",
		})
	}

	mqttHelper.Sub("dme/relay", func(msg sdk.MqttMessage) {
		if string(msg.Message[:]) == "close" {
			go handleAlarm()
		}
	})
}
