#!/usr/bin/python3

# A little bit messy, but it works. Now shut up

import configparser
import os
from pathlib import Path
import shutil
from distutils import util
import argparse
from enum import Enum
import filecmp

def createFileName(fileName):
  counter = 1
  newName = Path(fileName + ".old")
  if not newName.exists():
    return newName
  while True:
    countingName = Path(fileName + ".old(" + str(counter) + ")")
    if not countingName.exists():
      return countingName
    counter += 1

class USER_ANSWER(Enum):
  UNSET = lambda file : None
  SKIP = lambda file : False
  REPLACE = lambda file : True
  MOVE = lambda file : os.rename(file, createFileName(str(file))) or True

parser = argparse.ArgumentParser()

parser.add_argument('-s', '--store', help="Copy the configuration files into the backup folder", action='store_true')
parser.add_argument('-i', '--install', help="Copy the configuration files into the backup folder", action='store_true')
parser.add_argument("-c", "--configListFile", help="The ini file, where the config files are listed", default="configList.ini")
parser.add_argument("-b", "--backupFolder", help="The relative path to the folder, where the config files are safed.", default="configs")

args = parser.parse_args()

backupFolder = Path(Path(__file__).parent.absolute(), args.backupFolder)
configListFile = Path(args.configListFile)

if not configListFile.is_file():
  print("Could not find: " + str(configListFile.absolute()))
  exit()

if (args.store and args.install) or (not args.store and not args.install): # -s or -i has to be specified. Otherwise there is nothing to do
  print("You have to specify --store or --install")
  exit()


fileConfiguration = configparser.ConfigParser()
fileConfiguration.read("configList.ini")


def YESno(question): # ask question with default answer yes
  while True:
    try:
      resp = input(question)
      if resp == "":
        return True # default is yes
      else:
        return util.strtobool(resp)
    except ValueError:
      print("Please respond with 'y' or 'n'.\n")

def delete(path):
  if Path(path).is_dir():
    shutil.rmtree(path, ignore_errors=True)
  else:
    try:
      os.remove(path)
    except OSError:
      pass
  
def copy(src, dst):
  if Path(src).is_dir():
    shutil.copytree(src, dst, ignore_dangling_symlinks=True)
  else:
    shutil.copyfile(src, dst)

def checkPreRequirementsInstall():
  if not backupFolder.is_dir() or not any(backupFolder.iterdir()):
    print("Backup folder does not exists, or is empty: " + str(backupFolder))
    exit() 

def checkPreRequirementsBackup():
  # TODO: only ask, if new files will be created.
  if backupFolder.is_dir() and any(backupFolder.iterdir()) and YESno("Clear current backup(" + str(backupFolder) + ")? (You should have checked it in anyway) [Y/n] "): # Ask if backupfolder should be deleted, if it exists.
    shutil.rmtree(backupFolder)
  os.makedirs(backupFolder, exist_ok=True)

userAllAnswer = USER_ANSWER.UNSET
def alreadyExists(path):
  global userAllAnswer
  if not userAllAnswer == USER_ANSWER.UNSET:
    return userAllAnswer(path)
  while True:
    print("The file or folder \"" + str(path) + "\" does already exists.")
    resp = input("Should it be [r]eplaced, [s]kipped or [m]oved? (Use uppercase to answer for all): ")
    switch = {
      "r": USER_ANSWER.REPLACE,
      "s": USER_ANSWER.SKIP,
      "m": USER_ANSWER.MOVE,
    }
    if not resp.lower() in switch:
      continue
    if resp.isupper():
      userAllAnswer = switch[resp.lower()]
    return switch[resp.lower()](path)

def safeCopy(src : Path, dst : Path):
  if not dst.exists(): # we are safe.
    copy(src, dst)
    return
  
  if src.is_file() ^ dst.is_file(): # they are not the same type
    if alreadyExists(dst):
      delete(dst)
      copy(src, dst)
    return  

  if src.is_file():
    if not filecmp.cmp(src, dst):
      if alreadyExists(dst):
        delete(dst)
        copy(src, dst)
    return

  # src and dst are an directory

  files = []
  for (dirpath, dirnames, filenames) in os.walk(src):
    for filename in filenames:
      files.append(Path(dirpath, filename).relative_to(src))

  for file in files:
    absoluteSrc = Path(src, file)
    absoluteDst = Path(dst, file)

    if absoluteDst.exists() and not filecmp.cmp(absoluteSrc, absoluteDst):
      if alreadyExists():
        delete(dst)
        copy(src, dst)

def backupConfigs():
  for section in fileConfiguration._sections: # folder is the absolute path
    sourceCategoryPath = Path(section).expanduser().absolute()
    targetCategoryName = list(fileConfiguration._sections[section])[0]
    configs = str(fileConfiguration._sections[section][targetCategoryName]).split(",")

    absolutePathCurrentBackup = Path(backupFolder, targetCategoryName)

    os.makedirs(absolutePathCurrentBackup, exist_ok=True)

    size = len(configs)
    counter = 0
    for config in configs:
      counter+=1
      print("Current catagory: " + targetCategoryName + " (" + str(sourceCategoryPath) + ")     ", end="")
      print(str(counter) + "/" + str(size) + " current: " + config + "                   ", end="\r")
      toBackUp = Path(sourceCategoryPath, config)
      target = Path(absolutePathCurrentBackup, config)
      safeCopy(toBackUp, target)
    print()

  print("\nDone. You can now commit and push your changes.")

def installConfigs():
  for section in fileConfiguration._sections:
    targetFolder = Path(section).expanduser().absolute()
    categoryName = list(fileConfiguration._sections[section])[0]
    configs = str(fileConfiguration._sections[section][categoryName]).split(",")

    absolutePathBackup = Path(backupFolder, categoryName)

    size = len(configs)
    counter = 0
    for config in configs:
      counter+=1
      print("Current catagory: " + categoryName + " (" + str(targetFolder) + ")     ", end="")
      print(str(counter) + "/" + str(size) + " current: " + config + "                   ", end="\r")
      backUp = Path(absolutePathBackup, config)
      target = Path(targetFolder, config)
      safeCopy(backUp, target)
    print()
  print("\nDone. Your configuration should be restored")

if args.install:
  checkPreRequirementsInstall()
  installConfigs()
else:
  checkPreRequirementsBackup()
  backupConfigs()
