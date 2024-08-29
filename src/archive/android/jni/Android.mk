LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE := libusb
LOCAL_SRC_FILES := extlibs/$(TARGET_ARCH_ABI)/libusb.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := libiconv
LOCAL_SRC_FILES := extlibs/$(TARGET_ARCH_ABI)/libiconv.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := rf21x-wrap
LOCAL_SRC_FILES := \
    wcsdup.c \
    ../../src/tthread/tinythread.cpp \
    ../../src/gsp/hidapi/linux/hid-libusb.c \
    ../../src/SimpleSocket/SimpleSocket.cpp ../../src/SimpleSocket/PassiveSocket.cpp ../../src/SimpleSocket/ActiveSocket.cpp \
    ../../src/gsp/abstract-gsp.cpp ../../src/gsp/gsp.cpp ../../src/gsp/gsp-hid.cpp ../../src/gsp/gsp-socket.cpp \
    ../../src/rf21x/rf21x-base.cpp ../../src/rf21x/rf21x.cpp ../../src/rf21x/rf215.cpp ../../src/rf21x/rf217.cpp ../../src/rf21x/rf218.cpp ../../src/rf21x/rf219.cpp \
    ../../src/rf21x-c/rf21x-api.cpp ../../src/rf21x-wrap/rf21x-wrap.cpp \
    rf21x_wrap_wrap.cpp
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../../src/tthread \
    $(LOCAL_PATH)/../../src/gsp/hidapi \
    $(LOCAL_PATH)/../../src/SimpleSocket \
    $(LOCAL_PATH)/../../src/gsp \
    $(LOCAL_PATH)/../../src/gsp/socket \
    $(LOCAL_PATH)/../../src/rf21x \
    $(LOCAL_PATH)/../../src/rf21x-c \
    $(LOCAL_PATH)/../../src/rf21x-wrap
LOCAL_CFLAGS += -fexceptions -fpermissive -D_LINUX
LOCAL_SHARED_LIBRARIES := libiconv libusb 
#LOCAL_STATIC_LIBRARIES := libiconv libusb 
#LOCAL_LDLIBS += -llog -lpthread
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../src/rf21x-c $(LOCAL_PATH)/../../src/rf21x-wrap

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := rf21x-wrap-test
LOCAL_SRC_FILES := ../../src/rf21x-wrap/rf21x-wrap-test.cpp
LOCAL_SHARED_LIBRARIES := rf21x-wrap

include $(BUILD_EXECUTABLE)
