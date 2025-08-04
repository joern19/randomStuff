#!/bin/sh
podman run -it --network host --restart=always -v $(dirname "$0"):/usr/local/freeswitch/scripts -v $(dirname "$0"):/usr/local/freeswitch/conf -v /webhookUrl.txt:/webhookUrl.txt --env-file=/home/joern/alarm.envfile freeswitch-custom
