import logging
import requests
from mqtt_client import create_client

logging.basicConfig(level=logging.INFO)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        logging.info("Connected, subscribing to 'alarm'")
        client.subscribe("alarm")
    else:
        logging.error(f"Connection failed with code {rc}")

def on_message(client, userdata, msg):
    logging.info(f"[{msg.topic}] {msg.payload.decode()}")
    requests.post('http://192.168.178.77/relay/0?turn=on&timer=240')

client = create_client("alarm-logger", on_connect=on_connect, on_message=on_message)
client.loop_forever()
