from tkinter import *
from tkinter import font as tkfont
import numpy as np
import serial
import cv2
from functionsProject import *
from test6 import getContours
from test6 import predict
from test6 import trackpuck
import cvzone
from cvzone.ColorModule import ColorFinder
from random import *
import time


hsvVals = {'hmin': 54, 'smin': 133, 'vmin': 59, 'hmax': 151, 'smax': 255, 'vmax': 143}

myColorFinder = ColorFinder(False)



global endPoint
endPoint = 0

global x
global y
global normal
global fans
global cval
global homed
global varE
global varNE
varNE = 0
varE = 0
homed = 1
y = 0
x = 0
normal = 0


cap = cv2.VideoCapture(0)
cap.set(3, int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)))
cap.set(4, int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
cap.set(5, 90)
cap.set(cv2.CAP_PROP_FPS, 120)



    
    

############################################################

def Delete():
    global normal
    normal = 0
    cv2.destroyAllWindows()

############################################################


def modeDef():
    cv2.destroyAllWindows()
    global normal
    normal = 1
    global endPoint
    endPoint = 60
    homed = 1
    Pos = bytes("p,300,50\n", 'UTF-8')
    print(Pos)
    ser.write(Pos)
    
    

def modeAttack():
    cv2.destroyAllWindows()
    global normal
    normal = 2
    global endPoint
    endPoint = 250
    homed = 1
    home()
    
    
###########################################################    


def modeAttackPlus():
    cv2.destroyAllWindows()
    global normal
    normal = 3
    global endPoint
    endPoint = 250
    global time
    time = randint(0, 10)
    homed = 1
    home()


def modeFunction():
    cv2.destroyAllWindows()
    global normal
    normal = 4
   
    def modeLoop():
        global i
        i = 0
        if normal != 0:
            
            while i<4:
                success, img = cap.read()
                
                imgColor, mask = myColorFinder.update(img, hsvVals)
    
                imgContours, contours = cvzone.findContours(img,mask, minArea=600)
                
                i = i + 1
                
                if i == 1:
                    x,y=getContours(mask)
                    f = x
                    g = y
            
            
                if i == 2:
                    x,y=getContours(mask)
                    t = x
                    k = y
                    
        
    
    
                if i == 3:
                    i = 4
                   
                    yFut, deltaXX = trackpuck(imgContours, f, g, t, k, endPoint)
                   
                    
                
                
            
                if normal != 0:
                    cv2.namedWindow("Attack", cv2.WINDOW_NORMAL)
                    cv2.resizeWindow("Attack", 640, 480)
                    cv2.moveWindow("Attack", 640, 500)    
                    cv2.imshow("Attack", imgContours)
                    
                    
                    
                    
                
        
                if cv2.waitKey(1) & 0xFF == ord('q'):
                
                    cv2.destroyAllWindows()
                    break
                
        
        
            app.after(1, modeLoop)
        
    
    
    app.after(1, modeLoop)


#################################################################################################################################################
#################################################################################################################################################   





    
#################################################################################################################################################   
#################################################################################################################################################
        

        
    




        

#################################################################################################################################################   
#################################################################################################################################################

class SampleApp(Tk):

    def __init__(self, *args, **kwargs):
        Tk.__init__(self, *args, **kwargs)
        
        self.title_font = tkfont.Font(family = 'Helvetica', size = 18, weight = 'bold', slant = "italic")



        container = Frame(self)
        container.pack(side = "top", fill = "x", expand = TRUE)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight = 1)

        self.frames = {}
        for F in (StartPage, Settings, Testing, Camera, Game):
            page_name = F.__name__
            frame =F(parent = container, controller =self)
            self.frames[page_name] = frame

            frame.grid(row = 0, column=0, sticky = "nsew")

        self.show_frame("StartPage")
    
    def show_frame(self, page_name):

        frame = self.frames[page_name]
        frame.tkraise()


