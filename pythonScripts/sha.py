import hashlib
BLOCKSIZE = 65536
hasher = hashlib.md5()
inp = input("(z.b. 256/1) sha-")
if (inp == "256"):
  hasher = hashlib.sha256()
elif (inp == "1"):
  hasher = hashlib.sha1()

name = input("Please enter the Path to/of the file(You can grap&drop it):")
name = name.replace('"', '')
try:
    with open(name, 'rb') as afile:
        buf = afile.read(BLOCKSIZE)
        while len(buf) > 0:
            hasher.update(buf)
            buf = afile.read(BLOCKSIZE)
except:
    print("You have enterd a wrong Path!")
    input("Press Enter to exit!")
    exit()
ha = hasher.hexdigest()
print(ha)
if (ha == input("Enter the correct sha String to compare it:").replace(" ", "")):
  print("it is equal!")
else:
  print("its not!! Delete it!")
input("Press Enter to exit!")
