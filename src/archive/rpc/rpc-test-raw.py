#!/usr/bin/env python
# coding: utf-8

import json
import urllib2

data = {"params": ["RF21X_DT_RF218", "hid://", 1, 40], "jsonrpc": "2.0", "method": "open", "id": "1"}
result = urllib2.urlopen('http://localhost:8080/', json.dumps(data))
print(result.read())
print(type(result.read()))
print(result.info())