#################################################################################################################################################   
#################################################################################################################################################


class StartPage(Frame):
   
    def __init__(self, parent, controller):
        
        
        self.hintergrundTest = PhotoImage(file = 'blue.png')
        self.playImage = PhotoImage(file = 'play.png')
            
        
        Frame.__init__(self, parent, bg="#e1f2f2")
        self.controller=controller
        self.controller.geometry("1920x1080")
        self.controller.title('AirHockey')
        

        headingLabel = Label(self, text="Air-Hockey-Robot", font=('bold', 45, 'bold'), bg = "#e1f2f2")
        headingLabel.pack(pady = 25)

        buttonFrame = Frame(self, bg="#e1f2f2")
        buttonFrame.pack()

        buttonFrame2 = Frame(self, bg="#e1f2f2")
        buttonFrame2.pack()

        settingsButton = Button(buttonFrame, text="Settings", image = self.hintergrundTest, command = lambda:controller.show_frame("Settings"),  font=('bold', 20, 'bold'), border = 0, bg = "#e1f2f2", fg = "#e1f2f2", activeforeground = "#e1f2f2", activebackground = "#e1f2f2")
        settingsButton.grid(row = 0, column = 0, padx = 200, pady = 80)

        playButton = Button(buttonFrame, image = self.playImage, text="Game", command = lambda:controller.show_frame("Game"),   font=('bold', 20, 'bold'), border = 0, bg = "#e1f2f2", fg = "#e1f2f2", activeforeground = "#e1f2f2", activebackground = "#e1f2f2")
        playButton.grid(row = 0, column = 1, padx = 10, pady = 80)

        testButton = Button(buttonFrame, text="Test", command = lambda:controller.show_frame("Testing"),  font=('bold', 20, 'bold'), height = 9, width = 17, border = 0, bg = "#e1f2f2", activebackground = "#e1f2f2")
        testButton.grid(row = 0, column = 2, padx = 175, pady = 80)



        camButton = Button(buttonFrame2,text="Camera", command = combine_funcs(lambda:controller.show_frame("Camera"), modeFunction), font=('bold', 20, 'bold'), width = 72, height =10, border = 0, fg = "red", activeforeground ="red",bg = "#e1f2f2",  activebackground = "#e1f2f2")
        camButton.pack(padx = 20, pady = 100)


#################################################################################################################################################   
#################################################################################################################################################
    

class Settings(Frame):
    
    
    def __init__(self, parent, controller):
        Frame.__init__(self, parent, bg = "#e1f2f2")
        self.controller=controller
        
        def cval_function(val=0):
            input1 = str(cval.get())
            input2 = bytes(str(input1), 'UTF-8')
            ser.write(input2)
        
        self.backImage = PhotoImage(file = 'back.png')
        
        
        
        buttonFrame2 = Frame(self, bg="#e1f2f2")
        buttonFrame2.pack(side =TOP, fill = "x")

        headingLabel = Label(buttonFrame2, text="Settings", font=('bold', 45, 'bold'), bg = "#e1f2f2")
        headingLabel.grid(pady = 0, row = 0, column = 1, padx = 600)

        backButton = Button(buttonFrame2, text ="Back",font=('bold', 45, 'bold'),  command = combine_funcs(lambda:controller.show_frame("StartPage"), Delete), image = self.backImage, border = 0, bg = "#e1f2f2", fg = "#e1f2f2", activeforeground = "#e1f2f2", activebackground = "#e1f2f2")
        backButton.grid(row = 0, column = 0, padx = 10, pady = 20)

        buttonFrame1 = Frame(self, bg="#e1f2f2", height = 400)
        buttonFrame1.pack(side = TOP, fill = "x")

        fansButton=Button(buttonFrame1, text = "Fans", relief = "raised", font=('bold', 20, 'bold'), command = fans1,borderwidth=3,  height = 5, width = 20, border = 0, bg = "#e1f2f2", fg = "black", activeforeground = "black", activebackground = "#e1f2f2")
        fansButton.grid(row = 1, column=0, pady = 130, padx = 85)
        

        ledsButton=Button(buttonFrame1, text = "Leds", relief = "raised",  font=('bold', 20, 'bold'),borderwidth=3,height = 5, width = 20, command = leds,border = 0, bg = "#e1f2f2", fg = "black", activeforeground = "black", activebackground = "#e1f2f2")
        ledsButton.grid(row = 1, column=2, padx = 0)

        
        cval=Scale(buttonFrame1, from_=0, to=200, orient=HORIZONTAL, length = 300, width = 50, activebackground = "#e1f2f2", bg="#e1f2f2", command = cval_function)
        cval.grid(row=2, column=0)
        

        dval=Button(buttonFrame1, text = "dval", relief = "raised",  font=('bold', 20, 'bold'),borderwidth=3,height = 5, width = 20, command = dvals,border = 0, bg = "#e1f2f2", fg = "black", activeforeground = "black", activebackground = "#e1f2f2")
        dval.grid(row = 2, column=1, padx = 190)

        mval=Button(buttonFrame1, text = "mval", relief = "raised",  command = mvalFunc, font=('bold', 20, 'bold'),borderwidth=3,height = 5, width = 20,border = 0, bg = "#e1f2f2", fg = "black", activeforeground = "black", activebackground = "#e1f2f2")
        mval.grid(row = 2, column=2)
        

        


