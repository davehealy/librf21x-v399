# Guide

0. Make sure you have the python api files "_rf21x.pyd" and "rf21x.py", and the low-level api file "rf21x-api.dll" in this directory. Then you can run "rf21x-test.py" to check your SDK and hardware. Moreover, make sure you have installed "Python 2.7" and the package "python-jsonrpc".
1. Run "rpc-server.py" to start RPC-API server, and you can run "rpc-test.py" to check it. Now we have a local RPC server whcih can control our hardware devices.
2. Deploy "rpc-cgi.py" and "pyjsonrpc" on your web server (IIS or Apache), and run "rpc-test-cgi.py" to check it. Now we have a remote API on your server which is used to access local RPC server.
3. Deploy "rpc-test-cgi.html" and "jquery-2.1.1.min.js" on your web server (IIS or Apache), and access this page to check it. Now we can access our hardware in web page. The request is started by JS in your broswer, posted to remote server, and then gated to local rpc-server for actual hardware control. 

# Note

If you are using a normal desktop software, librf21x SDK is the best choice.
Other wise, if you need your software access RF21x hardware on other computer, you need "rpc-server.py", which provide JSON-RPC API.
Finally, if you need to access RF21x hardware in your HTML page, you need both "rpc-server.py" and "rpc-cgi.py". The CGI module will transport your AJAX request to JSON-RPC API, and hide all session and cross domain problem.

# Documents

All you got is just 5 API, you can get their usage in "rpc-test.py", "rpc-test-cgi.py" or "rpc-test-cgi.html":
    - open
    - close
    - startQuiz
    - stopQuiz
    - getMessage

Supported device type:
device_type_list = {
    'RF21X_DT_RF215': rf21x.RF21X_DT_RF215,
    'RF21X_DT_RF217': rf21x.RF21X_DT_RF217,
    'RF21X_DT_RF218': rf21x.RF21X_DT_RF218,
    'RF21X_DT_RF219': rf21x.RF21X_DT_RF219,
    None: rf21x.RF21X_DT_RF218,
}


Supported quiz type:
quiz_type_list = {
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

# More Detail Documents:
For more detail, you can check documents with librf21x C/C++ API.
