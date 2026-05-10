#!/bin/sh

./.generic.sh \
  --security-opt no-new-privileges:true \
  -l traefik.enable=true \
  '-l "traefik.http.routers.myapi.rule=Host(\"kaboom.l.joern19.de\") && PathPrefix(\"/api\")"' \
  -l traefik.http.routers.myapi.service=api@internal \
  -l traefik.http.routers.myapi.entrypoints=web,websecure \
  '-l "traefik.http.routers.mydashboard.rule=Host(\"kaboom.l.joern19.de\") && PathPrefix(\"/\")"' \
  -l traefik.http.routers.mydashboard.service=dashboard@internal \
  -l traefik.http.routers.mydashboard.entrypoints=web,websecure \
  -p 80:80 -p 443:443 \
  -v /run/user/1000/podman/podman.sock:/var/run/docker.sock:z \
  docker.io/traefik:v3.6.7 traefik --api=true --entrypoints.web.address=:80 --entrypoints.websecure.address=:443 --entrypoints.websecure.http.tls=true --providers.docker=true --providers.docker.exposedbydefault=false --providers.docker.network=proxy

