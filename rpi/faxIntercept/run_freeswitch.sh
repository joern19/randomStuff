#!/bin/sh
podman run --rm -it --network host -v $(dirname "$0"):/usr/local/freeswitch/conf freeswitch "/usr/local/freeswitch/bin/freeswitch"
