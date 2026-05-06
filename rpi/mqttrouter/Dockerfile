FROM docker.io/library/golang:1.26-alpine

COPY . .
RUN go build
CMD ["./mqttrouter"]
