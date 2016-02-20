# 
# COSOperator.cpp was removed from IRROTHEROBJ
LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

ANDROID = app-android.cpp android-activity.cpp


LOCAL_MODULE := gamedemo
LOCAL_ARM_MODE   := arm 
LOCAL_CFLAGS := -O3 -DANDROID_NDK -DDISABLE_IMPORTGL  -I./jni/ -I./jni/box2d/ -I./jni/irrlicht/ -I./jni/irrlicht/include/ -I./jni/freetype2_static/include -I./jni/freetype2_static/src -I./jni/librocket/include -I./jni/SDL/include -I./jni/SDL_Mixer -I./jni/jsoncpp-src-0.5.0/include -I./jni/Game  -I./jni/oolua/include -I./jni/lua-5.2.2/src

													

LOCAL_SRC_FILES := $(ANDROID)


LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lGLESv2 -landroid
LOCAL_STATIC_LIBRARIES := \
				libsdl2_static \
				tonatiuh_static \
				game_static \
				tonatiuh_static \
				game_static \
				jsoncpp_static \
                librocket_static \
				libfreetype2_static \
				libsdl2_mixer_static \
                libsdl2_static \
                libtremor_static \
				libogg_static \
				irrlicht_static \
				box2d_static \
				oolua_static \
				lua_static			

include $(BUILD_SHARED_LIBRARY)


$(call import-module,lua-5.2.2)
$(call import-module,oolua)
$(call import-module,box2d)
$(call import-module,irrlicht)
$(call import-module,freetype2_static)
$(call import-module,librocket)
$(call import-module,libogg-1.3.0)
$(call import-module,tremor)
$(call import-module,sdl)
$(call import-module,sdl_mixer)
$(call import-module,jsoncpp-src-0.5.0)
$(call import-module,game)
$(call import-module,tonatiuh)

