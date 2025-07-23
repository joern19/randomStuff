#!/bin/sh
podman run -it --network host --restart=always -v $(dirname "$0"):/usr/local/freeswitch/conf -v /webhookUrl.txt:/webhookUrl.txt freeswitch-custom "/usr/local/freeswitch/bin/freeswitch"
