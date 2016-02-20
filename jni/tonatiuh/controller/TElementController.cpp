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
#include "GObject.h"
#include "GBox2dComponent.h"
#include "GTransformComponent.h"
#include "GController.h"
#include "GControllerComponent.h"
#include "GSpriteComponent.h"

#include "TonatiuhDefines.h"
#include "Controller/TElementController.h"
#include "Controller/TElementCreatorController.h"

const char* TElementController::mName = "Element";

TElementController::TElementController()
{
	gcontrollerId = GComponent::GetIdFromName(TElementController::mName);

	mCrashState = CRASH_STATE_NONE;
	mLifeCycle = ELEMENT_CYCLE_CREATING;
	mInCrashForce= false;
	mSynth = NULL;
	mCreationTime = 1000; //elements take around 1 sec to create
	mTimer = 0;
	mHasPlayerContact = false;
	mHasContact=false;
	mRadiationFactor = 0.0f;
	mColorId = 0;
	mInAttraction = false;
	mCrashOperation = 0;
	mContactTimer = 0;
	mHasContactTimer = false;
	mAllowAttraction = false;

}	


void TElementController::setSynth(GObjectPtr synth) 
{
	if(synth) 
	{
		//initialize as creating 
		GSpriteComponent* scomp = mGameObject->GetComponent<GSpriteComponent>(mGameObject->GetComponentIdByIndex(GC_SPRITE_IDX));
		scomp->mScale.set(0.0f,0.0f);
		scomp->mColor.set(255,0,0,0);
	
		mSynth = synth;
		
		GControllerComponent* econtc = mSynth->GetComponent<GControllerComponent>(mSynth->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TElementCreatorController* econt = (TElementCreatorController*)econtc->gameController;
		econt->setState(CREATOR_STATE_CREATING);
		econt->setTimer();
		econt->setActive(true);


		mLifeCycle = ELEMENT_CYCLE_CREATING;

	}
	else
	{	mSynth = NULL;
		mLifeCycle = ELEMENT_CYCLE_CREATED;
		GBox2dComponent*  bcomp = mGameObject->GetComponent<GBox2dComponent>(mGameObject->GetComponentIdByIndex(GC_BOX2D_IDX));
		

		bcomp->mBody->SetActive(true);
		

	}
}

void TElementController::FreeSynth()
{
	if(mSynth != NULL)
	{
		GControllerComponent* econtc = mSynth->GetComponent<GControllerComponent>(mSynth->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TElementCreatorController* econt = (TElementCreatorController*)econtc->gameController;
		econt->setState(CREATOR_STATE_CREATED);
		econt->setTimer();
	}
}
void TElementController::VUpdate(int deltaMs) 
{ 

	mTimer = mTimer + deltaMs;


	if( (mLifeCycle == ELEMENT_CYCLE_CREATING) && mSynth)
	{
		GControllerComponent* econtc = mSynth->GetComponent<GControllerComponent>(mSynth->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TElementCreatorController* econt = (TElementCreatorController*)econtc->gameController;
		
		//get the sprite component
		GSpriteComponent* scomp = mGameObject->GetComponent<GSpriteComponent>(mGameObject->GetComponentIdByIndex(GC_SPRITE_IDX));
		//get the box2d component
		GBox2dComponent*  bcomp = mGameObject->GetComponent<GBox2dComponent>(mGameObject->GetComponentIdByIndex(GC_BOX2D_IDX));
		
		//get the synth transform component
		GTransformComponent* ztrans = mSynth->GetComponent<GTransformComponent>(mSynth->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		

		float scalef = (float) mTimer / (float) mCreationTime;

		if(scalef > 1.0f)
			scalef  = 1.0f;
		
		scomp->mScale.set(scalef,scalef);
		irr::video::SColorf f (1.0f,scalef,scalef,scalef);		
		
		scomp->mColor = irr::video::SColor(225, 
										(int)((float)mColor.getRed()* scalef),
										(int)((float)mColor.getGreen()* scalef),
										(int)((float)mColor.getBlue()* scalef));

			


		//slowly move from center to 
		//b2Vec2 distance(-scalef,0.0);		
		//b2Vec2 position =  b2Mul(ztrans->mTrans.q, distance);
		//bcomp->mBody->SetTransform(ztrans->mTrans.p +  position , ztrans->mTrans.q.GetAngle());
		bcomp->mBody->SetTransform(ztrans->mTrans.p , ztrans->mTrans.q.GetAngle());

		if(mTimer>= mCreationTime)
		{	
			econt->setState(CREATOR_STATE_CREATED);
			econt->setTimer();

			mLifeCycle = ELEMENT_CYCLE_CREATED;
	
			//if created then activate the box2d component and launch
			

			float period = 2.0f + (float)(rand()%2);
			float rangle = period * 360.0f * (3.1416f /180.0f);

			bcomp->mBody->SetActive(true);
		    bcomp->mBody->SetLinearVelocity(b2Vec2(mInitialSpeed*cos(3.1416+ztrans->mTrans.q.GetAngle()),
										           mInitialSpeed*sin(3.1416+ztrans->mTrans.q.GetAngle())));

			if(!(mElementType == T_OS_ELEMENT_ENERGY ||mElementType == T_OS_ELEMENT_ENERGY2 || mElementType == T_OS_ELEMENT_BAD1))
				bcomp->mBody->SetAngularVelocity(rangle);		

			mSynth = NULL;
		}

	}
	else 
	{
		

		if (mLifeCycle== ELEMENT_CYCLE_CREATED)
		{
			//get the sprite component
			GSpriteComponent* sc = mGameObject->GetComponent<GSpriteComponent>(mGameObject->GetComponentIdByIndex(GC_SPRITE_IDX));
		
			
			if(mHasContactTimer)
			{
				mContactTimer += deltaMs;
			}


			if(mCrashState == CRASH_STATE_CANCRASH)
			{
				
				int step =  (mTimer%1000);
				

				if(mCrashOperation == 0)
				{
					step = step / 25; 
					step = step%3;

					if(step==0)
					{
					
						sc->mColor = irr::video::SColor(255,255,255,0);	
					}
					else if(step==1)
					{
						sc->mColor = irr::video::SColor(255,255,0,255);	
					
					}
					else if(step==2)
					{
						sc->mColor = irr::video::SColor(255,255,0,0);						
					}
					else 
					{
						sc->mColor = mColor;
					}
				}
				else if(mCrashOperation == 1)
				{
					/*step = step / 50; 
					
					if(step%2==0)
					{
						sc->mColor = irr::video::SColor(255,
								255,
								255,
								128
							);
					}
					else 
					{*/
						sc->mColor = mColor;
					//}
				
				}
				else
				{
					sc->mColor = mColor;
				}
			}
			else
			{
				sc->mColor = mColor;
			
			}
		}
	}
	
	

	//reset flags on every update
	mInCrashForce=false;
//	mHasContact = false;
	
}


void TElementController::VInit(int elementType)
{
	mElementType = elementType;
	
	
}


bool TElementController::VInit(const Json::Value& jcomponent) 
{
	
	return true;
}
	

TElementController& TElementController::operator= (const TElementController& other )
{
	GController::operator= (other); //call base assignment operator
	mElementType = other.mElementType;

	return *this;
}


void TElementController::setHasPlayerContact(bool hasPlayerContact)  
{ 
	mHasPlayerContact = hasPlayerContact;

	


}
