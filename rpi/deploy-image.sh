#!/bin/sh

set -e
name=$(basename $(pwd))
echo "Building podman image"
podman build --platform linux/arm64 -t randomstuff_$name:latest .
echo "Uploading to pi"
podman save randomstuff_$name:latest | ssh pi@kaboom.l.joern19.de podman load
echo "Start new image"
sh ../podman-run.sh "--name $name $@ randomstuff_$name"
