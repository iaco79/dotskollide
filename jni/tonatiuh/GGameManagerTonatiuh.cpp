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
#include "os.h"
#include "GObjectFactory.h"
#include "GNodeFactory.h"
#include "GController.h"
#include "GControllerFactory.h"
#include "GGameManagerTonatiuh.h"
#include "TControllerFactory.h"
#include "TObjectFactory.h"
#include "preferences/PreferencesManager.h"
#include "shaders/FireworkShader.h"
#include "shaders/SpiralShader.h"
#include "shaders/MetaballShader.h"
#include "shaders/MetaballShaderPre.h"
#include "TGoalManager.h"
#include "spline/spline.h"
#include "gamestate/TGameStateMainMenu.h"
#include "gamestate/TGameStateSelectGame.h"
#include "gamestate/TGameStateOptions.h"
#include "gamestate/TGameStateExtras.h"
#include "TGameStateDemo.h"


//this will create the instance
GGameManager* GGameApp_CreateGameManager(GGameApp* app)
{
	
	return new GGameManagerTonatiuh(app);
}


GGameManagerTonatiuh::GGameManagerTonatiuh(GGameApp* app): GGameManager(app)
{

	//intialize the factories
	app->mControllerFactory = new TControllerFactory();
	app->mNodeFactory = new GNodeFactory(app->getIrrlichtDevice()->getSceneManager());
	GObjectFactory::Initialize(new TObjectFactory(app->getIrrlichtDevice()->getFileSystem()));
	app->mObjectFactory = GObjectFactory::getInstance();

	mLastGameStateIndex= GAME_STATE_NONE;
	mCurrentGameStateIndex= GAME_STATE_NONE;
	mApp= app;

	mCurrentGameStateParam = 0;
	mFirstLoop=true;
	

}


//initialize the objects
void GGameManagerTonatiuh::VInit()
{




	//initialize the custom shaders
	irr::video::IGPUProgrammingServices* gpu = mApp->getIrrlichtDevice()->getVideoDriver()->getGPUProgrammingServices();		
	if (gpu)
	{	
		FireWorkShader::Initialize();
		SpiralShader::Initialize();
		MetaballShader::Initialize();
		MetaballShaderPre::Initialize();
	}
	 

	//initialize the game states 
	LoadGameStates();

	//load the music
	#ifndef _IRR_ANDROID_PLATEFORM_
		mApp->getAudioManager()->loadMusic("IrrlichtSdcard/backmusic14.ogg");
	#else
		mApp->getAudioManager()->loadMusic("backmusic14.ogg");
	#endif


}

void GGameManagerTonatiuh::LoadGameStates()
{
	
	GGameState*  gameState = new  TGameStateMainMenu();
	gameState->VInit();
	mGameStates.push_back(gameState);
	
	
	gameState = new  TGameStateSelectGame();
	gameState->VInit();
	mGameStates.push_back(gameState);
	
	
	gameState = new TGameStateOptions();
	gameState->VInit();
	mGameStates.push_back(gameState);
	
	gameState = new TGameStateExtras();
	gameState->VInit();
	mGameStates.push_back(gameState);
	
	

	gameState = new TGameStateDemo();
	gameState->VInit();
	mGameStates.push_back(gameState);
	
	
	VSetNextGameState(GAME_STATE_MAINMENU,0);

}



void GGameManagerTonatiuh::VFinish()
{
	//finalize and delete game states

	for(int i=0;i<mGameStates.size();i++)
	{
		GGameState* gs = mGameStates[i];
		gs->VFinish();
		delete gs;
	}

	mGameStates.clear();
}


