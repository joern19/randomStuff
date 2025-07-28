import sys
import requests

tifFile = sys.argv[1]
print("Parsing: " + tifFile)

import pytesseract
from PIL import Image

img = Image.open(tifFile)
text = pytesseract.image_to_string(img, lang='deu')

with open("/webhookUrl.txt") as webhookUrlFile:
    webhookUrl = webhookUrlFile.readlines()[0].strip()
    requests.post(webhookUrl, json={'content': text})

