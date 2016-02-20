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
#include "TonatiuhDefines.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <CIrrDeviceStub.h>
#include "os.h"
#include "GGameApp.h"
#include "GObject.h"
#include "GObjectFactory.h"
#include "GController.h"
#include "preferences/PreferencesManager.h"
#include "TGoalManager.h"
#include "TGameStateOptions.h"


int TGameStateOptions::VGameStateId()
{
	return (int)GAME_STATE_OPTIONS;

}


TGameStateOptions::TGameStateOptions() :GGameState()
{
	
}


void TGameStateOptions::VFinish() 
{ 

}

void TGameStateOptions::VInit() 
{ 
	GGameApp* app = GGameApp::getInstance();




	//register as menu event listener
	if(app->getEventManager())
		app->getEventManager()->AttachRocketListener(*this);

	if(app->getEventManager())
		app->getEventManager()->AttachListener(*this);

}



		

void TGameStateOptions::VUpdate(int deltaMs)
{
	

}

void TGameStateOptions::VOnGetFocus(int param) 
{ 
	GGameState::VOnGetFocus(param);
	GGameApp* app = GGameApp::getInstance();
	
		
#if defined(_IRR_ANDROID_PLATEFORM_)
	//load the menu GUI
	if (app->getRocketGUI()->LoadDocument("options.rml"))
	{	
		app->getRocketGUI()->SetDocumentVisible("options_menu",true);		
	}
#else
	//load the menu GUI
	if (app->getRocketGUI()->LoadDocument("IrrlichtSdcard/options.rml"))
	{	
		app->getRocketGUI()->SetDocumentVisible("options_menu",true);		
	}
#endif

	bool sfx= (app->getPreferencesManager()->getPreference(GAME_PREFERENCES_SFX,1))==1;
	bool msc= (app->getPreferencesManager()->getPreference(GAME_PREFERENCES_MUSIC,1))==1;
	bool vib= (app->getPreferencesManager()->getPreference(GAME_PREFERENCES_VIBRATE,1))==1;
	
	if(sfx)	
		app->getRocketGUI()->SetElementInnerText("options_menu","optSFX","On");
	else
		app->getRocketGUI()->SetElementInnerText("options_menu","optSFX","Off");
	
	if(msc)
		app->getRocketGUI()->SetElementInnerText("options_menu","optMusic","On");
	else
		app->getRocketGUI()->SetElementInnerText("options_menu","optMusic","Off");

	if(vib)
		app->getRocketGUI()->SetElementInnerText("options_menu","optVibrate","On");
	else
		app->getRocketGUI()->SetElementInnerText("options_menu","optVibrate","Off");

};
void TGameStateOptions::VOnLostFocus() 
{
	GGameState::VOnLostFocus();
	GGameApp* app = GGameApp::getInstance();
	app->getRocketGUI()->CloseDocument("options_menu");		
	

};





//process a librocket menu event
void TGameStateOptions::ProcessRocketEvent(const Rocket::Core::String& value)
{
	GGameApp* app = GGameApp::getInstance();

	

	if(value== "onToggleSfx")
	{
		bool sfx= (app->getPreferencesManager()->getPreference(GAME_PREFERENCES_SFX,1))==1;
		sfx = !sfx;
		app->getPreferencesManager()->setPreference(GAME_PREFERENCES_SFX, sfx?1:0);


		
		if(sfx)	
		{
			app->getRocketGUI()->SetElementInnerText("options_menu","optSFX","On");

			app->getAudioManager()->enableSound(true);
			
		}
		else
		{
			app->getRocketGUI()->SetElementInnerText("options_menu","optSFX","Off");
			app->getAudioManager()->enableSound(false);

		}
	
		

	}
	else if(value== "onToggleMsc")
	{
		bool msc= (app->getPreferencesManager()->getPreference(GAME_PREFERENCES_MUSIC,1))==1;
		msc = !msc;
		app->getPreferencesManager()->setPreference(GAME_PREFERENCES_MUSIC, msc?1:0);

		if(msc)
		{
			app->getAudioManager()->enableMusic(true);				
			app->getRocketGUI()->SetElementInnerText("options_menu","optMusic","On");
		}
		else
		{
			app->getAudioManager()->enableMusic(false);
			app->getRocketGUI()->SetElementInnerText("options_menu","optMusic","Off");
		}


	}
	else if(value== "onToggleVib")
	{
		bool vib= (app->getPreferencesManager()->getPreference(GAME_PREFERENCES_VIBRATE,1))==1;
		vib = !vib;
		app->getPreferencesManager()->setPreference(GAME_PREFERENCES_VIBRATE, vib?1:0);

		if(vib)
			app->getRocketGUI()->SetElementInnerText("options_menu","optVibrate","On");
		else
			app->getRocketGUI()->SetElementInnerText("options_menu","optVibrate","Off");

	}
	else if(value== "OnToggleMenu")
	{
		
		app->mGameManager->VSetNextGameState(GAME_STATE_MAINMENU,0);
	}
	
}

void TGameStateOptions::OnKeyDown(irr::EKEY_CODE keycode)
{
}
void TGameStateOptions::OnKeyUp(irr::EKEY_CODE keycode)
{	
	if(!mHasFocus)
		return;

	GGameApp* app = GGameApp::getInstance();

	if(keycode == irr::KEY_KEY_B)
	{
		 app->mGameManager->VSetNextGameState(GAME_STATE_MAINMENU,0);
	}

}
void TGameStateOptions::OnMouseDown(int x, int y)
{
	
	

}
void TGameStateOptions::OnMouseUp(int x, int y)
{
	

}


void TGameStateOptions::OnMouseMove(int x, int y)
{
	
}


TGameStateOptions::~TGameStateOptions(void)
{
	
}


void TGameStateOptions::VRender(irr::IrrlichtDevice* device)
{
}

