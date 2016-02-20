LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := jsoncpp_static

SRC= src/lib_json/json_reader.cpp src/lib_json/json_value.cpp src/lib_json/json_writer.cpp



LOCAL_SRC_FILES := $(SRC)

LOCAL_ARM_MODE   := arm 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CFLAGS := -O3 -I./jni


include $(BUILD_STATIC_LIBRARY)

