import time 
import os.path
import yaml
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import requests
from requests.utils import requote_uri

class Configuration():
  def __init__(self, filePath):
    with open(filePath) as configFile:
      root = yaml.load(configFile, Loader=yaml.FullLoader)
      self.folderPath = root["rootFilePath"]
      self.url = root["url"]
      self.method = root["requestMesthod"]

configuration = Configuration(os.path.dirname(os.path.realpath(__file__)) + "/folderDetectConfig.yaml")
print("Listening on: " + configuration.folderPath)


def callUrl(config, dateId, emailId):
  url = config.url
  url = url.replace("{emailID}", requote_uri(emailId)) # requote_uri converts the string to an url. We dont need it but it is a nice to have thing.
  url = url.replace("{dateID}", requote_uri(dateId))

  method = config.method.lower()
  response = getattr(requests, method)(url) # execute the request using the method specified in the config.
  if not response.status_code == 200:
    print("'" + method.upper() + "' request to '" + url + "' failed with status code: " + str(response.status_code)) 

class FileEventHandler(FileSystemEventHandler):
  def on_created(self, event):
    if not event.is_directory:
      return # A file was created. cancel
    
    relativePath = os.path.relpath(os.path.normpath(event.src_path), configuration.folderPath)
    splittedPath = os.path.split(relativePath)
    if splittedPath[0] == "":
      return # A folder was created one level to high. cancel
    if "/" in splittedPath[0]:
      return # A folder was created too deep. cancel

    dateId = splittedPath[0]
    emailId = splittedPath[1]
    print("Detected email creation with id: " + emailId + " The id of parent folder(dateId) is: " + dateId)
    callUrl(configuration, dateId, emailId)

observer = Observer()
event_handler = FileEventHandler()
observer.schedule(event_handler, path=configuration.folderPath, recursive=True)
observer.start()

try:
    while True:
      time.sleep(100)
except KeyboardInterrupt:
    observer.stop()
