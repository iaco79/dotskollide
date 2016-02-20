LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := tonatiuh_static

COMMON= GGameManagerTonatiuh.cpp TControllerFactory.cpp  TElementsFactory.cpp TGameStateDemo.cpp TGameStateDemoCrashLogic.cpp TObjectFactory.cpp TGoalManager.cpp

COMPONENTS= component/TSpringGridComponent.cpp

CONTROLLERS= controller/TFireworkController.cpp controller/TShieldController.cpp controller/TElementController.cpp controller/TPlayerController.cpp controller/TSynthetizerController.cpp controller/TBitController.cpp controller/TEnergyBarController.cpp controller/TElementCreatorController.cpp

IRRNODE= scenenode/SpringNode.cpp scenenode/ShieldNode.cpp scenenode/FireworkNode.cpp scenenode/EnergyBarNode.cpp scenenode/SpiralNode.cpp scenenode/JointsNode.cpp

SPRINGGRID= springrid/SpringGrid.cpp spline/spline.cpp

SHADERS= shaders/FireworkShader.cpp shaders/SpiralShader.cpp shaders/MetaballShaderPre.cpp shaders/MetaballShader.cpp

GAMESTATES= gamestate/TGameStateExtras.cpp gamestate/TGameStateOptions.cpp gamestate/TGameStateSelectGame.cpp gamestate/TGameStateMainMenu.cpp

TONATIUFULL = $(COMMON) $(COMPONENTS) $(CONTROLLERS) $(IRRNODE) $(SPRINGGRID) $(SHADERS) $(GAMESTATES)


LOCAL_SRC_FILES := $(TONATIUFULL)

LOCAL_ARM_MODE   := arm 
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_CFLAGS := -O3 -DANDROID_NDK -DDISABLE_IMPORTGL -I./../include/ -I./jni/ -I./jni/box2d/ -I./jni/irrlicht -I./jni/irrlicht/include/ -I./jni/freetype2_static/include -I./jni/freetype2_static/src -I./jni/librocket/include -I./jni/SDL/include -I./jni/SDL_Mixer -I./jni/jsoncpp-src-0.5.0/include -I./jni/Game  -I./jni/oolua/include -I./jni/lua-5.2.2/src




include $(BUILD_STATIC_LIBRARY)

