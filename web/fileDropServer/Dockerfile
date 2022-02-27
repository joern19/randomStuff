FROM golang:latest as upload-server

WORKDIR /usr/src/app
COPY upload.go main.go
RUN go build -o /usr/src/app/main main.go


FROM nginx:latest

COPY nginx.conf /etc/nginx/conf.d/default.conf
RUN mkdir -p /usr/share/nginx/html/fileServer
COPY upload.html /usr/share/nginx/html/fileServer/upload.html
VOLUME [ "/usr/share/nginx/html/fileServer/" ]

RUN openssl req -batch -newkey rsa:4096 -x509 -sha256 -days 365 -nodes -out /etc/nginx/ssl.crt -keyout /etc/nginx/ssl.key

COPY --from=upload-server /usr/src/app/main /usr/bin/upload

RUN echo "upload & \n nginx -g \"daemon off;\"" > /start.sh && chmod +x start.sh
CMD [ "/start.sh" ]