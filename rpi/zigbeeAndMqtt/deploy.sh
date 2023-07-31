TARGET=pi4

ssh $TARGET 'rm -r /tmp/zigbeeAndMqttServer'
scp -r . $TARGET:/tmp/zigbeeAndMqttServer
ssh $TARGET 'cd /tmp/zigbeeAndMqttServer && docker build . -t zigbee-mqtt:latest'
ssh $TARGET 'docker rm -f zigbee-server && docker run -d --restart unless-stopped --name zigbee-server zigbee-server'
