from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.firefox.service import Service
from selenium.webdriver.firefox.options import Options
import os
import time
import json

with open(os.environ['HOME'] + "/autoWhatsapp.json", "r", encoding="utf-8") as f:
    config = json.load(f)

# --- Setup Firefox + Geckodriver ---
options = Options()
options.set_preference("dom.webnotifications.enabled", False)
options.add_argument("-profile")
options.add_argument(os.environ['HOME'] + "/.mozilla/firefox/autowhatsapp.default-release")
options.add_argument("--headless")
service = Service("/usr/bin/geckodriver", log_path="geckodriver.log", service_log_path="service-geckodriver.log")
driver = webdriver.Firefox(service=service, options=options)
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

print("Message sent ✅")
