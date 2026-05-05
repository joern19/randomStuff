import os
import ssl
import logging
import paho.mqtt.client as mqtt
import subprocess
import json

logging.basicConfig(level=logging.INFO)

def create_client(client_id, on_connect=None, on_message=None):
    hostname = os.getenv("MQTT_HOSTNAME", "kaboom.l.joern19.de")

    client = mqtt.Client()
    client.on_message = on_message
    client.on_connect = on_connect

    client.connect(hostname, 1883, 60)
    return client

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        logging.info("Connected, subscribing to 'rcswitch'")
        client.subscribe("rcswitch")
    else:
        logging.error(f"Connection failed with code {rc}")

def send443(group, device, state):
    subprocess.call(["./send443", str(group), str(device), state])

def on_message(client, userdata, msg):
    body = msg.payload.decode()
    logging.info(f"[{msg.topic}] {body}")
    try:
        parsed = json.loads(body)
        group = parsed["group"]
        device = parsed["device"]
        state = parsed["state"]
        logging.info(f"Sending {state} to {group}/{device}")
        send443(group, device, state)
    except Exception as e:
        logging.error(f"Something failed while handling message '{body}': {e}")

client = create_client("mqtt2rcswitch", on_connect=on_connect, on_message=on_message)
client.loop_forever()

