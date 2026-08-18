#!/bin/sh
ssh pi@kaboom.l.joern19.de "podman run -d --restart=unless-stopped --replace --network kaboom $@"
