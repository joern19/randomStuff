#!/bin/sh

# TODO: e2ee without nextcloud, Push service

./.generic.sh --init \
  -v /home/pi/nextcloud/custom_apps:/var/www/html/custom_apps \
  -v /home/pi/nextcloud/config:/var/www/html/config \
  -v /home/pi/nextcloud/data:/var/www/html/data \
  -l traefik.enable=true \
  -l traefik.http.routers.nextcloud.entrypoints=websecure \
  -l traefik.http.routers.nextcloud.tls=true \
  -l traefik.http.routers.nextcloud.tls.certresolver=joern19 \
  -l traefik.http.services.nextcloud.loadbalancer.server.port=80 \
  '-l "traefik.http.routers.nextcloud.rule=Host(\"nextcloud.l.joern19.de\")"' \
  -e NEXTCLOUD_TRUSTED_DOMAINS=nextcloud.l.joern19.de \
  -e OVERWRITEPROTOCOL=https \
  -e TRUSTED_PROXIES=0.0.0.0/0 \
  -e SQLITE_DATABASE=nextcloudsqlite \
  -e REDIS_HOST=valkey \
  -e REDIS_PORT=6379 \
  -e PHP_UPLOAD_LIMIT=100G \
  -e APACHE_BODY_LIMIT=107374182400 \
  -e PHP_MEMORY_LIMIT=512M \
  -e PHP_MAX_TIME=3600 \
  -e NEXTCLOUD_ADMIN_USER=admin \
  --secret "nextcloud,type=env,target=NEXTCLOUD_ADMIN_PASSWORD" \
  -e TZ=Europe/Berlin \
  --shm-size 134217728 --cap-drop NET_RAW \
  docker.io/nextcloud:34.0-apache

