package zigbeedevice

// https://www.zigbee2mqtt.io/devices/L1527.html
type L1527 struct {
	// ON OFF TOGGLE
	State string `json:"state"`
	// between 0 and 254
	Brightness int `json:"brightness"`
	// between 250 and 454.
	ColorTemp        int `json:"color_temp"`
	ColorTempStartup int `json:"color_temp_startup"`
	// transition time in seconds
	Transition int `json:"transition"`
	// blink, breathe, okay, channel_change, finish_effect, stop_effect
	Effect string `json:"effect"`
}
