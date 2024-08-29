#!/usr/bin/env python
# coding: utf-8

import pyjsonrpc
import rf21x


def add(a, b):
    """Test function"""
    return a + b

rf = rf21x.RF21xDevice()


def open(typ, path, min, max):
    typ = str(typ)
    path = str(path)
    type_list = {
        'RF21X_DT_RF215': rf21x.RF21X_DT_RF215,
        'RF21X_DT_RF217': rf21x.RF21X_DT_RF217,
        'RF21X_DT_RF218': rf21x.RF21X_DT_RF218,
        'RF21X_DT_RF219': rf21x.RF21X_DT_RF219,
        None: rf21x.RF21X_DT_RF218,
    }
    return rf.open(type_list.get(typ), path, min, max)


def close():
    return rf.close()


def startQuiz(typ, param1=0, param2=0):
    typ = str(typ)
    type_list = {
        'RF21X_QT_Homewrok': rf21x.RF21X_QT_Homewrok,
        'RF21X_QT_Unknow': rf21x.RF21X_QT_Unknow,
        'RF21X_QT_Rush': rf21x.RF21X_QT_Rush,
        'RF21X_QT_Single': rf21x.RF21X_QT_Single,
        'RF21X_QT_Multiple': rf21x.RF21X_QT_Multiple,
        'RF21X_QT_Number': rf21x.RF21X_QT_Number,
        'RF21X_QT_Text': rf21x.RF21X_QT_Text,
        'RF21X_QT_Classify': rf21x.RF21X_QT_Classify,
        'RF21X_QT_Sort': rf21x.RF21X_QT_Sort,
        'RF21X_Qt_JudgeOrVote': rf21x.RF21X_Qt_JudgeOrVote,
        'RF21X_QT_SelectId': rf21x.RF21X_QT_SelectId,
        'RF21X_QT_Control': rf21x.RF21X_QT_Control,
        None: rf21x.RF21X_QT_Single,
    }
    return rf.startQuiz(type_list.get(typ))


def stopQuiz():
    return rf.stopQuiz()


def getMessage():
    msg = rf21x.RF21xMessage()
    msg_list = []
    while rf.getMessage(msg):
        m = {
            'messageType': msg.messageType,
            'quizType': msg.quizType,
            'quizNumber': msg.quizNumber,
            'TimeStamp': msg.timeStamp,
            'keypadId': msg.keypadId,
            'data': msg.data,
        }
        print(m)
        msg_list.append(m)
    return msg_list


class RequestHandler(pyjsonrpc.HttpRequestHandler):

    # Register public JSON-RPC methods
    methods = {
        "add": add,
        "open": open,
        "close": close,
        "startQuiz": startQuiz,
        "stopQuiz": stopQuiz,
        "getMessage": getMessage,
    }

# Threading HTTP-Server
http_server = pyjsonrpc.ThreadingHttpServer(
    server_address=('localhost', 8080),
    RequestHandlerClass = RequestHandler
)
print "Starting HTTP server ..."
print "URL: http://localhost:8080"
http_server.serve_forever()
