#!/bin/sh

set -e

name=$(basename $(pwd))
scp -r . pi@kaboom.l.joern19.de:/tmp/$name
ssh pi@kaboom.l.joern19.de "cd /tmp/$name && podman build -t randomstuff_$name . && podman run -d --restart=unless-stopped --name $name --replace $@ randomstuff_$name"