class Testing(Frame):

    def __init__(self, parent, controller):
        
        self.backImage = PhotoImage(file = 'back.png')

        
        Frame.__init__(self, parent, bg = "#e1f2f2")
        self.controller=controller
        
        def move():
            input1 = str(MoveButton.get())
            input2 = bytes("p," + input1 + "\n", 'UTF-8')
            ser.write(input2)
        
        buttonFrame2 = Frame(self, bg="#e1f2f2")
        buttonFrame2.pack(side =TOP, fill = "x")

        headingLabel = Label(buttonFrame2, text="Testing", font=('bold', 45, 'bold'), bg = "#e1f2f2")
        headingLabel.grid(pady = 0, row = 0, column = 1, padx = 620)

        backButton = Button(buttonFrame2, text ="Back", font=('bold', 45, 'bold'),  command = combine_funcs(lambda:controller.show_frame("StartPage"), Delete), image = self.backImage, border = 0, bg = "#e1f2f2", fg = "#e1f2f2", activeforeground = "#e1f2f2", activebackground = "#e1f2f2")
        backButton.grid(row = 0, column = 0, padx = 10, pady = 20)
        
        buttonFrame1 = Frame(self, bg="#e1f2f2", height = 400)
        buttonFrame1.pack(side = TOP, fill = "x")

        MoveButton=Entry(buttonFrame1, text = "Move", font=('bold', 20, 'bold'), width = 10)
        MoveButton.grid(row = 0, column = 1, padx = 430, pady = 100)
        
        MoveButton1=Button(buttonFrame1, text ="Move",font=('bold', 20, 'bold'), width = 10, command = move,border = 0, bg = "#e1f2f2", fg = "black", activeforeground = "black", activebackground = "#e1f2f2")
        MoveButton1.grid(row = 0, column = 2)
        
        solButton = Button(buttonFrame1, text="Sol",command = sole, font=('bold', 20, 'bold'),height = 5, width = 10,border = 0, bg = "#e1f2f2", fg = "black", activeforeground = "black", activebackground = "#e1f2f2")
        solButton.grid(row = 1, column = 1)

        DemoButton=Button(buttonFrame1, text = "Demo", font=('bold', 20, 'bold'),height = 5, width = 10, command = demo,border = 0, bg = "#e1f2f2", fg = "black", activeforeground = "black", activebackground = "#e1f2f2")
        DemoButton.grid(row = 1, column = 0, padx = 105, pady = 0)

        HomeButton=Button(buttonFrame1, text = "Home",font=('bold', 20, 'bold'),height = 5, width = 10, command = home,border = 0, bg = "#e1f2f2", fg = "black", activeforeground = "black", activebackground = "#e1f2f2")
        HomeButton.grid(row = 1, column = 2, padx = 70)

        cycleButton=Button(buttonFrame1, text = "Cycle",font=('bold', 20, 'bold'),height = 5, width = 10, command = cycle,border = 0, bg = "#e1f2f2", fg = "black", activeforeground = "black", activebackground = "#e1f2f2")
        cycleButton.grid(row = 2, column = 1, padx = 0, pady = 100)


