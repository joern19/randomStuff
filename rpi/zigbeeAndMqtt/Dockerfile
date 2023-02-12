FROM golang:alpine AS build

RUN apk update && apk add --no-cache git
WORKDIR $GOPATH/myapp/
COPY . .
RUN go get -d -v
RUN go build -o /go/bin/zigbeeAndMqtt

FROM scratch
COPY --from=build /go/bin/zigbeeAndMqtt /zigbeeAndMqtt
ENTRYPOINT ["/zigbeeAndMqtt"]
