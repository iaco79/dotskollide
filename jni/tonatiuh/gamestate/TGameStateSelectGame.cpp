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

#include "TGameStateSelectGame.h"


int TGameStateSelectGame::VGameStateId()
{
	return (int)GAME_STATE_LEVELSELECT;

}


TGameStateSelectGame::TGameStateSelectGame() :GGameState()
{
	
}


void TGameStateSelectGame::VFinish() 
{ 

}

void TGameStateSelectGame::VInit() 
{ 
	GGameApp* app = GGameApp::getInstance();




	//register as menu event listener
	if(app->getEventManager())
		app->getEventManager()->AttachRocketListener(*this);
	if(app->getEventManager())
		app->getEventManager()->AttachListener(*this);


}



		

void TGameStateSelectGame::VUpdate(int deltaMs)
{
	

}

void TGameStateSelectGame::VOnGetFocus(int param) 
{ 
	GGameState::VOnGetFocus(param);

	GGameApp* app = GGameApp::getInstance();
		
#if defined(_IRR_ANDROID_PLATEFORM_)
	//load the menu GUI
	if (app->getRocketGUI()->LoadDocument("levelselect.rml"))
	{	
		app->getRocketGUI()->SetDocumentVisible("level_select",true);		
	}
#else
	//load the menu GUI
	if (app->getRocketGUI()->LoadDocument("IrrlichtSdcard/levelselect.rml"))
	{	
		app->getRocketGUI()->SetDocumentVisible("level_select",true);		
	}
#endif


};
void TGameStateSelectGame::VOnLostFocus() 
{
	GGameState::VOnLostFocus();
	GGameApp* app = GGameApp::getInstance();
	app->getRocketGUI()->CloseDocument("level_select");		
	

};





//process a librocket menu event
void TGameStateSelectGame::ProcessRocketEvent(const Rocket::Core::String& value)
{
	GGameApp* app = GGameApp::getInstance();

	

	if(value== "OnTutorial")
	{
		
		app->mGameManager->VSetNextGameState(GAME_STATE_GAME, GAME_ONFOCUS_PARAMS_TUTORIAL);
		
	}
	else if(value== "OnNewGame1")
	{
		app->mGameManager->VSetNextGameState(GAME_STATE_GAME,GAME_ONFOCUS_PARAMS_LEVEL1);
		
	}
	else if(value== "OnNewGame2")
	{
		app->mGameManager->VSetNextGameState(GAME_STATE_GAME,GAME_ONFOCUS_PARAMS_LEVEL2);
		
	}
	else if(value== "OnNewGame3")
	{
		app->mGameManager->VSetNextGameState(GAME_STATE_GAME,GAME_ONFOCUS_PARAMS_LEVEL3);
		
	}
	else if(value== "OnToggleMenu")
	{
		app->mGameManager->VSetNextGameState(GAME_STATE_MAINMENU,0);
		
	}
	
}

void TGameStateSelectGame::OnKeyDown(irr::EKEY_CODE keycode)
{
}
void TGameStateSelectGame::OnKeyUp(irr::EKEY_CODE keycode)
{	
	if(!mHasFocus)
		return;

	GGameApp* app = GGameApp::getInstance();

	if(keycode == irr::KEY_KEY_B)
	{
		 app->mGameManager->VSetNextGameState(GAME_STATE_MAINMENU,0);
	}

}
void TGameStateSelectGame::OnMouseDown(int x, int y)
{
	
	

}
void TGameStateSelectGame::OnMouseUp(int x, int y)
{
	

}


void TGameStateSelectGame::OnMouseMove(int x, int y)
{
	
}


TGameStateSelectGame::~TGameStateSelectGame(void)
{
	
}


void TGameStateSelectGame::VRender(irr::IrrlichtDevice* device)
{
}

