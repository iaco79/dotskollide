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
#include <irrlicht.h>
#include "GameDefines.h"
#include "GObject.h"
#include "GController.h"
#include "GControllerComponent.h"
#include "GControllerFactory.h"

const char* GControllerComponent::mName = "Controller";

GControllerComponent::GControllerComponent()
{
	gcomponentId = GComponent::GetIdFromName(GControllerComponent::mName);
	gameController = NULL;

}

bool GControllerComponent::VInit(Json::Value& jcomponent)
{
	GControllerFactory* controllerFactory = GGameApp::getInstance()->getControllerFactory();

	//if(mGameObject->IsPrototype())
	//	mjcomponent = jcomponent; //if this is a prototype, then backup the json definition

	gameController = controllerFactory->CreateController(jcomponent);
	return true;
}

bool GControllerComponent::VInit(GController* controller)
{
	
	gameController = controller;
	return true;
}


GControllerComponent& GControllerComponent::operator= (const GControllerComponent& other )
{
	GComponent::operator= (other); //call base assignment operator

	GControllerFactory* controllerFactory = GGameApp::getInstance()->getControllerFactory();
	gameController = controllerFactory->CloneController(other.gameController);
	
	
	return *this;
}

void GControllerComponent::SetOwner(GObjectPtr owner)
{ 
	mGameObject = owner; 

	if(gameController)
		gameController->mGameObject = owner;
};

GControllerComponent::~GControllerComponent()
{
	if(gameController)
		delete  gameController;
}

void GControllerComponent::VUpdate(int deltaMs) 
{
	if(gameController)
		gameController->VUpdate(deltaMs);

}