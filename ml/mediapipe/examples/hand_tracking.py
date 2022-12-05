#you must install mediapipe and cv2
#INSTALL MEDIAPIPE
  #click search bar then write "cmd"and right click then click run as administrator
  #copy then paste:pip install mediapipe
#INSTALL CV2
  #copy then paste:pip install opencv-python

#If you don't know how to install,sigh and comment your question :)

from unittest import result
import cv2
import mediapipe as mp
import time

cap = cv2.VideoCapture(0)

mpHand = mp.solutions.hands
hands = mpHand.Hands()
mpdraw = mp.solutions.drawing_utils

while True:
    success, img = cap.read()
    imgRBG = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
    results = hands.process(imgRBG)
    #print(results.multi_hand_landmarks)
    if results.multi_hand_landmarks:
        for handlms in results.multi_hand_landmarks:
            mpdraw.draw_landmarks(img,handlms,mpHand.HAND_CONNECTIONS)

    cv2.imshow("normal", img)
    cv2.waitKey(1)

