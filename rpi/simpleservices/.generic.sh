#!/bin/sh
PARENT_COMMAND=$(ps -o comm= $PPID)
ssh pi@kaboom.l.joern19.de "podman run -d --restart=unless-stopped --name ${PARENT_COMMAND%.*} --replace --network podman $@"

