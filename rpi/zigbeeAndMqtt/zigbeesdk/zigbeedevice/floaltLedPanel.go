package zigbeedevice

// https://www.zigbee2mqtt.io/devices/L1527.html
type L1527 struct {
	// ON OFF TOGGLE
	State string `json:"state,omitempty"`
	// between 0 and 254
	Brightness int `json:"brightness,omitempty"`
	// between 250 and 454.
	ColorTemp int `json:"color_temp,omitempty"`
	// transition time in seconds
	Transition int `json:"transition,omitempty"`
	// blink, breathe, okay, channel_change, finish_effect, stop_effect
	Effect string `json:"effect,omitempty"`
}
