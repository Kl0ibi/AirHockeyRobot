import cvzone
import cv2
from cvzone.ColorModule import ColorFinder
import numpy as np
import time



def getContours(img):
    img, contours, hierarchy = cv2.findContours(img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    x, y, w, h = 0, 0, 0, 0
    
    for cnt in contours:
        area = cv2.contourArea(cnt)
        if (area//10) >55:
            peri = cv2.arcLength(cnt, True)
            approx = cv2.approxPolyDP(cnt, 0.02 * peri, True)
            x, y, w, h = cv2.boundingRect(approx)

    return (x + (w // 2),y+(h//2))




def predict(x, y, deltaX, deltaY, imgContours, endPoint):
    
    
    
    #stepX = -1
    
    if deltaX != 0:
        stepY = -(deltaY/deltaX)
    else:
        stepY = 0
   
    while(1):    
        y_next = y+stepY
        x_next = x-1
        cv2.circle(imgContours, (x_next, int(y_next)),1, (0,255,255), cv2.FILLED)
        
        if x_next <= endPoint:
           
           return int(y_next)
           
        if y_next <= 40 or y_next >= 420:
            stepY = -stepY
        
        y = y_next
        x = x_next
    
    



def trackpuck(imgContours, lastx, lasty, x, y, endPoint):

        
    deltaX = x-lastx
    deltaY = y-lasty
    
    if deltaX < -3:
        yf = predict(x,y, deltaX, deltaY, imgContours, endPoint)
        return yf, deltaX
    else:
        return 80, deltaX









    




