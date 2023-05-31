# docker build . -t pjsua2-python
# docker run -it -v ./app:/app -p 56447:56447/udp --entrypoint /bin/sh pjsua2-python
# Inside the container, cd /app and start the app with python3 sip.py
FROM alpine:latest

WORKDIR /home

ENV PJSIP_VERSION=2.13

RUN apk update && apk add build-base openssl wget tar swig python3-dev linux-headers
RUN wget https://github.com/pjsip/pjproject/archive/refs/tags/${PJSIP_VERSION}.tar.gz
RUN tar -xzf ${PJSIP_VERSION}.tar.gz
WORKDIR /home/pjproject-${PJSIP_VERSION}

COPY pjproject/config_site.h pjlib/include/pj/config_site.h

RUN ./configure CFLAGS="-fPIC"
RUN make dep && make && make install

RUN cd pjsip-apps/src/swig/python && make && make install

ENTRYPOINT [ "python" ]