//process a frame 
void GGameManagerTonatiuh::VUpdate(irr::u32 deltaMS, irr::u32 elapsedTimeMS)
{
	irr::IrrlichtDevice* device = mApp->getIrrlichtDevice();
	irr::video::IVideoDriver *driver = device->getVideoDriver();	
	irr::gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
	GGameState* gs =  GetCurrentGameState(); 	
	
	GGameState* gameplayState =  GetGameStateById(GAME_STATE_GAME);


	if(mFirstLoop)
	{	
		ActivateSoundIfNecessary();
		mFirstLoop=false;
	}


	//if user has changed game state
	if(mCurrentGameStateIndex != mLastGameStateIndex)
	{
		
		GGameState* lastGS=  GetLastGameState();
		//there is a change in the game state
		if(lastGS!=NULL)
		{
			lastGS->VOnLostFocus();
		}

		GGameState* demoGS = GetGameStateById(GAME_STATE_GAME);

		if((gs->VGameStateId() == GAME_STATE_MAINMENU) && demoGS)
		{
			TGameStateDemo* gsDemo = (TGameStateDemo*)demoGS;
			mCurrentGameStateParam = (gsDemo->GetGameMode()==2);
			gs->VOnGetFocus(mCurrentGameStateParam);
		}
		else
		{
			gs->VOnGetFocus(mCurrentGameStateParam);
		}

	}

	mLastGameStateIndex = mCurrentGameStateIndex;

	
	if(gs!=NULL)
		gs->VUpdate(deltaMS);


	//

	if(driver)
	{
		driver->beginScene(true, true, irr::video::SColor(255,0,0,0));
		
		//always draw the game state
		if(mCurrentGameStateIndex != GAME_STATE_GAME)
		{
			//gameplayState->VRender(device);
		}


		if(gs)
			gs->VRender(device);

		
		
		if(mApp->mRocketGUI)
		{	
			mApp->mRocketGUI->run();
		}

		if (font)
		{
			int w = mApp->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Width;
			int h = mApp->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Height;

			font->draw(L"Dots Kollide v0.9.0 Demo, by IACO79.",
            core::rect<s32>(w/4,h-50, w,h),
            video::SColor(255,255,255,255));
		}


		driver->endScene();



	}
	//update audio

	mApp->getAudioManager()->update();


}
void GGameManagerTonatiuh::VPause()
{
	irr::os::Printer::log("GGameManagerTonatiuh VPause");

	mApp->getAudioManager()->pauseAudio();
}
void GGameManagerTonatiuh::VResume()
{
	irr::os::Printer::log("GGameManagerTonatiuh VResume");
	mApp->getAudioManager()->resumeAudio();

}
	
void GGameManagerTonatiuh::ActivateSoundIfNecessary()
{
	
	bool sound = ((mApp->getPreferencesManager()->getPreference((int)GAME_PREFERENCES_SFX,1))==1);
	bool music = ((mApp->getPreferencesManager()->getPreference((int)GAME_PREFERENCES_MUSIC,1))==1);		
	mApp->getAudioManager()->enableSound(sound);
	mApp->getAudioManager()->enableMusic(music);
}
	



void GGameManagerTonatiuh::VSetNextGameState(int gameState, int param)
{	
	mCurrentGameStateIndex = (GAME_STATES)gameState;
	mCurrentGameStateParam = param;


}

GGameState*	GGameManagerTonatiuh::GetGameStateById(GAME_STATES gameStateId)
{
	GGameState* gs=NULL; 
	
	for(int i=0;i<mGameStates.size();i++)
	{	
		if(    (mGameStates[i] != NULL) 
			&& (mGameStates[i]->VGameStateId() == gameStateId))
		{
			gs = mGameStates[i];
		}
	}
	
	return gs;

}



GGameState* GGameManagerTonatiuh::GetCurrentGameState()
{
	GGameState* gs=NULL; 
	
	for(int i=0;i<mGameStates.size();i++)
	{	
		if(    (mGameStates[i] != NULL) 
			&& (mGameStates[i]->VGameStateId() == mCurrentGameStateIndex))
		{
			gs = mGameStates[i];
		}
	}
	
	return gs;

}

GGameState* GGameManagerTonatiuh::GetLastGameState()
{
	GGameState* gs=NULL; 
	
	for(int i=0;i<mGameStates.size();i++)
	{	
		if(    (mGameStates[i] != NULL) 
			&& (mGameStates[i]->VGameStateId() == mLastGameStateIndex))
		{
			gs = mGameStates[i];
		}
	}
	
	return gs;

}

void GGameManagerTonatiuh::VReleaseRenderContext()
{
	irr::IrrlichtDevice* device = mApp->getIrrlichtDevice();
	irr::video::IVideoDriver *driver = device->getVideoDriver();	

	driver->removeAllTextures();
	
	//close the current gamestate
	GGameState* currentGS=  GetCurrentGameState();

	if(currentGS)
		currentGS->VOnLostFocus();


	mApp->mRocketGUI->Reset();


	mLastGameStateIndex= GAME_STATE_NONE;
	VSetNextGameState(GAME_STATE_MAINMENU,0);
	
}
void GGameManagerTonatiuh::VReInitRenderContext()
{
	//irr::IrrlichtDevice* device = mApp->getIrrlichtDevice();
//	irr::video::IVideoDriver *driver = device->getVideoDriver();	
	
	//reinit shaders
	irr::video::IGPUProgrammingServices* gpu = mApp->getIrrlichtDevice()->getVideoDriver()->getGPUProgrammingServices();		
	if (gpu)
	{	
		FireWorkShader::Initialize();
		SpiralShader::Initialize();
		MetaballShader::Initialize();
		MetaballShaderPre::Initialize();
	}
	 

	//reinit the game states render context

	for(int i=0;i<mGameStates.size();i++)
	{	
		if(    mGameStates[i] != NULL) 			
		{
			mGameStates[i]->VReInitRenderContext();

		}
	}
	

}


