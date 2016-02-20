LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := oolua_static

OOLUA1= src/class_from_stack.cpp src/lua_function.cpp src/lua_ref.cpp src/lua_stack_dump.cpp src/lua_table.cpp src/oolua.cpp src/oolua_check_result.cpp
OOLUA2= src/oolua_error.cpp src/oolua_push_pull.cpp src/oolua_registration.cpp src/oolua_storage.cpp src/push_pointer_internal.cpp

OOLUAFULL = $(OOLUA1) $(OOLUA2) 


LOCAL_SRC_FILES := $(OOLUAFULL)


LOCAL_ARM_MODE   := arm 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CFLAGS := -O3 -I./jni
LOCAL_CFLAGS := -O3 -I./jni/lua-5.2.2/src


include $(BUILD_STATIC_LIBRARY)



