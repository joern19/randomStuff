FROM docker.io/selenium/standalone-firefox:136.0-geckodriver-0.36-20250808
USER root
RUN apt update && apt install -y python3-selenium && rm -rf /var/lib/apt/lists/*

#RUN pip install selenium
#
#RUN apt update && apt install -y \
#    firefox-esr wget bzip2 libxtst6 libgtk-3-0 libx11-xcb-dev libdbus-glib-1-2 libxt6 libpci-dev \
#    && rm -rf /var/lib/apt/lists/*

# ADD https://github.com/mozilla/geckodriver/releases/download/v0.36.0/geckodriver-v0.36.0-linux-aarch64.tar.gz /usr/bin/
# RUN cd /usr/bin/ && tar -xzvf geckodriver-v0.36.0-linux-aarch64.tar.gz
#ADD https://github.com/mozilla/geckodriver/releases/download/v0.36.0/geckodriver-v0.36.0-linux64.tar.gz /usr/bin/
#RUN cd /usr/bin/ && tar -xzvf geckodriver-v0.36.0-linux64.tar.gz
#RUN chmod +x /usr/bin/geckodriver

#ENV DISPLAY=:99

USER seluser
WORKDIR /app
COPY main.py .
VOLUME /seluser/autoWhatsapp.json
VOLUME /seluser/.mozilla/firefox/autowhatsapp.default-release/
CMD ["/usr/bin/python3", "main.py"]
