from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
import os
import time
import json

with open(os.environ['HOME'] + "/autoWhatsapp.json", "r", encoding="utf-8") as f:
    config = json.load(f)

options = Options()
options.add_argument("--disable-gpu")
options.add_argument("--disable-software-rasterizer")
options.add_argument("--disable-extensions")
options.add_argument("--disable-translate")
options.add_argument("--headless=new")
options.add_argument("--window-size=1280,800")
options.add_argument(
    "--user-agent=Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
    "(KHTML, like Gecko) Chrome/116.0.5845.96 Safari/537.36"
)

options.add_argument("--user-data-dir=" + os.environ['HOME'] + "/autowhatsapp-chromium-user-dir")
options.add_argument("--no-sandbox")
options.add_argument("--disable-dev-shm-usage")
service = Service("/usr/bin/chromedriver")
driver = webdriver.Chrome(service=service, options=options)
wait = WebDriverWait(driver, 10 * 60)

def waitFor(xpath: str):
    print("Waiting for: " + xpath)
    return wait.until(
        EC.presence_of_element_located((By.XPATH, xpath))
    )

def clickAfterWait(xpath: str):
    waitFor(xpath).click()

# --- Open WhatsApp Web ---
driver.get("https://web.whatsapp.com")
print("Logging in. You may have to scan the QR code.")
search_box = waitFor('//div[@contenteditable="true"][@data-tab="3"]')
print("Login successful.")
time.sleep(1)

def writeInto(box, text):
    box.click()
    input = driver.switch_to.active_element
    input.clear()
    for char in text:
        input.send_keys(char)
    time.sleep(0.5)

chat_name = config['chat_name']
writeInto(search_box, chat_name + Keys.ENTER)
time.sleep(0.5)

# todo: confirm we are in the correct chat
waitFor(f'//div[@id="main"]//span[normalize-space(text())="{chat_name}"]')

clickAfterWait('//button[@title="Attach"]')
clickAfterWait('//span[normalize-space(text())="Poll"]')

question_box = waitFor('//span[normalize-space(text())="Ask question"]/..')
writeInto(question_box, config['question'])

def nextFreeOption():
    return driver.find_elements(By.XPATH, f'//span[normalize-space(text())="Add"]/..')[0]

for option in config['options']:
    writeInto(nextFreeOption(), option)

time.sleep(3)

clickAfterWait('//div[@aria-label="Send"]')

time.sleep(20) # make sure the message is send or smth

print("Message sent ✅")
