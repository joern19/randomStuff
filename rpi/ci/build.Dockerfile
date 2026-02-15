FROM docker.io/library/alpine:3.23

LABEL description="Alpine with a bunch of tools concourse needs to deploy basic stuff"

RUN apk add openssh helm