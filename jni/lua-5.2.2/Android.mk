LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := lua_static

LUA1= src/lapi.c src/lauxlib.c src/lbaselib.c src/lbitlib.c src/lcode.c src/lcorolib.c src/lctype.c src/ldblib.c src/ldebug.c src/ldo.c
LUA2= src/ldump.c src/lfunc.c src/lgc.c src/linit.c src/liolib.c src/llex.c src/lmathlib.c src/lmem.c src/loadlib.c src/lobject.c  src/lopcodes.c
LUA3= src/loslib.c src/lparser.c src/lstate.c src/lstring.c src/lstrlib.c src/ltable.c src/ltablib.c src/ltm.c src/lua.c src/luac.c src/lundump.c src/lvm.c src/lzio.c

LUAFULL = $(LUA1) $(LUA2) $(LUA3)


LOCAL_SRC_FILES := $(LUAFULL)


LOCAL_ARM_MODE   := arm 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_CFLAGS := -O3 -I./jni


include $(BUILD_STATIC_LIBRARY)

