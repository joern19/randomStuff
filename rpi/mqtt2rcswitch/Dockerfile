# --device /dev/gpiomem --group-add keep-groups
FROM debian:12.13

RUN apt update && apt install -y git python3 python3-paho-mqtt g++

ARG WIRINGPI_VERSION=3.18
ADD https://github.com/WiringPi/WiringPi/releases/download/${WIRINGPI_VERSION}/wiringpi_${WIRINGPI_VERSION}_arm64.deb ./wiringpi.deb
RUN apt -y install ./wiringpi.deb

RUN git clone https://github.com/sui77/rc-switch.git
COPY main.cpp rc-switch/

RUN cd rc-switch \
  && g++ -DRPI -O2 -c main.cpp -o main.o \
  && g++ -DRPI -O2 -c RCSwitch.cpp -o RCSwitch.o \
  && g++ -DRPI RCSwitch.o main.o -o main -lwiringPi -lwiringPiDev -lcrypt
RUN mv rc-switch/main ./send443

COPY mqtt_client.py .
CMD ["python3", "mqtt_client.py"]

