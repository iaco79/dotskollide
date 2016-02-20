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
#include "GTransformComponent.h"
#include "GIrrlichtComponent.h"
#include "GObject.h"
#include "GController.h"
#include "irrnode/Base2dNode.h"
#include "Controller/TFireWorkController.h"
#include "SceneNode/FireworkNode.h"




const char* TFireWorkController::mName = "Firework";


TFireWorkController::TFireWorkController()
{
	gcontrollerId = GComponent::GetIdFromName(TFireWorkController::mName);

	mElapsedTime=0;
	mActive=0;
	mFactor = 1.0f;
	mLifeSpan = 1500;

}


void TFireWorkController::VUpdate(int deltaMs) 
{
	if(mActive)
	{
		//update elapsed time
		mElapsedTime = mElapsedTime + deltaMs;

		//get the scene Node
		GComponentId compId = mGameObject->GetComponentIdByIndex(GC_IRRNODE_IDX);
		GIrrlichtComponent* ncomp = mGameObject->GetComponent<GIrrlichtComponent>(compId);
		FireworkNode* node = NULL;

		if(ncomp)
		{
			//set visible 
			node = (FireworkNode* )ncomp->mNode;
			node->setVisible(true);

			//update the node elapsed time
			node->SetElapsedTime((float)mElapsedTime*0.0007f); // * mFactor);

		}

		if(mElapsedTime > mLifeSpan)
		{
			if(node)
				node->setVisible(false);

			mActive = false;

		}
	}
}
bool TFireWorkController::VInit(const Json::Value& jcomponent) 
{
	return true;
}
void TFireWorkController::VInit(irr::u32 lifeSpan, irr::f32 factor) 
{
	SetLifeSpan(lifeSpan);
	SetFactor(factor);

	//disable the scene node
	
	if(mGameObject)
	{
		GComponentId compId = mGameObject->GetComponentIdByIndex(GC_IRRNODE_IDX);
		GIrrlichtComponent* ncomp = mGameObject->GetComponent<GIrrlichtComponent>(compId);

		if(ncomp)
			ncomp->mNode->setVisible(false);

			
	}

}
		

TFireWorkController& TFireWorkController::operator= (const TFireWorkController& other )
{
	GController::operator= (other); //call base assignment operator
	mActive = false;
	mElapsedTime = 0;

	return *this;

}


void TFireWorkController::ExplodeAt(b2Vec2 position, irr::video::SColor clr)
{
	mActive = true;
	mElapsedTime = 0;

	//get the transform component
	if(mGameObject)
	{
		GComponentId compId = mGameObject->GetComponentIdByIndex(GC_TRANSFORM_IDX);
		GTransformComponent* bcomp = mGameObject->GetComponent<GTransformComponent>(compId);

		if(bcomp)
		{	
			float angle = (12.0*(float)(std::rand()%30))*3.1416/180.0f;
			bcomp->mTrans.Set(position, angle);
		}

		compId = mGameObject->GetComponentIdByIndex(GC_IRRNODE_IDX);
		GIrrlichtComponent* ncomp = mGameObject->GetComponent<GIrrlichtComponent>(compId);
		FireworkNode* node = NULL;

		if(ncomp)
		{
			//set visible 
			node = (FireworkNode* )ncomp->mNode;
			node->SetFireColor(clr);			
		}

			

	}


}


