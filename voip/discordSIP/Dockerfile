# You can open this folder in a vscode with the "Dev Containers" Extension.
FROM alpine:latest

WORKDIR /home

ENV PJSIP_VERSION=2.13.1

RUN apk update && apk add build-base openssl wget tar swig openjdk17 linux-headers
RUN wget https://github.com/pjsip/pjproject/archive/refs/tags/${PJSIP_VERSION}.tar.gz
RUN tar -xzf ${PJSIP_VERSION}.tar.gz
WORKDIR /home/pjproject-${PJSIP_VERSION}

COPY pjproject/config_site.h pjlib/include/pj/config_site.h
#COPY pjproject/media.hpp pjlib/include/pjsua2/media.hpp
#COPY pjproject/media.cpp pjlib/src/pjsua2/media.cpp
#COPY pjproject/pjsua2.i pjlib/src/swig/pjsua2.i

RUN ./configure CFLAGS="-fPIC"
RUN make dep && make && make install

RUN cd pjsip-apps/src/swig/java && make && make install

# TODO: Copy *.java and .so file! These are the only Files needed from the build.

ENTRYPOINT [ "python" ]
