FROM freeswitch
# Build it yourself, by cloning the gh freeswitch repo and building the example dockerfile

RUN apt install -y python3-pip tesseract-ocr-deu
RUN pip3 install pytesseract requests paho-mqtt

ENTRYPOINT ["/bin/sh", "-c"]
CMD ["nohup python3 /usr/local/freeswitch/conf/mqtt/sender.py & /usr/local/freeswitch/bin/freeswitch"]

