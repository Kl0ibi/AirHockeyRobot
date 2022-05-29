import serial
import time

ser = serial.Serial('/dev/ttyACM0',115200, timeout=0.000000000000000000001)
ser.flush()

############################################################

def functiontest():
    print("Test geht")

############################################################
def fans1():
    print("fans")
    fans = bytes("fans\n", 'UTF-8')
    print(fans)
    
    ser.write(fans)


############################################################
    
def mvalFunc():
    mval = bytes("mval\n", 'UTF-8')
    ser.write(mval)
    

def sole():
    soleV = bytes("solenoid\n", 'UTF-8')
    ser.write(soleV)

############################################################

def demo():
    demo = bytes("demo\n", 'UTF-8')
    ser.write(demo)

############################################################

def cycle():
    cycle = bytes("cycle\n", 'UTF-8')
    ser.write(cycle)
         
############################################################
         
def home():
    home = bytes("home\n", 'UTF-8')
    ser.write(home)
    
############################################################
        
def leds():
    leds = bytes("leds\n", 'UTF-8')
    ser.write(leds)

############################################################
        
def dvals():
    default = bytes("default\n", 'UTF-8')
    ser.write(default)

############################################################

def startFunction():
    home()
    leds()
    fans1()
    
    
    
############################################################

def combine_funcs(*funcs):
    def combined_func(*args, **kwargs):
        for f in funcs:
            f(*args,**kwargs)
    return combined_func

