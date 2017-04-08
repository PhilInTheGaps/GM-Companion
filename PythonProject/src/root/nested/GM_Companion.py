'''
Created on 08.04.2017

@author: Phil
'''

from tkinter import *
import ctypes

class App:

    def __init__(self, master):
        pw = PanedWindow(orient="horizontal")
        pw.pack(fill="both", expand=True)
        
        sidebar = PanedWindow(pw, orient="vertical")
        pw.add(sidebar)
        
        main = Frame(pw, width=1280, height=720, background="grey")
        pw.add(main)
        
        musicButtonsLabel = Label(sidebar, text="Music")
        sidebar.add(musicButtonsLabel)
        
        musicButtons = Frame(sidebar, width=200, height=200, background="white")
        sidebar.add(musicButtons)
        
        musicButtonPlay = Button(musicButtons, text="Play").pack(side=LEFT)
        musicButtonPause = Button(musicButtons, text="Play").pack(side=LEFT)
        musicButtonReplay = Button(musicButtons, text="Play").pack(side=LEFT)
        musicButtonNext = Button(musicButtons, text="Play").pack(side=LEFT)
        
        soundButtonsLabel = Label(sidebar, text="Sound")
        sidebar.add(soundButtonsLabel)
        
        soundButtons = Frame(sidebar, width=200, height=200, background="white")
        sidebar.add(soundButtons)
        
        soundButtonPlay = Button(soundButtons, text="Play").pack(side=LEFT)
        soundButtonPause = Button(soundButtons, text="Play").pack(side=LEFT)
        soundButtonReplay = Button(soundButtons, text="Play").pack(side=LEFT)
        soundButtonNext = Button(soundButtons, text="Play").pack(side=LEFT)
        
        sidebar.add(Label())
        
        # Creating Menu Bar
        menubar = Menu(root)
        menubar.add_command(label="GM-Help", command=self.say_hi)
        menubar.add_command(label="Music", command=self.say_hi)
        menubar.add_command(label="Sounds", command=self.say_hi)
        menubar.add_command(label="Maps", command=self.say_hi)
        menubar.add_command(label="Options", command=self.say_hi)
        
        # display the menu
        root.config(menu=menubar)

    def say_hi(self):
        print("Hello there!")

root = Tk()
#root.geometry("1280x720+0+0")
root.title("GM-Companion | Version: Beta 3.0 PRE-RELEASE (0.3.0.0)")
root.iconbitmap(default="resources/icon256.ico")
myappid = "GM-Companion (Python)"
ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)
app = App(root)

root.mainloop()
