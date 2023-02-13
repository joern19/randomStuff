FROM golang:alpine AS build

RUN apk update && apk add --no-cache git
WORKDIR $GOPATH/app/
COPY . .
RUN go get -d -v
RUN go build -o /server

FROM scratch
COPY --from=build /server /server
ENTRYPOINT ["/server"]
