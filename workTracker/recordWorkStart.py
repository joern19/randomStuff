#!/bin/python3

import subprocess
import sqlite3
import datetime
import os
import sys

NOW = datetime.datetime.now().astimezone()

class Db:
  def formatDatetime(self, datetime): # For sqlite3
    return datetime.astimezone().replace(microsecond=0).isoformat()
  def __init__(self):
    self.con = sqlite3.connect(os.path.expanduser('~/workTimes.db'), detect_types=sqlite3.PARSE_DECLTYPES|sqlite3.PARSE_COLNAMES)
    self.cursor = self.con.cursor()
    self.cursor.execute("CREATE TABLE IF NOT EXISTS workStart (id INTEGER PRIMARY KEY AUTOINCREMENT, timestamp TEXT NOT NULL, place TEXT)")
  def insertWorkStart(self, place):
    self.cursor.execute("INSERT INTO workStart(timestamp, place) VALUES (?, ?)", (self.formatDatetime(NOW), place))
  def __del__(self):
    self.con.commit()
    self.con.close()

def formatDatetime(datetime): # For humans
  return datetime.astimezone().strftime("%d.%m.%y %H:%M")

def fail(message):
  subprocess.run(["swaynag", "-t", "error", "-m", message])
  sys.exit(1)

if __name__ == "__main__":
  db = Db()
  place = sys.argv[1].lower()
  db.insertWorkStart(place)
