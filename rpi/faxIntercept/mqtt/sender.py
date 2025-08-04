import signal
import time
import logging
import os
from datetime import datetime
from mqtt_client import create_client

logging.basicConfig(level=logging.INFO)

def publish_timestamp(signum, frame):
    timestamp = datetime.now().isoformat()
    logging.info(f"SIGUSR1 received, publishing timestamp: {timestamp}")
    client.publish("alarm", timestamp)

client = create_client("alarm-sender", None)
signal.signal(signal.SIGUSR1, publish_timestamp)
logging.info(f"PID: {os.getpid()} – waiting for SIGUSR1...")
client.loop_forever()

