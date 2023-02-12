package main

import (
	"fmt"
	"zigbeeAndMqtt/zigbeedevice"
)

const broker = "10.1.128.1:1883"
const remote = "0x842e14fffe586da3"
const panel = "0x680ae2fffea3eb13"

func main() {
	mqttHelper := NewMqttHelper(broker)
	mqttHelper.connect()
	listenOnPhysicalRemote(mqttHelper)
	<-make(chan int)
}

func listenOnPhysicalRemote(mqttHelper MqttHelper) {
	newDevice := func() zigbeedevice.E1810 {
		return zigbeedevice.E1810{}
	}
	handler := func(device zigbeedevice.E1810) {
		switch device.Action {
		case "toggle":
			PubToZigbeeDevice(mqttHelper, panel, zigbeedevice.L1527{
				Effect: "blink",
			})
		case "arrow_left_click":
			PubToZigbeeDevice(mqttHelper, panel, zigbeedevice.L1527{
				Effect: "breathe",
			})
		default:
			fmt.Println("Unhandled action: " + device.Action)
		}
	}
	SubToZigbeeDevice(mqttHelper, remote, newDevice, handler)
}
