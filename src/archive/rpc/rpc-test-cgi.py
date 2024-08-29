#!/usr/bin/env python
# coding: utf-8

import pyjsonrpc
import time

http_client = pyjsonrpc.HttpClient(
    url = "http://localhost/cgi-bin/rpc-cgi.py"
)

# print http_client.call("add", 1, 2)
# # Result: 3

# # It is also possible to use the *method* name as *attribute* name.
# print http_client.add(1, 2)
# # Result: 3

# # Notifications send messages to the server, without response.
# http_client.notify("add", 3, 4)

print(http_client.open('RF21X_DT_RF218', 'hid://', 1, 40))
print(http_client.startQuiz('RF21X_QT_Single'))

print(None)
t = 0
while t < 10:
    t = t + 1
    time.sleep(1)
    print(http_client.getMessage())

print(http_client.stopQuiz())
print(http_client.close())
