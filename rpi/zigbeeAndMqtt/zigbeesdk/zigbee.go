package sdk

import (
	"encoding/json"
	"fmt"

	mqtt "github.com/eclipse/paho.mqtt.golang"
)

// newDevice should return a new instance of the zigbee device (see package zigbeedevice)
func SubToZigbeeDevice[D any](mqttHelper MqttHelper, name string, newDevice func() D, callback func(D)) {
	topic := "zigbee/" + name
	c := func(msg MqttMessage) {
		device := newDevice()
		err := json.Unmarshal(msg.Message, &device)
		if err != nil {
			fmt.Printf("Device %s got the message '%s', which could not be unmarshaled to '%+v': %s\n", name, string(msg.Message), device, err)
			return
		}
		go callback(device)
	}
	mqttHelper.Sub(topic, c)
}

func PubToZigbeeDevice(mqttHelper MqttHelper, name string, settings any) mqtt.Token {
	topic := "zigbee/" + name + "/set"
	out, err := json.Marshal(settings)
	if err != nil {
		fmt.Printf("Failed to marshal: '%+v': %s", settings, err)
		return nil
	}
	return mqttHelper.Pub(topic, out)
}
