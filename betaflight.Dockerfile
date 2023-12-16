# syntax=docker/dockerfile:1.5-labs
FROM node:16

WORKDIR /app
ADD --keep-git-dir=true https://github.com/betaflight/betaflight-configurator.git .

RUN yarn install

EXPOSE 8080

CMD ["yarn", "dev", "--host", "0.0.0.0"]
