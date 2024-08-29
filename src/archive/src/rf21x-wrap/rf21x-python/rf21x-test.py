#!/usr/bin/env python
# coding=utf-8

#####################################################################
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

import os
os.sys.path.append(os.path.dirname(__file__))
#####################################################################

import rf21x
import time

print("Begin!")

rf = rf21x.RF21xDevice()
rf.open(rf21x.RF21X_DT_RF218, 'hid://', 1, 40)
time.sleep(1)
rf.startQuiz(rf21x.RF21X_QT_Single)

message = rf21x.RF21xMessage()
raw_data = rf21x.RF21xMessageRawData()
while True:
    if (rf.getMessage(message)):
        msg = {
            'messageType': message.messageType,
            'quizType': message.quizType,
            'quizNumber': message.quizNumber,
            'TimeStamp': message.timeStamp,
            'keypadId': message.keypadId,
            'data': message.data,
        }
        print(msg)

        if message.messageType == rf21x.RF21X_MT_Teacher:
            print("Teacher: " + message.data)
        elif message.messageType == rf21x.RF21X_MT_Student:
            print("Student %d for current question: %s" % (message.keypadId, message.data))
        elif message.messageType == rf21x.RF21X_MT_SetId:
            print("Set ID %d\n" % message.keypadId)
        else:
            pass
        str = 'Raw Data: '
        
        for i in message.rawData:
            str += '%02X ' % i
        print(str)
raw_input('Press any key to exit.')


rf.stopQuiz()
rf.close()

print("Finish!")
