LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := motor-jni
LOCAL_SRC_FILES := motor-jni.c
LOCAL_LDLIBS    := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)