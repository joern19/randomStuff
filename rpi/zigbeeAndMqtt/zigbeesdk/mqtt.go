package sdk

import (
	"fmt"

	mqtt "github.com/eclipse/paho.mqtt.golang"
)

type MqttMessage struct {
	Topic   string
	Message []byte
	Ack     func()
}
type MqttHandler func(MqttMessage)

type MqttHelper struct {
	client mqtt.Client
	qos    byte
}

func NewMqttHelper(broker string) MqttHelper {
	opts := mqtt.NewClientOptions()
	opts.AddBroker(fmt.Sprintf("tcp://%s", broker))
	opts.SetClientID("go_mqtt_client")

	defaultHandler := func(client mqtt.Client, msg mqtt.Message) {
		fmt.Printf("[Warning] Unhandled message: %s on topic: %s\n", msg.Payload(), msg.Topic())
	}

	opts.SetDefaultPublishHandler(defaultHandler)
	client := mqtt.NewClient(opts)

	return MqttHelper{
		client: client,
		qos:    0,
	}
}

func (helper *MqttHelper) Connect() {
	client := helper.client
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		panic(token.Error())
	}
}

func (helper *MqttHelper) Sub(topic string, callback MqttHandler) mqtt.Token {
	return helper.client.Subscribe(topic, helper.qos, func(c mqtt.Client, m mqtt.Message) {
		msg := MqttMessage{
			Topic:   m.Topic(),
			Message: m.Payload(),
			Ack:     m.Ack,
		}
		callback(msg)
	})
}

func (helper *MqttHelper) Pub(topic string, msg interface{}) mqtt.Token {
	return helper.client.Publish(topic, helper.qos, false, msg)
}
