package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"time"
)

type DiscordWebhookEmbedPayload struct {
	Title       string `json:"title"`
	Description string `json:"description"`
	Timestamp   string `json:"timestamp"`
	Color       int    `json:"color"`
}

type DiscordWebhookPayload struct {
	Content string                       `json:"content"`
	Embeds  []DiscordWebhookEmbedPayload `json:"embeds"`
}

type DiscordWebhook struct {
	url string
}

func (dw *DiscordWebhook) send(title string, description string) bool {
	log.Println("Sending message to discord: ", title, description)
	body := DiscordWebhookPayload{
		Content: "",
		Embeds: []DiscordWebhookEmbedPayload{
			DiscordWebhookEmbedPayload{
				Title:       title,
				Description: description,
				Color:       6684774,
				Timestamp:   time.Now().Format(time.RFC3339),
			},
		},
	}

	jsonData, err := json.Marshal(body)
	if err != nil {
		fmt.Println("Failed to marshal json: ", body)
		return false
	}

	resp, err := http.Post(dw.url, "application/json", bytes.NewBuffer(jsonData))
	if err != nil {
		fmt.Println("Failed to send request to discord:", err.Error())
		return false
	}
	defer resp.Body.Close()
	return true
}
