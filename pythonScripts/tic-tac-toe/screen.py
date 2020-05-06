from tkinter import *
from logic import logicClass as logic
import threading
import time
import ctypes

master = Tk()
logicC = logic()

w = Canvas(master, width=300, height=300)
w.pack()
w.configure(background='white')

gameover = None

def createCircle(h, v):
    w.create_oval(5 + (h * 100), 95  + (v * 100), 95  + (h * 100), 5  + (v * 100), fill="white", outline="#000", width=4)

def createX(h, v):
    w.create_line(95 + (h * 100), 95 + (v * 100), 5 + (h * 100), 5 + (v * 100), fill="#000000", width=3)
    w.create_line(95 + (h * 100), 5 + (v * 100), 5 + (h * 100), 95 + (v * 100), fill="#000000", width=3)

def switch_title():
    one = "X ist dran"
    two = "O ist dran"
    if master.title() == one:
        master.title(two)
        return False
    else:
        master.title(one)
        return True

placed = 9

def close():
    master.destroy()
    close()

def callback(event):
    global gameover
    global placed
    if (gameover != None):
        master.destroy()
        return
    h = None
    v = None
    if (event.x > 200):
        h = 2
    elif (event.x > 100):
        h = 1
    else:
        h = 0
    if (event.y > 200):
        v = 2
    elif (event.y > 100):
        v = 1
    else:
        v = 0
    player = 1
    if (master.title() == "O ist dran"):
        player = 2
    result = logicC.place(h,v,player)
    if (result == True):
        placed -= 1
        if (placed == 0):
            gameover = "nobody won!"
            thr = threading.Thread(target=ctypes.windll.user32.MessageBoxW, args=(0, gameover, "GameOver", 1), kwargs={})
            thr.start()
        if (switch_title()):
            createCircle(h,v)
        else:
            createX(h,v)
    elif (result == None):
        if (switch_title()):
            createCircle(h,v)
        else:
            createX(h,v)
        if (player == 1):
            gameover = "X won!"
        else:
            gameover = "O won!"
        thr = threading.Thread(target=ctypes.windll.user32.MessageBoxW, args=(0, gameover, "GameOver", 1), kwargs={})
        thr.start()

w.bind("<Button-1>", callback)
switch_title()

w.create_line(100, 0, 100, 300, fill="#000000", width=3)
w.create_line(200, 0, 200, 300, fill="#000000", width=3)

w.create_line(0, 100, 300, 100, fill="#000000", width=3)
w.create_line(0, 200, 300, 200, fill="#000000", width=3)

mainloop()