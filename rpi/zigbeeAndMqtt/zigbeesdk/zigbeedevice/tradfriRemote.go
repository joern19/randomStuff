package zigbeedevice

// https://www.zigbee2mqtt.io/devices/E1524_E1810.html
var PossibleActions = []string{
	"arrow_left_click", "arrow_left_hold", "arrow_left_release", "arrow_right_click", "arrow_right_hold", "arrow_right_release", "brightness_down_click", "brightness_down_hold", "brightness_down_release", "brightness_up_click", "brightness_up_hold", "brightness_up_release", "toggle",
}

type E1810 struct {
	// one of possibleActions
	Action string `json:"action"`
	// in percent
	Battery int `json:"battery"`
	// between 0 and 255
	Linkquality int `json:"linkquality"`
}
type E1524 E1810
