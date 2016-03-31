LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
APP_ABI := armeabi-v7a
APP_PLATFORM := android-18
LOCAL_CFLAGS += -fPIE -pie 
LOCAL_MODULE := samsung_cid
LOCAL_LDFLAGS := -pie  

LOCAL_SRC_FILES :=  samsung_cid.c  
LOCAL_LDLIBS += 
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
APP_ABI := armeabi-v7a
APP_PLATFORM := android-18
LOCAL_CFLAGS += -fPIE -pie 
LOCAL_MODULE := samsung_unlock
LOCAL_LDFLAGS := -pie  

LOCAL_SRC_FILES :=  samsung_unlock.c  
LOCAL_LDLIBS += 
include $(BUILD_EXECUTABLE)
