import mediapipe as mp
import cv2
import numpy as np
import uuid
import os

mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands

cap = cv2.VideoCapture(1)

with mp_hands.Hands(min_detection_confidence=0.8, min_tracking_confidence=0.5) as hands: 
    while cap.isOpened():
        ret, frame = cap.read()
        
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        h, w, c = image.shape
        
        image = cv2.flip(image, 1)
        
        image.flags.writeable = False
        results = hands.process(image)
        image.flags.writeable = True
        
        # RGB 2 BGR
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        # print(results)
        
        if not results.multi_hand_landmarks:
            continue

        cv2.rectangle(image, (100, 100), (400, 200), (0, 0, 255) , 3, cv2.LINE_AA)

        for num, hand in enumerate(results.multi_hand_landmarks):
            for l in hand.landmark:
                x = l.x * w
                y = l.y * h
                if x < 400 and y < 200 and x > 100 and y > 100:
                    print("access")
                    image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
                    break

            mp_drawing.draw_landmarks(image, hand, mp_hands.HAND_CONNECTIONS, 
                mp_drawing.DrawingSpec(color=(121, 22, 76), thickness=2, circle_radius=4),
                mp_drawing.DrawingSpec(color=(250, 44, 250), thickness=2, circle_radius=2),
            )
            
        # print(results.multi_hand_landmarks)
        cv2.imshow('Hand Tracking', image)

        if cv2.waitKey(10) & 0xFF == ord('q'):
            break

cap.release()
cv2.destroyAllWindows()