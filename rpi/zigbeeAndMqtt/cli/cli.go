package main

import (
	"flag"
	"fmt"

	"github.com/joern19/zigbeeAndMqtt/sdk"
	"github.com/joern19/zigbeeAndMqtt/sdk/zigbeedevice"
)

type Flags struct {
	Broker string
	Name   string

	State string
	// between 0 and 254
	Brightness int
	// between 250 and 454.
	ColorTemp int
	// transition time in seconds
	Transition int
	// blink, breathe, okay, channel_change, finish_effect, stop_effect
	Effect string
}

func createFlags() Flags {
	flags := Flags{}
	flag.StringVar(&flags.Broker, "broker", "pi4:1883", "The address of the mqtt broker. eg: 'localhost:1883'")
	flag.StringVar(&flags.Name, "name", "0x680ae2fffea3eb13", "The FRIENDLY_NAME of the zigbee device")

	flag.StringVar(&flags.State, "state", "", "The new State: TOGGLE, ON or OFF")
	flag.IntVar(&flags.Brightness, "brightness", 0, "The new Brightness: between 1 and 254")
	flag.IntVar(&flags.ColorTemp, "color", 0, "The new Color Temperature: between 250 and 454")
	flag.IntVar(&flags.Transition, "transition", 0, "The transition time to the new state in seconds")

	var blink bool
	flag.BoolVar(&blink, "blink", false, "An effect. Only one Effect can be enabled")
	var breathe bool
	flag.BoolVar(&breathe, "breathe", false, "An effect. Only one Effect can be enabled")

	flag.Parse()
	if blink {
		flags.Effect = "blink"
	}
	if breathe {
		flags.Effect = "breathe"
	}
	return flags
}

func main() {
	flags := createFlags()
	fmt.Printf("Flags: %+v\n", flags)

	mqttHelper := sdk.NewMqttHelper(flags.Broker)
	mqttHelper.Connect()
	token := sdk.PubToZigbeeDevice(mqttHelper, flags.Name, zigbeedevice.L1527{
		State:      flags.State,
		Brightness: flags.Brightness,
		ColorTemp:  flags.ColorTemp,
		Transition: flags.Transition,
		Effect:     flags.Effect,
	})
	if token != nil {
		token.Wait()
	}
}
