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

#include "TGameStateExtras.h"


int TGameStateExtras::VGameStateId()
{
	return (int)GAME_STATE_EXTRAS;

}


TGameStateExtras::TGameStateExtras() :GGameState()
{
	
}


void TGameStateExtras::VFinish() 
{ 

}

void TGameStateExtras::VInit() 
{ 
	GGameApp* app = GGameApp::getInstance();




	//register as menu event listener
	if(app->getEventManager())
		app->getEventManager()->AttachRocketListener(*this);

	if(app->getEventManager())
		app->getEventManager()->AttachListener(*this);

}



		

void TGameStateExtras::VUpdate(int deltaMs)
{
	

}

void TGameStateExtras::VOnGetFocus(int param) 
{ 
	GGameState::VOnGetFocus(param);


	GGameApp* app = GGameApp::getInstance();
	
		
#if defined(_IRR_ANDROID_PLATEFORM_)
	//load the menu GUI
	if (app->getRocketGUI()->LoadDocument("extras.rml"))
	{	
		app->getRocketGUI()->SetDocumentVisible("extras_menu",true);		
	}
#else
	//load the menu GUI
	if (app->getRocketGUI()->LoadDocument("IrrlichtSdcard/extras.rml"))
	{	
		app->getRocketGUI()->SetDocumentVisible("extras_menu",true);		
	}
#endif


	

};
void TGameStateExtras::VOnLostFocus() 
{
	GGameState::VOnLostFocus();
	
	GGameApp* app = GGameApp::getInstance();
	app->getRocketGUI()->CloseDocument("extras_menu");		
	

};





//process a librocket menu event
void TGameStateExtras::ProcessRocketEvent(const Rocket::Core::String& value)
{
	GGameApp* app = GGameApp::getInstance();

	

	if(value== "xyz")
	{
		
		app->mGameManager->VSetNextGameState(GAME_STATE_GAME,0);
		
	}
	else if(value== "OnToggleMenu")
	{
		
		app->mGameManager->VSetNextGameState(GAME_STATE_MAINMENU,0);
	}
	
}

void TGameStateExtras::OnKeyDown(irr::EKEY_CODE keycode)
{
}
void TGameStateExtras::OnKeyUp(irr::EKEY_CODE keycode)
{	
	if(!mHasFocus)
		return;

	GGameApp* app = GGameApp::getInstance();

	if(keycode == irr::KEY_KEY_B)
	{
		 app->mGameManager->VSetNextGameState(GAME_STATE_MAINMENU,0);
	}


}
void TGameStateExtras::OnMouseDown(int x, int y)
{
	
	

}
void TGameStateExtras::OnMouseUp(int x, int y)
{
	

}


void TGameStateExtras::OnMouseMove(int x, int y)
{
	
}


TGameStateExtras::~TGameStateExtras(void)
{
	
}


void TGameStateExtras::VRender(irr::IrrlichtDevice* device)
{
}

