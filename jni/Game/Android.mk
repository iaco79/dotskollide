LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := game_static

GAMECORE= GGameAppAndroid.cpp GBox2dComponent.cpp GComponent.cpp GController.cpp GControllerComponent.cpp GControllerFactory.cpp GEventManager.cpp GGameApp.cpp GGameManager.cpp GGameState.cpp GIrrlichtComponent.cpp GNodeFactory.cpp GObject.cpp GObjectFactory.cpp GSpriteComponent.cpp GTransformComponent.cpp

AUDIO= audio/AudioManager.cpp

IRRNODE= irrnode/B2dNode.cpp irrnode/Base2dNode.cpp irrnode/BatchedSpriteNode.cpp irrnode/SpriteNode.cpp

MENU= menu/GRocketEvent.cpp menu/GRocketEventHandler.cpp menu/GRocketEventInstancer.cpp

ROCKETGUI= rocketgui/CIrrRocketEventReceiver.cpp rocketgui/CIrrRocketGUI.cpp rocketgui/GRocketElementGame.cpp rocketgui/IrrRocketFileInterface.cpp rocketgui/IrrRocketInterface.cpp rocketgui/IrrRocketRenderer.cpp

SCRIPTING= scripting/GScriptable.cpp scripting/GScriptManager.cpp  

SDLFILE= sdl/SDLFileInterface.cpp sdl/SDLFS.cpp preferences/PreferencesManager.cpp

GAMEFULL = $(GAMECORE) $(AUDIO) $(IRRNODE) $(MENU) $(ROCKETGUI) $(SCRIPTING) $(SDLFILE)


LOCAL_SRC_FILES := $(GAMEFULL)

LOCAL_ARM_MODE   := arm 
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_CFLAGS := -O3 -DANDROID_NDK -DDISABLE_IMPORTGL -I./../include/ -I./jni/ -I./jni/box2d/ -I./jni/irrlicht/ -I./jni/irrlicht/include/ -I./jni/freetype2_static/include -I./jni/freetype2_static/src -I./jni/librocket/include -I./jni/SDL/include -I./jni/SDL_Mixer -I./jni/jsoncpp-src-0.5.0/include -I./jni/oolua/include -I./jni/lua-5.2.2/src





include $(BUILD_STATIC_LIBRARY)

