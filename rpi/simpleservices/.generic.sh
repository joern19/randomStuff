#!/bin/sh

# Create the network this way:
# podman network create --driver bridge --ipv6 kaboom # Make sure DNS is enabled!

PARENT_COMMAND=$(ps -o comm= $PPID)
ssh pi@kaboom.l.joern19.de "podman run -d --restart=unless-stopped --name ${PARENT_COMMAND%.*} --replace --network kaboom $@"

