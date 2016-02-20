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
#include "GBox2dComponent.h"
#include "GTransformComponent.h"
#include "GIrrlichtComponent.h"
#include "GControllerComponent.h"
#include "GSpriteComponent.h"
#include "irrnode/Base2dNode.h"
#include "irrnode/B2dNode.h"
#include "SceneNode/EnergyBarNode.h"
#include "TonatiuhDefines.h"
#include "TGoalManager.h"
#include "Controller/TPlayerController.h"
#include "Controller/TShieldController.h"


#include "Controller/TBitController.h"

const char* TBitController::mName = "BitElement";






TBitController::TBitController()
{
	gcontrollerId = GComponent::GetIdFromName(TBitController::mName);

	mState= BIT_STATE_ACTIVE;
	mTarget = NULL;
	mPlayer = NULL;
	mValue - 0.0f;

	mLinearVelocity.Set(0.0f,0.0f);
}	


void TBitController::VUpdate(int deltaMs) 
{ 

	mTimer = mTimer + deltaMs;

	if(mState== BIT_STATE_ACTIVE && mTarget!=NULL)
	{
		//get my transform
		GTransformComponent*  tcomp = mGameObject->GetComponent<GTransformComponent>(mGameObject->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		GTransformComponent*  tTarget = mTarget->GetComponent<GTransformComponent>(mTarget->GetComponentIdByIndex(GC_TRANSFORM_IDX));
	
		b2Vec2 targetP;

		targetP = tTarget->mTrans.p;
		if(mTarget->GetType() == T_OT_BAR)
		{
			GIrrlichtComponent* irrcomp = mTarget->GetComponent<GIrrlichtComponent>(mTarget->GetComponentIdByIndex(GC_IRRNODE_IDX));
			EnergyBarNode* barNode =  ((EnergyBarNode*) irrcomp->mNode);
						
			
			//targetP = tTarget->mTrans.p;
			targetP.y = targetP.y - 0.5f + 1.0f * barNode->getEnergyValue() ;


		}

		b2Vec2 dif = targetP - tcomp->mTrans.p ;
		float distance = dif.Length();
		dif.Normalize();

		b2Vec2 velocity = mLinearVelocity;
		float factor = ((30.0f-distance)/30.0);
		b2Vec2 velocity1 = factor*dif;
		velocity1 = 10.0f*velocity1;
		b2Vec2 veldif = velocity1- velocity;
		velocity=  velocity + veldif*factor*.2;
		mLinearVelocity = velocity;

		float dTime = ((float)deltaMs) / 1000.0f;
		tcomp->mTrans.Set(tcomp->mTrans.p +  dTime*mLinearVelocity,0.0f);

		if(distance<0.5f)
		{
		
			mState = BIT_STATE_FINISHED;


			if(mTarget->GetType() == T_OT_BAR)
			{
				//get the player controller
				GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TPlayerController* pcont = (TPlayerController*)ccomp->gameController;
				
				switch(mTarget->GetSubType())
				{
					case 0:
						if(pcont->mCountA< pcont->mColorCount)
							pcont->mCountA+=  mValue;
					break;
					case 1:
						if(pcont->mCountB< pcont->mColorCount)
							pcont->mCountB+=  mValue;
					break;
					case 2:
						if(pcont->mCountC< pcont->mColorCount)
							pcont->mCountC+=  mValue;
					break;
					case 3:
						if(pcont->mCountD< pcont->mColorCount)
							pcont->mCountD+=  mValue;
					break;
					case 4:
						if(pcont->mCountE< pcont->mColorCount)
							pcont->mCountE+=  mValue;
					break;
				}

				
				
			}
			else if(mTarget->GetType() == T_OT_SHIELD)
			{
			
				//get the shield controller
				GControllerComponent* ccomp = mTarget->GetComponent<GControllerComponent>(mTarget->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TShieldController* pcont = (TShieldController*)ccomp->gameController;
						
				pcont->mPower+=3;
				pcont->mFixers--;

				if(pcont->mPower == 3)
				{
					pcont->mState = SHIELD_STATE_FIXED;
				}
			}


		}

	}

}


void TBitController::VInit(GObjectPtr player, GObjectPtr target, float value)
{
	mLinearVelocity.Set(0.0f,0.0f);
	mTarget = target;
	mPlayer = player;
	mValue = value;

}

bool TBitController::VInit(const Json::Value& jcomponent) 
{
	
	return true;
}

TBitController& TBitController::operator= (const TBitController& other )
{
	GController::operator= (other); //call base assignment operator

	return *this;
}
