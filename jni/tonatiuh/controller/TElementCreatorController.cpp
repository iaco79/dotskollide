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
#include "GSpriteComponent.h"
#include "GObject.h"
#include "GController.h"
#include "TonatiuhDefines.h"
#include "irrnode/Base2dNode.h"
#include "irrnode/SpriteNode.h"
#include "SceneNode/SpiralNode.h"
#include "springrid/SpringGrid.h"
#include "component/TSpringGridComponent.h"
#include "Controller/TElementCreatorController.h"


const char* TElementCreatorController::mName = "ElementCreator";


TElementCreatorController::TElementCreatorController()
{
	gcontrollerId = GComponent::GetIdFromName(TElementCreatorController::mName);

	mOrigin.Set(0.0f,0.0f);
	mTimer = 0;
	mState = CREATOR_STATE_NONE;
	mActive = false;
	mCreationTime = 1000;
	mFinishTime = 500;
	mSpringIndex =-1;
	mSpringObject = NULL;
	mAngle = 0.0f;
	mSpeed = 0.0f;
}


void TElementCreatorController::VUpdate(int deltaMs) 
{
	//get the transform component

	if(mActive)
	{
		 mTimer+= deltaMs;

		

		 GTransformComponent* tcomp = mGameObject->GetComponent<GTransformComponent>(mGameObject->GetComponentIdByIndex(GC_TRANSFORM_IDX));	  
    	 GIrrlichtComponent* icomp = mGameObject->GetComponent<GIrrlichtComponent>(mGameObject->GetComponentIdByIndex(GC_IRRNODE_IDX));
		 SpriteNode* spNode = (SpriteNode*)icomp->mNode;

		 tcomp->mTrans.Set(mOrigin,mAngle); // (float)(mTimer/1000.f)* 6.28f);
		 
		 tcomp->mZ = 0.05f;

		if(mState  == CREATOR_STATE_CREATING)
		{
		 
		  float scalef =  2.0f * ((float) mTimer / (float) mCreationTime)  * 1.0f;	  
		  spNode->setScale(irr::core::vector3df(scalef, scalef, scalef));
		  spNode->setVisible(false); //not needed

		  //spNode->setElapsedTime(spNode->getElapsedTime() + deltaMs);
		   

		   float  colorStep =  2.0f * ((float) mTimer / (float) mCreationTime);
		  float rColor =  sin(colorStep*6.28f);
		  float bColor =  cos(colorStep*6.28f);

			irr::video::SColor clr(
					255,
					(int)(192.0f + rColor * 64.0f),
					0,
					(int)(192.0f + rColor * 64.0f));	


			spNode->SetVertexColor(clr);

		  //scale to size 
		    
		} 
		else if(mState  == CREATOR_STATE_CREATED)
		{
		  //spNode->setElapsedTime(spNode->getElapsedTime() + deltaMs);
		  
		  GIrrlichtComponent* icomp = mGameObject->GetComponent<GIrrlichtComponent>(mGameObject->GetComponentIdByIndex(GC_IRRNODE_IDX));
		   
		  float scalef = 2.0f - ((float) mTimer / (float) mFinishTime) * 2.0f ;
		  spNode->setScale(irr::core::vector3df(scalef, scalef, scalef));
		  spNode->setVisible(false ); //


		  float  colorStep =  2.0f * ((float) mTimer / (float) mFinishTime);
		  float rColor =  sin(colorStep*6.28f);
		  float bColor =  cos(colorStep*6.28f);

			irr::video::SColor clr(
					255,
					(int)(192.0f - rColor * 64.0f),
					0,
					(int)(192.0f - rColor * 64.0f));	

			spNode->SetVertexColor(clr);

			

		  if(mTimer >= mFinishTime)
		  {
			mState = CREATOR_STATE_FINISHED;
		  }

		}
		else if(mState  == CREATOR_STATE_FINISHED)
		{
			mActive = false;

			if(mSpringObject && mSpringIndex>=0)
			{
				TSpringGridComponent* spring = mSpringObject->GetComponent<TSpringGridComponent>(mSpringObject->GetComponentIdByIndex(T_GC_SPRINGRID_IDX));
				spring->mSpringGrid->UnsetPinchForce(mSpringIndex);

			}
		


			if(icomp->mNode)
				spNode->setVisible(false);
		}
	}
			
}
bool TElementCreatorController::VInit(const Json::Value& jcomponent) 
{
	
	return true;
}

void TElementCreatorController::Start(b2Vec2 origin,float speed, float angle, GObjectPtr springObject)
{
	mOrigin = origin;
	/*GIrrlichtComponent* icomp = mGameObject->GetComponent<GIrrlichtComponent>(mGameObject->GetComponentIdByIndex(GC_IRRNODE_IDX));
	SpiralNode* node = (SpiralNode*) (icomp->mNode);
	node->setElapsedTime(0);
	*/
	mActive = true;
	mAngle = angle-3.1416f;
	mSpringObject = springObject;
	mSpeed =  speed;

	 GTransformComponent* tcomp = mGameObject->GetComponent<GTransformComponent>(mGameObject->GetComponentIdByIndex(GC_TRANSFORM_IDX));	  
     tcomp->mTrans.Set(mOrigin,mAngle); // (float)(mTimer/1000.f)* 6.28f);
		

	GIrrlichtComponent* icomp = mGameObject->GetComponent<GIrrlichtComponent>(mGameObject->GetComponentIdByIndex(GC_IRRNODE_IDX));
    SpriteNode* spNode = (SpriteNode*)icomp->mNode;

	if(mSpeed> 0.0f)
	{
		spNode->setFrame(3);
	
	}
	else
	{
		spNode->setFrame(11);
	}


	/*
	TSpringGridComponent* spring = springObject->GetComponent<TSpringGridComponent>(springObject->GetComponentIdByIndex(T_GC_SPRINGRID_IDX));
	springrid::SpringGrid::PinchForce sf;
	sf.mForcePosition =  springrid::Vector3D(origin.x ,origin.y, 0.0f);
	sf.mForceFactor = 0.7f;
	mSpringIndex = spring->mSpringGrid->SetPinchForce(sf);
	*/
	//spring->set



	 
}

bool TElementCreatorController::VInit() 
{
	GIrrlichtComponent* icomp = mGameObject->GetComponent<GIrrlichtComponent>(mGameObject->GetComponentIdByIndex(GC_IRRNODE_IDX));
	icomp->mNode->setVisible(false);
	
	mSpringIndex = -1;
	mOrigin.Set(0.0f,0.0f);
	mTimer = 0;
	mState = CREATOR_STATE_NONE;
	mActive = false;
	mCreationTime = 1000;
	mFinishTime = 500;
	mSpringObject = NULL;

	return true;
} 
		

TElementCreatorController& TElementCreatorController::operator= (const TElementCreatorController& other )
{
	GController::operator= (other); //call base assignment operator
	mTimer = 0;
	mActive = false;
	return *this;
}

