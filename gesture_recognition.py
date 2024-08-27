import sys

import numpy as np

import cv2

import mediapipe as mp

import time

import warnings

import logging



# Configure logging to write to a file instead of the console

logging.basicConfig(filename='gesture_recognition.log', level=logging.INFO,

                    format='%(asctime)s - %(levelname)s - %(message)s')



# Suppress TensorFlow warnings

warnings.filterwarnings('ignore', category=FutureWarning)

import tensorflow as tf

tf.compat.v1.logging.set_verbosity(tf.compat.v1.logging.ERROR)



class GestureRecognizer:

    def __init__(self):

        self.mp_hands = mp.solutions.hands

        self.mp_drawing = mp.solutions.drawing_utils

        self.mp_drawing_styles = mp.solutions.drawing_styles

        self.hands = self.mp_hands.Hands(

            model_complexity=0,

            min_detection_confidence=0.5,

            min_tracking_confidence=0.5)

        self.last_process_time = time.time()

        self.first_frame = True



    def process_frame(self, frame):

        if self.first_frame:

            logging.info(f"Processing first frame: shape={frame.shape}, dtype={frame.dtype}")

            logging.info(f"First few pixels: {frame[:5, 0, :]}")

            self.first_frame = False



        if frame is None or frame.size == 0:

            return "No gesture", frame



        results = self.hands.process(frame)



        if results.multi_hand_landmarks:

            for hand_landmarks in results.multi_hand_landmarks:

                self.mp_drawing.draw_landmarks(

                    frame,

                    hand_landmarks,

                    self.mp_hands.HAND_CONNECTIONS,

                    self.mp_drawing_styles.get_default_hand_landmarks_style(),

                    self.mp_drawing_styles.get_default_hand_connections_style())



        gesture = self.recognize_gesture(results)

        return gesture, frame



    def recognize_gesture(self, results):

        if results.multi_hand_landmarks:

            for hand_landmarks in results.multi_hand_landmarks:

                hand = 0 if hand_landmarks.landmark[self.mp_hands.HandLandmark.WRIST].x < 0.5 else 1

                fingers = self.fingersUp(hand_landmarks.landmark, hand)

                gesture = self.findSymbol(fingers)

                return gesture

        return "No gesture"



    def fingersUp(self, lmList, hand):

        tipIds = [4, 8, 12, 16, 20]

        fingers = []



        # Thumb

        if hand == 0:  # left hand

            fingers.append(1 if lmList[tipIds[0]].x > lmList[tipIds[0] - 1].x else 0)

        else:  # right hand

            fingers.append(1 if lmList[tipIds[0]].x < lmList[tipIds[0] - 1].x else 0)



        # 4 Fingers

        for id in range(1, 5):

            if lmList[tipIds[id]].y < lmList[tipIds[id] - 2].y:

                fingers.append(1)

            else:

                fingers.append(0)



        return fingers



    def findSymbol(self, fingers):

        if fingers == [0, 0, 0, 0, 0]:

            return "Rock"

        elif fingers == [1, 1, 1, 1, 1]:

            return "Paper"

        elif fingers == [0, 1, 1, 0, 0]:

            return "Scissors"

        else:

            return "No gesture"



# Global GestureRecognizer instance

recognizer = None



def initialize():

    global recognizer

    if recognizer is None:

        recognizer = GestureRecognizer()

        logging.info("GestureRecognizer initialized")



def process_frame(frame_data, width, height):

    global recognizer

    if recognizer is None:

        initialize()



    frame = np.frombuffer(frame_data, dtype=np.uint8).reshape((height, width, 3))



    if frame.size == 0:

        logging.warning("Received empty frame")

        return "No gesture", frame_data



    gesture, processed_frame = recognizer.process_frame(frame)



    return gesture, processed_frame.tobytes()



# Initialize the global instance when the module is imported

initialize()