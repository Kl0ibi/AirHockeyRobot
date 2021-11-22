import cv2
import numpy as np

frameWidth = 640
frameHeight = 480
cap = cv2.VideoCapture('Ressources/pucktest3.mp4')
cap.set(3, frameWidth)
cap.set(4, frameHeight)
#cap.set(640,480)

myColors = [30, 94, 33, 76, 236, 209]
myColorValues = [[0, 255, 0]]



def findColor(img, myColors):
    imgHSV = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    lower = np.array(myColors[0:3])
    upper = np.array(myColors[3:6])
    mask = cv2.inRange(imgHSV, lower, upper)
    x, y, area = getContours(mask)
    cv2.circle(img, (x, y), area, (255, 0, 0), 3)

    cv2.imshow("img", mask)
    # if (area)>55:
    # cv2.circle(img, (x, y), area, (255, 0, 0), 2)


def getContours(img):
    img, contours, hierarchy = cv2.findContours(img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    x, y, w, h = 0, 0, 0, 0
    for cnt in contours:
        area = cv2.contourArea(cnt)
        #print(area//200)
        if (area//200) >55:
            cv2.drawContours(img, cnt, -1, (255, 0, 0), 3)
            peri = cv2.arcLength(cnt, True)
           # print(peri)
            approx = cv2.approxPolyDP(cnt, 0.02 * peri, True)
            #print(approx)
            x, y, w, h = cv2.boundingRect(approx)

            print('x: %d y: %d'%(x,y))


    return (x + (w // 2),y+(h//2),(int)(area//200))


while True:

    success, img = cap.read()
    findColor(img, myColors)

    cv2.imshow("Result",img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


