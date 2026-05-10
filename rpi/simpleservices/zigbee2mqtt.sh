#!/bin/sh

# To enable uart, do raspbee-config and interface options -> serial port. There, disable login and enable serial port hardware.

./.generic.sh \
   --device=/dev/serial0:/dev/serial0 \
   -v /home/pi/zigbee2mqtt-data:/app/data \
   -v /run/udev:/run/udev:ro \
   --group-add keep-groups \
   -l traefik.enable=true \
   -l traefik.http.routers.concourse.entrypoints=web,websecure \
   -l traefik.http.services.concourse.loadbalancer.server.port=8080 \
   '-l "traefik.http.routers.concourse.rule=Host(\"zigbee.l.joern19.de\")"' \
   -e ZIGBEE2MQTT_CONFIG_mqtt_server=mqtt://kaboom.l.joern19.de:1883 \
   -e TZ=Europe/Amsterdam \
   ghcr.io/koenkk/zigbee2mqtt

#   -e ZIGBEE2MQTT_CONFIG_serial_baudrate=38400 \
#   -e ZIGBEE2MQTT_CONFIG_serial_adapter=deconz \
# -e ZIGBEE2MQTT_CONFIG_serial_port=/dev/serial0 \
