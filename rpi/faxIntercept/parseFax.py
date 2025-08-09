import sys
import requests
import os
import time
import pytesseract
import re
import json
from urllib.parse import quote
from dataclasses import dataclass
from PIL import Image

tifFile = sys.argv[1]
print("Parsing: " + tifFile)

def getSizeOr0():
  try:
    return os.path.getsize(tifFile)
  except FileNotFoundError:
    return 0

def waitForPage():
  # 1000 * 0.2 / 60 = About 3 minutes. Enough time to get the first page!
  limit = 1000
  while getSizeOr0() < 1000 or limit <= 0:
    limit -= 1
    if limit <= 0:
      print("Timeout waiting for first page. continue anyway")
      return
    time.sleep(0.2)
  print("Looks like the first page was recieved.")

def triggerDiscord(text):
  with open("/webhookUrl.txt") as webhookUrlFile:
    webhookUrl = webhookUrlFile.readlines()[0].strip()
    requests.post(webhookUrl, json={'content': text})

def ocr():
  return pytesseract.image_to_string(Image.open(tifFile), lang='deu')

def encode(text):
  print("encoding: " + text)
  return quote(text, safe="")

@dataclass
class Details:
  sonderrechte: bool
  meldebild: str
  bemerkung: str
  str: str
  maps: str
  objekt: str
  ortsteil: str

def triggerDivera(details: Details):
  group = os.environ.get("DIVERA_GROUP")
  key = os.environ.get("DIVERA_ACCESS_KEY")
  body = {'Alarm': {
    'title': details.meldebild,
    'text': details.bemerkung,
    'group': int(group),
    'notification_type': 3,
    'destination': True,
    'destination_address': details.str,
    'additional_text_1': details.maps,
    'priority': details.sonderrechte,
    'additional_text_2': details.ortsteil,
    'additional_text_3': details.objekt
  }}
  print(json.dumps(body))
  print("Response:")
  # https://api.divera247.com/?urls.primaryName=api%2Fv2%2Falarm
  print(requests.post('https://app.divera245.com/api/v2/alarms', json=body).__dict__)

def get(key, text):
  exp = "(?<=" + key + ").*"
  result = re.search(exp, text)
  if result is None:
    return ""
  return result.group(0).strip()

def extractDetails(text):
  street = get("Straße", text)
  encoded = encode(street + " " + get("Ort", text))
  mapsUrl = "https://www.google.com/maps/search/?api=1&query=" + encoded
  return Details(
    sonderrechte=("Mit Sondersignal" in text),
    meldebild=get("Meldebild", text),
    bemerkung=get("Bemerkung", text),
    str=street,
    maps=mapsUrl,
    objekt=get("Objekt", text),
    ortsteil=get("Ortsteil", text)
  )

waitForPage()
text = ocr()
details = extractDetails(text)

triggerDiscord(text)

if "SA_Davenstedt" in text:
  print("Found SA_Davenstedt, everyone will be called.")
  triggerDiscord(str(details))
  triggerDivera(details)
else:
  print("Probably alarm for the AB-Hygene. We *could* check and send an alarm..")
  triggerDiscord("SA_Davenstedt NOT FOUND!\n" + str(details))