#################################################################################################################################################   
#################################################################################################################################################
        
        
class Camera(Frame):
    
    def __init__(self, parent, controller):
        Frame.__init__(self, parent, bg = "#e1f2f2")
        self.controller=controller
        
        self.backImage = PhotoImage(file = 'back.png')
       
        
        headingLabel = Label(self, text="Camera", font=('bold', 45, 'bold'), bg = "#e1f2f2")
        headingLabel.pack(padx =320, pady = 25)
        backButton = Button(self, text ="Back", font=('bold', 45, 'bold'),command = combine_funcs(lambda:controller.show_frame("StartPage"), Delete), image = self.backImage, border = 0, bg = "#e1f2f2", fg = "#e1f2f2", activeforeground = "#e1f2f2", activebackground = "#e1f2f2")
        backButton.pack(pady = 10, padx = 10)
        
        buttonFrame1 = Frame(self, bg="#e1f2f2", height = 400)
        buttonFrame1.pack(side = TOP, fill = "x")
   
   
#################################################################################################################################################   
#################################################################################################################################################


class Game(Frame):

    def __init__(self, parent, controller):
        Frame.__init__(self, parent, bg = "#e1f2f2")
        self.controller=controller
        self.backImage = PhotoImage(file = 'back.png')
        self.defendPic = PhotoImage(file = 'DefendPicr.png')
        self.AttackPic = PhotoImage(file = 'AttackPic.png')
        self.StartPic = PhotoImage(file = 'startLogo.png')
        
        
        
       
        def reset1():
            global scoreUser, scoreEnemy
            scoreUser = 0
            scoreEnemy = 0
            global normal
            global varE
            global varNE
            normal = 0
            varNE = 0
            varE = 0
            cv2.destroyAllWindows()       
            label2 = Label(buttonFrame2, text = 0,font=('bold', 100, 'bold'), bg = "#e1f2f2")
            label2.grid(row = 1, column = 0)
            label1 = Label(buttonFrame2, text = 0,font=('bold', 100, 'bold'), bg = "#e1f2f2")
            label1.grid(row = 1, column = 2)
       
       
        def defendingFunction():
            cv2.destroyAllWindows()
            global normal
            normal = 4
            global scoreEnemy, scoreUser
            scoreUser = 0
            scoreEnemy = 0
            global time
            global homed
            global varNE
            global varE
            
            
            def defendingLoop():
                global normal, scoreUser, scoreEnemy
                global time
                global homed
                global varE
                global varNE
        
                if normal != 0:
                    
                    success, img = cap.read()
                    
                    
                    imgColor, mask = myColorFinder.update(img, hsvVals)
                    
                    
                    x,y=getContours(mask)
                    xfirst = x
                    yfirst = y
            
                    success, img = cap.read()
                    imgColor, mask = myColorFinder.update(img, hsvVals)
                    
                    
                    x,y=getContours(mask)
                    xsecond = x
                    ysecond = y
                    
                    
                    yFut, deltaXX = trackpuck(img, xfirst, yfirst, xsecond, ysecond, endPoint)
                    
                    yCoord = round((y-80)*1.73)
                    xCoord = round((x-40)*1.73)
                    
                    if yFut > 80:
                        CoordsInMM = (int(yFut) -80)*1.73
                        RoundNum = round(CoordsInMM)
                        
                    else:
                        RoundNum = 1
                    
                    if normal == 1:
                        
                        if deltaXX < -4 and yFut > 80:
                            if xsecond > 270 and xsecond < 500 and homed == 1:
                                homed = 0
                                Pos = bytes("p," + str(RoundNum) + ",60\n", 'UTF-8')
                                ser.write(Pos)
              
              
                
                        if deltaXX > -4 and yFut > 40:
                            
                            if xsecond > 100 and xsecond < 200:
                                homed = 0
                                Pos = bytes("p," + str(yCoord) + "," + str(xCoord) + "\n", 'UTF-8')
                                ser.write(Pos)
                                print("pos")
                                
                            if xsecond > 300 and xsecond < 500 and homed == 0:
                                homed = 1
                                print("zruck")
                                Pos = bytes("p,300,60\n", 'UTF-8')
                                ser.write(Pos)
                        
                        
                        
                
                    if normal == 2:
                        
                        if deltaXX < -4 and yFut > 80:
                            if xsecond > 300 and xsecond < 420 and homed == 1:
                                homed = 0
                                attackPos = bytes("p," + str(RoundNum) + ",250\n", 'UTF-8')
                                ser.write(attackPos)
                            
                                
                                
                
                
                        if deltaXX > -4 and yFut > 80:
                            if xsecond < 200 and xsecond > 100:
                                homed = 0
                                attackPos = bytes("p," + str(yCoord) + "," + str(xCoord) + "\n", 'UTF-8')
                                ser.write(attackPos)
                                
                            if xsecond > 300 and xsecond < 500 and homed == 0:
                                homed == 1
                                attackPos = bytes("p,300,150\n", 'UTF-8')
                                ser.write(attackPos)
            
            
                
                    if normal == 3:
                        
                        RoundNum = RoundNum + time
                        
                        yFutReal = yFut + RoundNum
                        if deltaXX < -4 and yFut > 80:
                            if xsecond > 270 and xsecond < 420 and homed == 1:
                                homed = 0
                                attackPos = bytes("p," + str(yFutReal) + ",300\n", 'UTF-8')
                                ser.write(attackPos)
                            
                                
                                
                
                
                        if deltaXX > -4 and yFut > 80:
                            if x < 200:
                                homed = 0
                                attackPos = bytes("p," + str(yCoord) + "," + str(xCoord) + "\n", 'UTF-8')
                                ser.write(attackPos)
                                
                            if x > 200 and homed == 0:
                                homed = 1
                                attackPos = bytes("p,300,150", 'UTF-8')
                                ser.write(attackPos)
                                
                        
                        
                        
            
                    
                    if xsecond == 0 and ysecond == 0 and varNE == 0:
                        msgNE = bytes("nd\n\r", 'UTF-8')
                        ser.write(msgNE)
                        varNE = 1
                    
                    if xsecond != 0 and ysecond != 0 and varE == 0:
                        msgE = bytes("n\n\r", 'UTF-8')
                        ser.write(msgE)
                        varE = 1
                        
                    
                    
                    
                    
                    if xsecond < 10 or xsecond > 400 and yFut > 80:
                        daten = ser.readline()
        
                        scoreSignal = daten.decode('utf8')
                    
                    else:
                        scoreSignal = 0
                        
                    
                   
                    
                    if scoreSignal == "gr\r\n":
                        scoreEnemy += 1
                        
                        label1 = Label(buttonFrame2, text = scoreEnemy,font=('bold', 100, 'bold'), bg = "#e1f2f2")
                        label1.grid(row = 1, column = 2)
        
            
            
                    if scoreSignal == "gh\r\n":  
                        scoreUser += 1
                        label2 = Label(buttonFrame2, text = scoreUser,font=('bold', 100, 'bold'), bg = "#e1f2f2")
                        label2.grid(row = 1, column = 0)
                    
                    if scoreSignal == "Error\r\n":
                        window = Tk()
                        window.title("Error")
                        window.geometry("400x100")
                        
                        normal = 0
                        errorLabel = Label(window, text = "Fehler ist aufgetreten!",font=('bold', 20, 'bold'))
                        errorLabel.pack()
                        ButtonClose = Button(window, text = "Close", command = window.destroy)
                        ButtonClose.pack()
                        window.mainloop()
                    
                    if normal != 0:
                        cv2.namedWindow("Attack", cv2.WINDOW_NORMAL)
                        cv2.resizeWindow("Attack", 640, 480)
                        cv2.moveWindow("Attack", 640, 500)    
                        cv2.imshow("Attack", img)
                    
                    cv2.waitKey(1)
                
                
                
                
                if normal != 0:     
                    app.after(1, defendingLoop)
    
            if normal != 0:
                app.after(1, defendingLoop)
        
        
        
        
        
        
        buttonFrame1 = Frame(self, bg="#e1f2f2")
        buttonFrame1.pack(side =TOP, fill = "x")    
        
                
        backButton = Button(buttonFrame1, text="Back", font=('bold', 45, 'bold'),command = combine_funcs(lambda:controller.show_frame("StartPage"), reset1), image = self.backImage, border = 0, bg = "#e1f2f2", fg = "#e1f2f2", activeforeground = "#e1f2f2", activebackground = "#e1f2f2")
        backButton.grid(row = 0, column = 0, padx = 10, pady = 10)
        
        
        
        buttonFrame2 = Frame(self, bg="#e1f2f2")
        buttonFrame2.pack(side =TOP, fill = "x")
        
        headingLabel = Label(buttonFrame1, text="Game", font=('bold', 45, 'bold'), bg = "#e1f2f2")
        headingLabel.grid(row = 0, column = 1, padx = 650,pady = 10)
        
        
        label1 = Label(buttonFrame2, text = 0,font=('bold', 100, 'bold'), bg = "#e1f2f2")
        label1.grid(row = 1, column = 2)
        
        label2 = Label(buttonFrame2, text = 0,font=('bold', 100, 'bold'), bg = "#e1f2f2")
        label2.grid(row = 1, column = 0)
        
        
        startRandButton = Button(buttonFrame1, text = "Attack+", font=('bold', 20, 'bold'),command = modeAttackPlus, border = 0, bg = "#e1f2f2", activebackground = "#e1f2f2")
        startRandButton.grid(row = 0, column = 2)
        
        
        
        
        startbutton = Button(buttonFrame2, text ="Start", font=('bold', 20, 'bold'), command = defendingFunction, image = self.StartPic, border = 0, bg = "#e1f2f2", fg = "#e1f2f2", activeforeground = "#e1f2f2", activebackground = "#e1f2f2")
        startbutton.grid(row = 1, column = 1, padx = 130,pady = 200)

        defendsbutton = Button(buttonFrame2, text = "Defending", font=('bold', 20, 'bold'),  command = modeDef, image = self.defendPic, border = 0, bg = "#e1f2f2", fg = "#e1f2f2", activeforeground = "#e1f2f2", activebackground = "#e1f2f2")
        defendsbutton.grid(row = 2, column = 0, padx = 200, pady = 1)

        normalbutton = Button(buttonFrame2, text = "Normal", font=('bold', 20, 'bold'),command = modeAttack, image = self.AttackPic, border = 0, bg = "#e1f2f2", fg = "#e1f2f2", activeforeground = "#e1f2f2", activebackground = "#e1f2f2")
        normalbutton.grid(row = 2, column = 2, padx = 250)
        

#################################################################################################################################################   
#################################################################################################################################################

        
if __name__ == "__main__":
    global app
    app = SampleApp()
    app.attributes('-fullscreen', True)
    app.mainloop()

