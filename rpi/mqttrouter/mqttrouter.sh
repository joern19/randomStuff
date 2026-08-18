#!/bin/sh
GOARCH=arm go build
../deploy-image.sh --secret mqttDiscordWebhook,type=env,target=DISCORD_WEBHOOK
