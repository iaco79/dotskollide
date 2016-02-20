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
#include "GameDefines.h"
#include "GComponent.h"
#include "GComponent.h"
#include "GObject.h"
#include "GController.h"
#include "GTransformComponent.h"
#include "GControllerComponent.h"
#include "GIrrlichtComponent.h"
#include "GSpriteComponent.h"
#include "irrnode/Base2dNode.h"
#include "irrnode/B2dNode.h"
#include "TonatiuhDefines.h"
#include "SceneNode/EnergyBarNode.h"
#include "GController.h"

#include "Controller/TEnergyBarController.h"
#include "TGoalManager.h"
#include "Controller/TPlayerController.h"


const char* TEnergyBarController::mName = "EnergyBarController";

TEnergyBarController::TEnergyBarController()
{
	mPlayer= NULL;
	mType = 0;

}	


void TEnergyBarController::VUpdate(int deltaMs) 
{ 
	if(mPlayer != NULL)
	{
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));

		GIrrlichtComponent* irrcomp = mGameObject->GetComponent<GIrrlichtComponent>(mGameObject->GetComponentIdByIndex(GC_IRRNODE_IDX));

		if(irrcomp)
		{
			//get the scene node		
			EnergyBarNode* barNode =  ((EnergyBarNode*) irrcomp->mNode);
			//get the player controller
			TPlayerController* pcont = (TPlayerController*)ccomp->gameController;

			if(mType==0)
				barNode->setEnergyValue(pcont->mEnergy);
			else
				barNode->setEnergyValue(pcont->mRadiation);
		}

		

	}
}


void TEnergyBarController::VInit(GObjectPtr player, int type)
{
	mPlayer = player;
	mType = type;
	
}


bool TEnergyBarController::VInit(const Json::Value& jcomponent) 
{
	
	return true;
}
	

TEnergyBarController& TEnergyBarController::operator= (const TEnergyBarController& other )
{
	GController::operator= (other); //call base assignment operator
	
	return *this;
}


