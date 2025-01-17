# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.11
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_rf21x', [dirname(__file__)])
        except ImportError:
            import _rf21x
            return _rf21x
        if fp is not None:
            try:
                _mod = imp.load_module('_rf21x', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _rf21x = swig_import_helper()
    del swig_import_helper
else:
    import _rf21x
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class SwigPyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SwigPyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SwigPyIterator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _rf21x.delete_SwigPyIterator
    __del__ = lambda self : None;
    def value(self): return _rf21x.SwigPyIterator_value(self)
    def incr(self, n=1): return _rf21x.SwigPyIterator_incr(self, n)
    def decr(self, n=1): return _rf21x.SwigPyIterator_decr(self, n)
    def distance(self, *args): return _rf21x.SwigPyIterator_distance(self, *args)
    def equal(self, *args): return _rf21x.SwigPyIterator_equal(self, *args)
    def copy(self): return _rf21x.SwigPyIterator_copy(self)
    def next(self): return _rf21x.SwigPyIterator_next(self)
    def __next__(self): return _rf21x.SwigPyIterator___next__(self)
    def previous(self): return _rf21x.SwigPyIterator_previous(self)
    def advance(self, *args): return _rf21x.SwigPyIterator_advance(self, *args)
    def __eq__(self, *args): return _rf21x.SwigPyIterator___eq__(self, *args)
    def __ne__(self, *args): return _rf21x.SwigPyIterator___ne__(self, *args)
    def __iadd__(self, *args): return _rf21x.SwigPyIterator___iadd__(self, *args)
    def __isub__(self, *args): return _rf21x.SwigPyIterator___isub__(self, *args)
    def __add__(self, *args): return _rf21x.SwigPyIterator___add__(self, *args)
    def __sub__(self, *args): return _rf21x.SwigPyIterator___sub__(self, *args)
    def __iter__(self): return self
SwigPyIterator_swigregister = _rf21x.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

class RF21xMessageRawData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, RF21xMessageRawData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, RF21xMessageRawData, name)
    __repr__ = _swig_repr
    def iterator(self): return _rf21x.RF21xMessageRawData_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _rf21x.RF21xMessageRawData___nonzero__(self)
    def __bool__(self): return _rf21x.RF21xMessageRawData___bool__(self)
    def __len__(self): return _rf21x.RF21xMessageRawData___len__(self)
    def pop(self): return _rf21x.RF21xMessageRawData_pop(self)
    def __getslice__(self, *args): return _rf21x.RF21xMessageRawData___getslice__(self, *args)
    def __setslice__(self, *args): return _rf21x.RF21xMessageRawData___setslice__(self, *args)
    def __delslice__(self, *args): return _rf21x.RF21xMessageRawData___delslice__(self, *args)
    def __delitem__(self, *args): return _rf21x.RF21xMessageRawData___delitem__(self, *args)
    def __getitem__(self, *args): return _rf21x.RF21xMessageRawData___getitem__(self, *args)
    def __setitem__(self, *args): return _rf21x.RF21xMessageRawData___setitem__(self, *args)
    def append(self, *args): return _rf21x.RF21xMessageRawData_append(self, *args)
    def empty(self): return _rf21x.RF21xMessageRawData_empty(self)
    def size(self): return _rf21x.RF21xMessageRawData_size(self)
    def clear(self): return _rf21x.RF21xMessageRawData_clear(self)
    def swap(self, *args): return _rf21x.RF21xMessageRawData_swap(self, *args)
    def get_allocator(self): return _rf21x.RF21xMessageRawData_get_allocator(self)
    def begin(self): return _rf21x.RF21xMessageRawData_begin(self)
    def end(self): return _rf21x.RF21xMessageRawData_end(self)
    def rbegin(self): return _rf21x.RF21xMessageRawData_rbegin(self)
    def rend(self): return _rf21x.RF21xMessageRawData_rend(self)
    def pop_back(self): return _rf21x.RF21xMessageRawData_pop_back(self)
    def erase(self, *args): return _rf21x.RF21xMessageRawData_erase(self, *args)
    def __init__(self, *args): 
        this = _rf21x.new_RF21xMessageRawData(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _rf21x.RF21xMessageRawData_push_back(self, *args)
    def front(self): return _rf21x.RF21xMessageRawData_front(self)
    def back(self): return _rf21x.RF21xMessageRawData_back(self)
    def assign(self, *args): return _rf21x.RF21xMessageRawData_assign(self, *args)
    def resize(self, *args): return _rf21x.RF21xMessageRawData_resize(self, *args)
    def insert(self, *args): return _rf21x.RF21xMessageRawData_insert(self, *args)
    def reserve(self, *args): return _rf21x.RF21xMessageRawData_reserve(self, *args)
    def capacity(self): return _rf21x.RF21xMessageRawData_capacity(self)
    __swig_destroy__ = _rf21x.delete_RF21xMessageRawData
    __del__ = lambda self : None;
RF21xMessageRawData_swigregister = _rf21x.RF21xMessageRawData_swigregister
RF21xMessageRawData_swigregister(RF21xMessageRawData)

class RF21xHidSerialNumbers(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, RF21xHidSerialNumbers, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, RF21xHidSerialNumbers, name)
    __repr__ = _swig_repr
    def iterator(self): return _rf21x.RF21xHidSerialNumbers_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _rf21x.RF21xHidSerialNumbers___nonzero__(self)
    def __bool__(self): return _rf21x.RF21xHidSerialNumbers___bool__(self)
    def __len__(self): return _rf21x.RF21xHidSerialNumbers___len__(self)
    def pop(self): return _rf21x.RF21xHidSerialNumbers_pop(self)
    def __getslice__(self, *args): return _rf21x.RF21xHidSerialNumbers___getslice__(self, *args)
    def __setslice__(self, *args): return _rf21x.RF21xHidSerialNumbers___setslice__(self, *args)
    def __delslice__(self, *args): return _rf21x.RF21xHidSerialNumbers___delslice__(self, *args)
    def __delitem__(self, *args): return _rf21x.RF21xHidSerialNumbers___delitem__(self, *args)
    def __getitem__(self, *args): return _rf21x.RF21xHidSerialNumbers___getitem__(self, *args)
    def __setitem__(self, *args): return _rf21x.RF21xHidSerialNumbers___setitem__(self, *args)
    def append(self, *args): return _rf21x.RF21xHidSerialNumbers_append(self, *args)
    def empty(self): return _rf21x.RF21xHidSerialNumbers_empty(self)
    def size(self): return _rf21x.RF21xHidSerialNumbers_size(self)
    def clear(self): return _rf21x.RF21xHidSerialNumbers_clear(self)
    def swap(self, *args): return _rf21x.RF21xHidSerialNumbers_swap(self, *args)
    def get_allocator(self): return _rf21x.RF21xHidSerialNumbers_get_allocator(self)
    def begin(self): return _rf21x.RF21xHidSerialNumbers_begin(self)
    def end(self): return _rf21x.RF21xHidSerialNumbers_end(self)
    def rbegin(self): return _rf21x.RF21xHidSerialNumbers_rbegin(self)
    def rend(self): return _rf21x.RF21xHidSerialNumbers_rend(self)
    def pop_back(self): return _rf21x.RF21xHidSerialNumbers_pop_back(self)
    def erase(self, *args): return _rf21x.RF21xHidSerialNumbers_erase(self, *args)
    def __init__(self, *args): 
        this = _rf21x.new_RF21xHidSerialNumbers(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _rf21x.RF21xHidSerialNumbers_push_back(self, *args)
    def front(self): return _rf21x.RF21xHidSerialNumbers_front(self)
    def back(self): return _rf21x.RF21xHidSerialNumbers_back(self)
    def assign(self, *args): return _rf21x.RF21xHidSerialNumbers_assign(self, *args)
    def resize(self, *args): return _rf21x.RF21xHidSerialNumbers_resize(self, *args)
    def insert(self, *args): return _rf21x.RF21xHidSerialNumbers_insert(self, *args)
    def reserve(self, *args): return _rf21x.RF21xHidSerialNumbers_reserve(self, *args)
    def capacity(self): return _rf21x.RF21xHidSerialNumbers_capacity(self)
    __swig_destroy__ = _rf21x.delete_RF21xHidSerialNumbers
    __del__ = lambda self : None;
RF21xHidSerialNumbers_swigregister = _rf21x.RF21xHidSerialNumbers_swigregister
RF21xHidSerialNumbers_swigregister(RF21xHidSerialNumbers)

RF21X_DT_Unknow = _rf21x.RF21X_DT_Unknow
RF21X_DT_RF217 = _rf21x.RF21X_DT_RF217
RF21X_DT_RF218 = _rf21x.RF21X_DT_RF218
RF21X_DT_RF219 = _rf21x.RF21X_DT_RF219
RF21X_DT_RF215 = _rf21x.RF21X_DT_RF215
RF21X_MT_Unknow = _rf21x.RF21X_MT_Unknow
RF21X_MT_Teacher = _rf21x.RF21X_MT_Teacher
RF21X_MT_Student = _rf21x.RF21X_MT_Student
RF21X_MT_SetId = _rf21x.RF21X_MT_SetId
RF21X_QT_Homewrok = _rf21x.RF21X_QT_Homewrok
RF21X_QT_Unknow = _rf21x.RF21X_QT_Unknow
RF21X_QT_Rush = _rf21x.RF21X_QT_Rush
RF21X_QT_Single = _rf21x.RF21X_QT_Single
RF21X_QT_Multiple = _rf21x.RF21X_QT_Multiple
RF21X_QT_Number = _rf21x.RF21X_QT_Number
RF21X_QT_Text = _rf21x.RF21X_QT_Text
RF21X_QT_Classify = _rf21x.RF21X_QT_Classify
RF21X_QT_Sort = _rf21x.RF21X_QT_Sort
RF21X_Qt_JudgeOrVote = _rf21x.RF21X_Qt_JudgeOrVote
RF21X_QT_SelectId = _rf21x.RF21X_QT_SelectId
RF21X_QT_Control = _rf21x.RF21X_QT_Control

def getHidSerialNumber():
  return _rf21x.getHidSerialNumber()
getHidSerialNumber = _rf21x.getHidSerialNumber
class RF21xMessage(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, RF21xMessage, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, RF21xMessage, name)
    __repr__ = _swig_repr
    __swig_setmethods__["messageType"] = _rf21x.RF21xMessage_messageType_set
    __swig_getmethods__["messageType"] = _rf21x.RF21xMessage_messageType_get
    if _newclass:messageType = _swig_property(_rf21x.RF21xMessage_messageType_get, _rf21x.RF21xMessage_messageType_set)
    __swig_setmethods__["quizType"] = _rf21x.RF21xMessage_quizType_set
    __swig_getmethods__["quizType"] = _rf21x.RF21xMessage_quizType_get
    if _newclass:quizType = _swig_property(_rf21x.RF21xMessage_quizType_get, _rf21x.RF21xMessage_quizType_set)
    __swig_setmethods__["quizNumber"] = _rf21x.RF21xMessage_quizNumber_set
    __swig_getmethods__["quizNumber"] = _rf21x.RF21xMessage_quizNumber_get
    if _newclass:quizNumber = _swig_property(_rf21x.RF21xMessage_quizNumber_get, _rf21x.RF21xMessage_quizNumber_set)
    __swig_setmethods__["timeStamp"] = _rf21x.RF21xMessage_timeStamp_set
    __swig_getmethods__["timeStamp"] = _rf21x.RF21xMessage_timeStamp_get
    if _newclass:timeStamp = _swig_property(_rf21x.RF21xMessage_timeStamp_get, _rf21x.RF21xMessage_timeStamp_set)
    __swig_setmethods__["keypadId"] = _rf21x.RF21xMessage_keypadId_set
    __swig_getmethods__["keypadId"] = _rf21x.RF21xMessage_keypadId_get
    if _newclass:keypadId = _swig_property(_rf21x.RF21xMessage_keypadId_get, _rf21x.RF21xMessage_keypadId_set)
    __swig_setmethods__["data"] = _rf21x.RF21xMessage_data_set
    __swig_getmethods__["data"] = _rf21x.RF21xMessage_data_get
    if _newclass:data = _swig_property(_rf21x.RF21xMessage_data_get, _rf21x.RF21xMessage_data_set)
    __swig_setmethods__["rawData"] = _rf21x.RF21xMessage_rawData_set
    __swig_getmethods__["rawData"] = _rf21x.RF21xMessage_rawData_get
    if _newclass:rawData = _swig_property(_rf21x.RF21xMessage_rawData_get, _rf21x.RF21xMessage_rawData_set)
    def __init__(self): 
        this = _rf21x.new_RF21xMessage()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _rf21x.delete_RF21xMessage
    __del__ = lambda self : None;
RF21xMessage_swigregister = _rf21x.RF21xMessage_swigregister
RF21xMessage_swigregister(RF21xMessage)

class RF21xDevice(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, RF21xDevice, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, RF21xDevice, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _rf21x.new_RF21xDevice()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _rf21x.delete_RF21xDevice
    __del__ = lambda self : None;
    def open(self, *args): return _rf21x.RF21xDevice_open(self, *args)
    def close(self): return _rf21x.RF21xDevice_close(self)
    def startQuiz(self, *args): return _rf21x.RF21xDevice_startQuiz(self, *args)
    def startDirectly(self, *args): return _rf21x.RF21xDevice_startDirectly(self, *args)
    def stopQuiz(self): return _rf21x.RF21xDevice_stopQuiz(self)
    def setKeypadId(self, *args): return _rf21x.RF21xDevice_setKeypadId(self, *args)
    def readDeviceSerialNumber(self, *args): return _rf21x.RF21xDevice_readDeviceSerialNumber(self, *args)
    def writeDeviceSerialNumber(self, *args): return _rf21x.RF21xDevice_writeDeviceSerialNumber(self, *args)
    def resetDeviceSerialNumber(self, *args): return _rf21x.RF21xDevice_resetDeviceSerialNumber(self, *args)
    def checkDeviceSerialNumber(self, *args): return _rf21x.RF21xDevice_checkDeviceSerialNumber(self, *args)
    def getMessage(self, *args): return _rf21x.RF21xDevice_getMessage(self, *args)
RF21xDevice_swigregister = _rf21x.RF21xDevice_swigregister
RF21xDevice_swigregister(RF21xDevice)

# This file is compatible with both classic and new-style classes.


