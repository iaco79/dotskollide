/*
  Copyright (C) 2015 Othon Insauste
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
 
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  Othon Insauste othonic@gmail.com
*/
#include <jni.h>
#include <unistd.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "SDL.h"
#include "SDL_mixer.h"
#include <irrlicht.h>
#include <CIrrDeviceStub.h>
#include <COGLES2Driver.h>
#include <os.h>
#include "GameDefines.h"
#include "GGameAppAndroid.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define ACTION_DOWN 0x00
#define ACTION_UP 0x01
#define ACTION_MOVE 0x02
#define ACTION_VIRTUAL_BUTTON 7777000


extern "C" void SDL_Android_Init(JNIEnv* env, jclass cls);

extern "C" void SDL_Android_Init_AudioThread(JNIEnv* mEnv, jclass cls);

int importGLInit();
void importGLDeinit();
void nativeDrawIteration();
void nativeInitialize(int width,int height);

// global variables
int  gWindowWidth  = 640;
int  gWindowHeight = 480;
int  gAppAlive   = 1;
int  lastTouchX;
int  lastTouchY;
int  gRequireContext = 0; 

static pthread_key_t tonatiuh_ThreadKey;
static JNIEnv* tonatiuh_JEnv;
static jclass tonatiuh_ActivityClass;


// method signatures
static jmethodID tonatiuh_getPreference; //get preferences value
static jmethodID tonatiuh_setPreference; //get preferences value
static jmethodID tonatiuh_vibrate; //vibrate
static jmethodID tonatiuh_gotoExternal; //scorelopp, rate the app

stringc gSdCardPath = "/sdcard/";

AAssetManager* mAssetManager=NULL;

IrrlichtDevice* ggDevice = NULL;


/* For JNI: C++ compiler need this */
extern "C" {


void Android_Tonatiuh_SetPreferenceValue(int preferenceId, int value)
{
	tonatiuh_JEnv->CallStaticVoidMethod(tonatiuh_ActivityClass, tonatiuh_setPreference, preferenceId, value);

}
int Android_Tonatiuh_GetPreferenceValue(int preferenceId)
{
	int value = tonatiuh_JEnv->CallStaticIntMethod(tonatiuh_ActivityClass, tonatiuh_getPreference, preferenceId);

	return value;
}

void Android_Tonatiuh_Vibrate()
{
	tonatiuh_JEnv->CallStaticVoidMethod(tonatiuh_ActivityClass, tonatiuh_vibrate);
}

/** Activity onCreate */
void Java_com_iaco_tonatiuh_GameActivity_nativeOnCreate( JNIEnv*  env,jobject defaultObj, jobject assetManager)
{
    

	//pass the assets manager	
	mAssetManager = AAssetManager_fromJava(env, assetManager);
}

/** Activity onPause */
void Java_com_iaco_tonatiuh_GameActivity_nativeOnPause( JNIEnv*  env )
{

	if(GGameApp::getInstance())
		GGameApp::getInstance()->Pause();
}

/** Activity onResume */
void Java_com_iaco_tonatiuh_GameActivity_nativeOnResume( JNIEnv*  env )
{
	if(GGameApp::getInstance())
		GGameApp::getInstance()->Resume();
}

//IACO add to init SDL

void Java_com_iaco_tonatiuh_GameActivity_nativeRunAudioThread( JNIEnv*  env, jclass cls, jobject obj )
{
	SDL_Android_Init_AudioThread(env, cls);

}
void Java_com_iaco_tonatiuh_GameActivity_nativeInitSDL( JNIEnv*  env, jclass cls, jobject obj )
{
    /* This interface could expand with ABI negotiation, calbacks, etc. */
    SDL_Android_Init(env, cls);

    /* Run the application code! */
	/*
    int status;
    char *argv[2];
    argv[0] = strdup("SDL_app");
    argv[1] = NULL;
    status = SDL_main(1, argv);
	*/

    /* Do not issue an exit or the whole application will terminate instead of just the SDL thread */
    //exit(status);
}

/** Activity onDestroy */
void Java_com_iaco_tonatiuh_GameActivity_nativeOnDestroy( JNIEnv*  env )
{
    importGLDeinit();
}

void Java_com_iaco_tonatiuh_GameActivity_nativeInitGL( JNIEnv*  env , jobject obj )
{
	tonatiuh_JEnv = env;		

	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "getting Activity Ref");
	tonatiuh_ActivityClass = env->GetObjectClass(obj);
	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "getting getPreferenceValue Ref");
	tonatiuh_getPreference = env->GetStaticMethodID(tonatiuh_ActivityClass,  "getPreferenceValue", "(I)I");		
	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "getting setPreferenceValue Ref");
	tonatiuh_setPreference  = env->GetStaticMethodID(tonatiuh_ActivityClass,  "setPreferenceValue", "(II)V");
	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "getting vibrate Ref");
	tonatiuh_vibrate = env->GetStaticMethodID(tonatiuh_ActivityClass,  "vibrate", "()V");



	if(gRequireContext == 0)
	{
		 __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "nativeInitGL");
		
		gRequireContext = 1;
		importGLInit();
		lastTouchX=0;
		lastTouchY=0;
		nativeInitialize(gWindowWidth, gWindowHeight);	    
		ggDevice = GGameApp::getInstance()->getIrrlichtDevice();
	}
	else
	{
		irr::core::dimension2d<unsigned int> size;
		size.Width = gWindowWidth;
		size.Height = gWindowHeight; 
		ggDevice->getVideoDriver()->OnResize(size);

		COGLES2Driver* gles2driver =  (COGLES2Driver*)ggDevice->getVideoDriver();
		
		
		//delete all materials 
		GGameApp::getInstance()->ReleaseRenderContext();

		if(gles2driver)
		{
			gles2driver->genericDriverInit(size, false);
		}

		GGameApp::getInstance()->ReInitRenderContext();


	}

}

