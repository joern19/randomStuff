from datetime import datetime, timedelta
from dateutil import parser
import time
import tkinter as tk
from tkinter import Tk
from tkinter import Entry
from tkinter import Label

then = datetime.now()
#then = parser.parse("10 27 2020 0:00AM", fuzzy=True)
then = datetime.now() + timedelta(hours=2)
print(then)


root = Tk()
#root.attributes('-fullscreen', True)
var = tk.StringVar()
clock = tk.Label(root, textvariable=var, height=0, width=10, font=("Courier", 70), bg="black", fg="white")
clock.place(x=25, y=25, anchor="center")
clock.pack()


sw = True
def blink():
    global sw
    if (sw):
        clock.config(fg="red")
    else:
        clock.config(fg="white")
    sw = not sw
    root.after(250, blink)


def getString():
    now = datetime.now()
    diff = then - now
    realDiff = divmod(diff.days * 86400 + diff.seconds, 60)    
    hours = int(realDiff[0] / 60)
    minutes = int(realDiff[0] - hours * 60)
    if (datetime.now() > then):
        blink()
        return "DONE!!!!"
    microseconds = str(int(diff.microseconds / 100000))
    return (str(hours) + ":" + str(minutes) + ":" + str(realDiff[1]) + "." + microseconds)

def doIt():
    a = getString()
    var.set(a)
    if (a == "DONE!!!!"):
        return
    root.after(100, doIt)


doIt()

root.mainloop()



'''while (True):
    a = getString()
    if (a == None):
        break
    print(a)
    time.sleep(0.1)

print("Yay")'''