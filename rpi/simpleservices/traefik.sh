#!/bin/sh

./.generic.sh \
  -v /home/pi/acme.json:/acme.json \
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
  --secret "cloudflareJoern19ApiToken,type=env,target=CLOUDFLARE_DNS_API_TOKEN" \
  --secret "cloudflareJoern19ApiToken,type=env,target=CLOUDFLARE_ZONE_API_TOKEN" \
  docker.io/traefik:v3.6.7 traefik --api=true --entrypoints.web.address=:80 \
  --entrypoints.websecure.address=:443 --entrypoints.websecure.http.tls=true --certificatesresolvers.joern19.acme.email=acme@joern19.de --certificatesresolvers.joern19.acme.storage=acme.json \
  --certificatesresolvers.joern19.acme.dnsChallenge=true --certificatesresolvers.joern19.acme.dnsChallenge.provider=cloudflare \
  --providers.docker=true --providers.docker.exposedbydefault=false --providers.docker.network=proxy

