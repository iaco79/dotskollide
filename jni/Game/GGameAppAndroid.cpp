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
#include "CNullDriver.h"
#include <CIrrDeviceStub.h>
#include <os.h>
#include "CFileSystem.h"
#include "Android/CAndroidAssetReader.h"
#include "Android/CAndroidAssetFileArchive.h"
#include "preferences/PreferencesManager.h"
#include "GGameAppAndroid.h"
#include "GGameManager.h"
#include "menu/GRocketEvent.h"
#include "menu/GRocketEventInstancer.h"
#include "rocketgui/GRocketElementGame.h"
#include "SDL.h"
#include "sdl/SDLFileInterface.h"

extern GGameManager* GGameApp_CreateGameManager(GGameApp* app);

bool gAppInitialized = false;

extern AAssetManager* mAssetManager;

extern "C" {
	void InitializeFileSystemExtern(irr::io::IFileSystem* fs);
};



extern "C" {
	void Android_Tonatiuh_SetPreferenceValue(int preferenceId, int value);
};

extern "C" {
	int Android_Tonatiuh_GetPreferenceValue(int preferenceId);
};
extern "C" {
	void Android_Tonatiuh_Vibrate();
};

class PreferencesAndroid: public IPreferences
{

public:
	virtual void setPreference(int preferenceID, int value) 
	{
		Android_Tonatiuh_SetPreferenceValue(preferenceID, value);

	}
	virtual int  getPreference(int preferenceID) 
	{
		return Android_Tonatiuh_GetPreferenceValue(preferenceID);
	}
	
	virtual void vibrate()
	{
		Android_Tonatiuh_Vibrate();

	}

};



//intialize the apk file system
void InitializeFileSystemExtern(irr::io::IFileSystem* fs)
{
	if(fs)
	{
		irr::io::CAndroidAssetFileArchive* assets = NULL;
		
		assets = new irr::io::CAndroidAssetFileArchive(mAssetManager, false, false);
		
		assets->addDirectoryToFileList("");
		//assets->addDirectoryToFileList("sounds");		
		fs->addFileArchive(assets);
		assets->drop();	
	}

}


GGameAppAndroid::GGameAppAndroid(int defaultWidth, int defaultHeight): GGameApp(),mFirstTime(true)
{
	mDefaultWidth = defaultWidth;
	mDefaultHeight =defaultHeight;
}
void GGameAppAndroid::Pause()
{
	if(mGameManager)
		mGameManager->VPause();

}
void GGameAppAndroid::Resume()
{
	if(mGameManager)
		mGameManager->VResume();


}


void GGameAppAndroid::ReleaseRenderContext()
{
	//pause timer
	if (gAppInitialized)
	{	
		if(mGameManager)
			mGameManager->VReleaseRenderContext();

	}
}
void GGameAppAndroid::ReInitRenderContext()
{
	//resume timer
	if (gAppInitialized)
	{	
		if(mGameManager)
			mGameManager->VReInitRenderContext();


	}

}


void GGameAppAndroid::Init()
{
	//initialize irrlicht
	mDevice = irr::createDevice( irr::video::EDT_OGLES2, irr::core::dimension2d<irr::u32>(mDefaultWidth, mDefaultHeight), 16, false, false, false, NULL);
	
	SDLFileInterface::Initialize(mDevice->getFileSystem());


	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "createDevice r=%d w=%d h=%d", mDevice, mDefaultWidth, mDefaultHeight);
	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "getVideoDriver r=%d", mDevice->getVideoDriver());

    if (!mDevice)
        __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "No device");
	mElapsedTime =0;

}



bool GGameAppAndroid::Render()
{

	if (!gAppInitialized)
	{	
		//create the event manager
		mEventManager = new GEventManager();

		//create the script manager 
		mScriptManager = new GScriptManager();
		mScriptManager->Initialize(mDevice);

		//create librocket interface

		mRocketGUI = new CIrrRocketGUI(mDevice);
		mRocketGUI->Initialize();


		

		mDevice->sleep(1000);

		
		irr::core::stringc myDir = "/mnt/sdcard/Irrlicht2";
		
		//initialize the file system
		mDevice->getFileSystem()->changeWorkingDirectoryTo(myDir.c_str());
	
		//initialize the audio system
		mAudioManager = AudioManager::getInstance();
		if(mAudioManager) 	
		{
			if (!mAudioManager->initialize())
			{	
		//IACO TODO		return;
			}		
		}
		
		//crete the preferences Manager
		mPreferencesManager = PreferencesManager::getInstance();
		mPreferencesManager->initialize(new PreferencesAndroid());
		
		//create the game manager
		mGameManager = GGameApp_CreateGameManager(this);

		//initialize the game manager
		if(mGameManager)
			mGameManager->VInit();

		 gAppInitialized=true;
	}

	if(gAppInitialized)
	{

		//this will dispatch all the events from the event manager to all the registered receivers
		bool r = mDevice->run(); 
		irr::u32 timeStep = 16; // 16ms = 60fps 
		
		if(r)
		{
			((irr::CIrrDeviceStub*) getIrrlichtDevice())->dispatchEventsFromUser();


			if(mFirstTime)
			{
				mLastTime = mDevice->getTimer()->getTime();
				mElapsedTime =0;
				mFirstTime=false;
			}
		
			if(mGameManager)
				mGameManager->VUpdate(mElapsedTime, mDevice->getTimer()->getTime());

			mElapsedTime = mDevice->getTimer()->getTime() - mLastTime;
			mLastTime = mDevice->getTimer()->getTime();


			if (mElapsedTime < timeStep)
			{
			//	mDevice->sleep(timeStep - elapsedTime);
			}
		}

	}

	return true;
}

void GGameAppAndroid::Finish()
{
	if(mDevice)
		mDevice->drop();
	

	if(mAudioManager)	
	{
		mAudioManager->shutDown();
		delete mAudioManager;

	}
	
}