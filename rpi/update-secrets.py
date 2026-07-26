#!/bin/python3

import subprocess
import io

# secretename ; pass path
secrets = {
  "nextcloud": "other/nextcloud.l.joern19.de",
  "mqttDiscordWebhook": "other/mqttDiscordWebhook",
  "cloudflareJoern19ApiToken": "other/cloudflare/joern19/apiToken",
}

def getPassword(secretname):
  passpath = secrets[secretname]
  result = subprocess.run(["pass", passpath], capture_output=True)
  result.check_returncode()
  return result.stdout.decode("utf-8").rstrip('\n')

def upload(secretname, secret):
  result = subprocess.run(["ssh", "pi@kaboom.l.joern19.de", f"podman secret create --replace=true {secretname} -"], input=bytes(secret, 'utf-8'), capture_output=True)
  result.check_returncode()

if __name__ == "__main__":
  for secretname in secrets:
    print(f"Fetching and uploading secret '{secretname}'", end=" ", flush=True)
    password = getPassword(secretname)
    print(f"[FETCHED]", end=" ", flush=True)
    upload(secretname, password)
    print("[UPLOADED]", flush=True)
