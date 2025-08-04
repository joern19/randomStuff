import os
import ssl
import logging
import paho.mqtt.client as mqtt

def create_client(client_id, on_connect=None, on_message=None):
    hostname = os.environ.get("MQTT_HOSTNAME")
    username = os.environ.get("MQTT_USERNAME")
    password = os.environ.get("MQTT_PASSWORD")

    if not username or not password or not hostname:
        raise EnvironmentError("username, password and hostname must be set.")

    client = mqtt.Client(client_id=client_id, transport="websockets")
    client.username_pw_set(username, password)
    client.tls_set(cert_reqs=ssl.CERT_REQUIRED)
    client.tls_insecure_set(False)

    client.on_message = on_message
    client.on_connect = on_connect

    client.connect(hostname, 443)
    return client

