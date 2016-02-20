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
#include "GGameAppWin.h"
#include "GGameManager.h"
#include "menu/GRocketEvent.h"
#include "menu/GRocketEventInstancer.h"
#include "rocketgui/GRocketElementGame.h"
#include "preferences/PreferencesManager.h"
#include "SDL.h"
#include "sdl/SDLFileInterface.h"


#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
//#include "vld.h"
#endif


extern "C" {
	void InitializeFileSystemExtern(irr::io::IFileSystem* fs);
};


extern GGameManager* GGameApp_CreateGameManager(GGameApp* app);

void InitializeFileSystemExtern(irr::io::IFileSystem* fs)
{
	if(fs)
	{
		fs->changeWorkingDirectoryTo(".");
		//fs->addFileArchive("IrrlichtSdcard.zip");
	}
	
}




class PreferencesWin: public IPreferences
{
	int mValues[10];

public:

	PreferencesWin()
	{
	
		for(int i=0; i<10;i++)
		{
			mValues[i]=1;
		}

	}
	virtual void setPreference(int preferenceID, int value) 
	{
		if(preferenceID>=0 && preferenceID<=3)
		{
			mValues[preferenceID] = value;
		}

	}
	virtual int  getPreference(int preferenceID) 
	{
		int value = 0;

		if(preferenceID>=0 && preferenceID<=3)
		{
			value = mValues[preferenceID];
		}

		return value;

	}

	virtual void vibrate() {;}
	
};



GGameAppWin::GGameAppWin(): GGameApp(),mFirstTime(true)
{
	
}
void GGameAppWin::Pause()
{
	//pause timer
	if(mGameManager)
		mGameManager->VPause();


}
void GGameAppWin::Resume()
{
	//resume timer
	if(mGameManager)
		mGameManager->VResume();


}

bool GGameAppWin::Render()
{
	//this will dispatch all the events from the event manager to all the registered receivers
	bool result = mDevice->run(); 
	irr::u32 timeStep = 16;// 16ms = 60fps 
	irr::u32 elapsedTime;
	
	if(result)
	{
		if(mFirstTime)
		{
			mLastTime = mDevice->getTimer()->getTime();
			elapsedTime =0;
			mFirstTime=false;
		}
		
		if(mGameManager)
			mGameManager->VUpdate(elapsedTime, mDevice->getTimer()->getTime());

		elapsedTime = mDevice->getTimer()->getTime() - mLastTime;
		mLastTime = mDevice->getTimer()->getTime();


		if (elapsedTime < timeStep)
		{
			mDevice->sleep(timeStep - elapsedTime);
		}

	}

	return result;

};
void GGameAppWin::Init()
{

	
	//initialize irrlicht
	

	mDevice = irr::createDevice( irr::video::EDT_OGLES2, 
								 irr::core::dimension2d<irr::u32>(512, 800), 16, 
								 false, 
								 false, 
								 false, 
								 NULL);
	
	SDLFileInterface::Initialize(mDevice->getFileSystem());

	
	//create the event manager
	mEventManager = new GEventManager();

	//create the script manager 
	mScriptManager = new GScriptManager();
	mScriptManager->Initialize(mDevice);

	//create librocket interface

	mRocketGUI = new CIrrRocketGUI(mDevice);
	mRocketGUI->Initialize();

	//crete the preferences Manager
	mPreferencesManager = PreferencesManager::getInstance();
	mPreferencesManager->initialize(new PreferencesWin());


	mDevice->sleep(1000);

	
	//initialize the audio system
	mAudioManager = AudioManager::getInstance();
	if(mAudioManager) 	
	{
		if (!mAudioManager->initialize())
		{	
			return;
		}
		
	}	

	//create the game manager
	mGameManager = GGameApp_CreateGameManager(this);

	//initialize the game manager
	if(mGameManager)
		mGameManager->VInit();

}
void GGameAppWin::Finish()
{
	if(mDevice)
		mDevice->drop();
	

	if(mAudioManager)	
	{
		mAudioManager->shutDown();
		delete mAudioManager;

	}
	
}