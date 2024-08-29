#!"C:\Python27\python.exe"
# coding: utf-8

import pyjsonrpc

http_client = pyjsonrpc.HttpClient(
    url="http://localhost:8080/jsonrpc"
)


def add(a, b):
    return a + b


def open(typ, path, min, max):
    return http_client.open(typ, path, min, max)


def close():
    return http_client.close()


def startQuiz(typ, param1=0, param2=0):
    return http_client.startQuiz(typ, param1, param2)


def stopQuiz():
    return http_client.stopQuiz()


def getMessage():
    return http_client.getMessage()

methods = {
    "add": add,
    "open": open,
    "close": close,
    "startQuiz": startQuiz,
    "stopQuiz": stopQuiz,
    "getMessage": getMessage,
}

pyjsonrpc.handle_cgi_request(methods=methods)