void Java_com_iaco_tonatiuh_GameActivity_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
	
    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "resize w=%d h=%d", w, h);
    gWindowWidth  = w;
    gWindowHeight = h;
    irr::core::dimension2d<unsigned int> size;
    size.Width = w;
    size.Height = h; 

    ggDevice->getVideoDriver()->OnResize(size);
}

void Java_com_iaco_tonatiuh_GameActivity_nativeSendEvent( JNIEnv*  env, jobject defaultObj, jobject event) 
{
    jclass classEvent = env->GetObjectClass(event);
    jfieldID fieldAction = env->GetFieldID(classEvent, "mAction", "I");
    jfieldID fieldX = env->GetFieldID(classEvent, "mX", "F");
    jfieldID fieldY = env->GetFieldID(classEvent, "mY", "F");
	jfieldID fieldSpeedX = env->GetFieldID(classEvent, "mSpeedX", "F");
    jfieldID fieldSpeedY = env->GetFieldID(classEvent, "mSpeedY", "F");

    int action = env->GetIntField(event, fieldAction);
    // convert Android coord to OpenGL coords
    float x = env->GetFloatField(event, fieldX);
    float y = env->GetFloatField(event, fieldY);
	float sx =  env->GetFloatField(event, fieldSpeedX);
	float sy =  env->GetFloatField(event, fieldSpeedY);

    SEvent irrevent;
    irrevent.MouseInput.ButtonStates = 0xffffffff;
    irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
    irrevent.MouseInput.Event = irr::EMIE_COUNT;
	irrevent.MouseInput.X = x;
	irrevent.MouseInput.Y = y;


	if(action == ACTION_VIRTUAL_BUTTON)
	{

		//this is the back boton
		irrevent.EventType = irr::EET_KEY_INPUT_EVENT;
		irrevent.KeyInput.Key = irr::KEY_KEY_B;		
		irrevent.KeyInput.PressedDown = false;
	    ((CIrrDeviceStub*) ggDevice)->enqueueEventsFromUser(irrevent);
		//device->postEventFromUser(irrevent);
		

	}
	else  if (action==ACTION_DOWN)
	{
		lastTouchX=x;
		lastTouchY=y;
		//__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "touch down %d %f %f", action, x, y);

		//send a mouse move
		irrevent.MouseInput.ButtonStates = 0;
		irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
		irrevent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
		irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;  			
		((CIrrDeviceStub*) ggDevice)->enqueueEventsFromUser(irrevent);


		//+ send a mouse down
        irrevent.MouseInput.ButtonStates = 0;
	    irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
        irrevent.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN; 
		irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;   
		

		((CIrrDeviceStub*) ggDevice)->enqueueEventsFromUser(irrevent);
		
		
	    
		
    } 
	else if (action==ACTION_UP)
	{
		lastTouchX=x;
		lastTouchY=y;

        irrevent.MouseInput.ButtonStates = 0;
        irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
        irrevent.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
		irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;  
        //device->postEventFromUser(irrevent);
		((CIrrDeviceStub*) ggDevice)->enqueueEventsFromUser(irrevent);
		//__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "touch up %d %f %f", action, x, y);

    } 	
	else if (action==ACTION_MOVE)
	{ //move

		if(x!= lastTouchX || y!=lastTouchY)
		{
			irrevent.MouseInput.ButtonStates = 0;
			irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
			irrevent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
			irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;  	
			irrevent.MouseInput.SX = sx;
			irrevent.MouseInput.SY = sy;


			((CIrrDeviceStub*) ggDevice)->enqueueEventsFromUser(irrevent);

			lastTouchX=x;
			lastTouchY=y;
		}
		

		 // __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "touch move %d %f %f", action, x, y);
	}
	else 
	{
        //__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "not handled %d", action);   
    }    
}

void Java_com_iaco_tonatiuh_GameActivity_nativeGetStatus(JNIEnv*  env, jobject defaultObj, jobject status) 
{
    if (gAppAlive==0) {
        jclass classStatus = env->GetObjectClass(status);
        jfieldID fieldStatus = env->GetFieldID(classStatus, "mQuit", "Z");
        env->SetBooleanField(status, fieldStatus, true);
    }    
}


void Java_com_iaco_tonatiuh_GameActivity_nativeEnvJ2C(JNIEnv*  env, jobject defaultObj, jstring jsdcardPath) 
{
    char ligne[1024+1];
    const char *msg = env->GetStringUTFChars(jsdcardPath,0);
    gSdCardPath = msg;
    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "not handled %s", gSdCardPath.c_str());
    env->ReleaseStringUTFChars(jsdcardPath,msg);
}

void Java_com_iaco_tonatiuh_GameActivity_nativeDrawIteration( JNIEnv*  env ) 
{
    nativeDrawIteration();   
}

}
