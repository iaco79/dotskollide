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
#include "TGoalManager.h"

#include "TGameStateMainMenu.h"


int TGameStateMainMenu::VGameStateId()
{
	return (int)GAME_STATE_MAINMENU;

}


TGameStateMainMenu::TGameStateMainMenu() :GGameState()
{
	
}


void TGameStateMainMenu::VFinish() 
{ 

}

void TGameStateMainMenu::VInit() 
{ 
	GGameApp* app = GGameApp::getInstance();




	//register as menu event listener
	if(app->getEventManager())
		app->getEventManager()->AttachRocketListener(*this);


}



		

void TGameStateMainMenu::VUpdate(int deltaMs)
{
	

}

void TGameStateMainMenu::VOnGetFocus(int param) 
{ 
	GGameState::VOnGetFocus(param);
	GGameApp* app = GGameApp::getInstance();

	
	#if defined(_IRR_ANDROID_PLATEFORM_)
		//load the menu GUI
		if (app->getRocketGUI()->LoadDocument("mainmenu.rml"))
		{	
			app->getRocketGUI()->SetDocumentVisible("main_menu",true);		
		}
	#else
		//load the menu GUI
		if (app->getRocketGUI()->LoadDocument("IrrlichtSdcard/mainmenu.rml"))
		{	
			app->getRocketGUI()->SetDocumentVisible("main_menu",true);		
		}
	#endif


	
	app->getRocketGUI()->SetElementVisibility("main_menu","button_resume", (param==1));		
		


};
void TGameStateMainMenu::VOnLostFocus() 
{
	GGameState::VOnLostFocus();
	GGameApp* app = GGameApp::getInstance();
	//app->getRocketGUI()->SetDocumentVisible("main_menu",false);		
	
	app->getRocketGUI()->CloseDocument("main_menu");


};





//process a librocket menu event
void TGameStateMainMenu::ProcessRocketEvent(const Rocket::Core::String& value)
{
	GGameApp* app = GGameApp::getInstance();

	

	if(value== "OnNewGame")
	{
		
		app->mGameManager->VSetNextGameState(GAME_STATE_LEVELSELECT,0);
		
	}
	else if(value== "OnResume")
	{
		
		app->mGameManager->VSetNextGameState(GAME_STATE_GAME,GAME_ONFOCUS_PARAMS_RESUME);
	}
	else if(value== "OnOptions")
	{
		
		app->mGameManager->VSetNextGameState(GAME_STATE_OPTIONS,0);
	}
	else if(value== "OnExtras")
	{
		
		app->mGameManager->VSetNextGameState(GAME_STATE_EXTRAS,0);
	}
}

void TGameStateMainMenu::OnKeyDown(irr::EKEY_CODE keycode)
{
}
void TGameStateMainMenu::OnKeyUp(irr::EKEY_CODE keycode)
{	
	if(!mHasFocus)
		return;
	/*
	switch(keycode)
	{
		case KEY_KEY_1:
			 	break;
		
	}
	*/
}
void TGameStateMainMenu::OnMouseDown(int x, int y)
{
	
	

}
void TGameStateMainMenu::OnMouseUp(int x, int y)
{
	

}


void TGameStateMainMenu::OnMouseMove(int x, int y)
{
	
}


TGameStateMainMenu::~TGameStateMainMenu(void)
{
	
}


void TGameStateMainMenu::VRender(irr::IrrlichtDevice* device)
{
}

