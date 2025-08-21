FROM docker.io/selenium/standalone-chromium:139.0-chromedriver-139.0
USER root
RUN apt update && apt install -y python3-selenium && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY main.py .
CMD ["/usr/bin/python3", "main.py"]
