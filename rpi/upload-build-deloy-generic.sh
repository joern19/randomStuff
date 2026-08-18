#!/bin/sh

set -e

name=$(basename $(pwd))
scp -r . pi@kaboom.l.joern19.de:/tmp/$name
podman build -t randomstuff_$name:latest .
podman save randomstuff_$name:latest | ssh pi@kaboom.l.joern19.de podman load
ssh pi@kaboom.l.joern19.de "podman run -d --restart=unless-stopped --name $name --replace $@ randomstuff_$name"

