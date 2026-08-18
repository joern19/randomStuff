#!/bin/sh

# Create the network this way:
# podman network create --driver bridge --ipv6 kaboom # Make sure DNS is enabled!

PARENT_COMMAND=$(ps -o comm= $PPID)
sh ../podman-run.sh "--name ${PARENT_COMMAND%.*} --replace --network kaboom $@"
