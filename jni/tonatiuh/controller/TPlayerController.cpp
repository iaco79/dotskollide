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
#include "os.h"
#include "irrnode/Base2dNode.h"
#include "irrnode/SpriteNode.h"
#include "preferences/PreferencesManager.h"
#include "GComponent.h"
#include "GController.h"
#include "GIrrlichtComponent.h"
#include "GBox2dComponent.h"
#include "GObject.h"
#include "TonatiuhDefines.h"
#include "TGoalManager.h"
#include "GControllerComponent.h"
#include "Controller/TElementController.h"
#include "Controller/TPlayerController.h"




const char* TPlayerController::mName = "Player";

void TPlayerController::PositionQueueAdd(MovementData data)
{

	if(positionQueue.size()<20)
	{
		positionQueue.push(data);
	}
};

void TPlayerController::PositionQueueClear()
{
	while(!positionQueue.empty())
	{
		positionQueue.pop();
	}

}

bool TPlayerController::PositionQueuePop(MovementData& data)
{
	bool result = false;

	if(!positionQueue.empty())
	{
		data = positionQueue.front();
		positionQueue.pop();
		result = true;
	}

	return result;
}


TPlayerController::TPlayerController()
{
	gcontrollerId = GComponent::GetIdFromName(TPlayerController::mName);

	mCrashState = CRASH_STATE_NONE;
	mApplyingForce =0.0f;
	mForceFactor=0.0f;
	mValidCollideRadius=0.0f;
	mAttractionForceRadius=0.0f;
	mCursorRadius=0.0f;
	bCollideOnStopForce = false;

	mCountA=0;
	mCountB=0;
	mCountC=0;
	mCountD=0;
	mCountE=0;
	mColorCount = 0;

	mElapsedTime=0;
	mEnergy=1.0f; 
	mF=0.005f;
	mCrashCount =0;
	mScore = 0;
	mElementsInArea = 0; 
	mBestColor=0;
	mScore = 0;
	mLevelCrashes=0;
	bInvinsible = false;
	mPlayerOffset.Set(0.0f,0.0f);
	mLevel =1;
	mLevelCrashesChanged = true;
	mScoreChanged = true;
	mLevelChanged=true;
	mGoalCountChanged=true;
	mMaxJoin=1;
	mMoleState = MOLE_STATE_NONE;
	mCanVibrate =false;

	clearMoles();

}


void TPlayerController::VUpdate(int deltaMs) 
{ 
	//get the irrlicht node

	GIrrlichtComponent* icomp = mGameObject->GetComponent<GIrrlichtComponent>(mGameObject->GetComponentIdByIndex(GC_IRRNODE_IDX));
	mElapsedTime = mElapsedTime+deltaMs;

	//if(mLevel>6)
	if(!mApplyingForce)
	{
		removeEnergy( ((float)(  1 + mElementsInArea /2)  ) * 0.01f );
	}

	removeRadiation(5.0f*0.7f* 0.01f);

	if(icomp)
	{
		irr::video::SColor defaultColor(255,255,255,255);

		if(mMaxJoin == 2)
		{
			defaultColor.set(255,255,255,0);
		}
		else if(mMaxJoin == 3)
		{
			defaultColor.set(255,255,128,0);
		}


		SpriteNode* sn = (SpriteNode*)icomp->mNode;

		if(mCrashState == CRASH_STATE_NONE)
		{
			if(mApplyingForce)
			{
				irr::video::SColor clr(255,255,0,64);	
				sn->SetVertexColor(clr);
			}
			else
			{	
				sn->SetVertexColor(defaultColor);
			}
		}
		else if(mCrashState == CRASH_STATE_CANCRASH)
		{
			if(mApplyingForce)
			{
				{
					removeEnergy(  ((float)(  1 + mTotalCount)  ) * 0.01f ) ;
				}

				int step =  (mElapsedTime%1000);

				step = step / 25; 
				step = step%3;

				if(step==0)
				{
					irr::video::SColor clr(255,255,255,0);	
					sn->SetVertexColor(clr);
				}
				else if(step==1)
				{
					irr::video::SColor clr(255,255,0,255);	
					sn->SetVertexColor(clr);
				}
				else if(step==2)
				{
					irr::video::SColor clr(255,255,0,0);	
					sn->SetVertexColor(clr);
				}
				else 
				{
					irr::video::SColor clr(255,255,255,255);	
					sn->SetVertexColor(clr);
				}

			}
			else
			{
				
				sn->SetVertexColor(defaultColor);
			}
		}
		else if(mCrashState == CRASH_STATE_INVALIDCRASH)
		{	
			if(mApplyingForce)
			{
								
				removeEnergy(  ((float)(  1 + mTotalCount)  ) * 0.05f);
				removeEnergy ((float)mBadCount * 0.3f);

				///removeEnergy((float)mBadCount * 0.2f); //   +   0.1f * (float)mTotalCount);

				irr::video::SColor clr(255,255,0,0);	
				sn->SetVertexColor(clr);
			}
			else
			{
				irr::video::SColor clr(defaultColor);	
				sn->SetVertexColor(clr);
			}
		}
		else if(mCrashState == CRASH_STATE_CRASHED)
		{	
			irr::video::SColor clr(255,220,0,255);	
			sn->SetVertexColor(clr);
		}
	}

}


void TPlayerController::VInit(float cursorRadius, float validCollideRadius, float attractionRadius)
{
	mValidCollideRadius=validCollideRadius;
	mAttractionForceRadius=attractionRadius;
	mCursorRadius=cursorRadius;
	mLevel = 1;
}


bool TPlayerController::VInit(const Json::Value& jcomponent) 
{
	
	return true;
}
	

TPlayerController& TPlayerController::operator= (const TPlayerController& other )
{
	GController::operator= (other); //call base assignment operator
	
	return *this;
}



void  TPlayerController::setApplyForce(bool forceEnable) 
{
	mApplyingForce = forceEnable;

	GBox2dComponent* bcomp = mGameObject->GetComponent<GBox2dComponent>(mGameObject->GetComponentIdByIndex(GC_BOX2D_IDX));

	if(bcomp)
	{		
		
		bcomp->mBody->SetActive(forceEnable);
		

	}

}


void TPlayerController::addEnergy(float factor)
{
	mEnergy = mEnergy+factor*mF;

	if(mEnergy>1.0)
		mEnergy= 1.0;
};

void TPlayerController::removeEnergy(float factor)
{
		
	if(!bInvinsible)
	{
		if(mEnergy<0.02)
			mEnergy =  mEnergy- mEnergy*factor*mF*4.0f;	
		else
			mEnergy = mEnergy-factor*mF;

		if(mEnergy<0.0)
			mEnergy= 0.0;

	}	
};
	

void TPlayerController::addRadiation(float factor)
{
	mRadiation = mRadiation+factor*mF;

	if(mRadiation>1.05f)
		mRadiation= 1.05f;
};

void TPlayerController::removeRadiation(float factor)
{
		
	mRadiation = mRadiation-factor*mF;

	if(mRadiation<0.0)
		mRadiation= 0.0;
};


void TPlayerController::AddMole(Mole* mole)
{
	if(mMoleCount<3)
	{
		std::string s = TGoalManager::intToString(mMoleCount);
		//irr::os::Printer::log("Adding mole #");
		irr::os::Printer::log(s.c_str());


		bool alreadyInMoles = false;

		//check if it is not yet in the current moles
		
		for(int k=0; k < mole->count;k++)
		{
			GObjectPtr newAtom = mole->atoms[k];
			GControllerComponent* ccomp2 =newAtom->GetComponent<GControllerComponent>(newAtom->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TElementController* econt2 = (TElementController*)ccomp2->gameController;		
			
			if(!econt2->getHasContact())
			{	
				econt2->setHasContact(true);

				if(k==0)
				{
					econt2->setAllowAttraction(true);
				
				}

			}
			else
			{
				alreadyInMoles=true;				
			}

			/*
			for (irr::u32 j = 0; j < mMoleCount; j++)
			{			
				for (irr::u32 i = 0; i < mMoles[j].count; i++)
				{
					GObjectPtr existingAtom = mMoles[j].atoms[i];
					if (newAtom->GetId() == existingAtom->GetId())
					{
						GControllerComponent* ccomp2 =newAtom->GetComponent<GControllerComponent>(newAtom->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						TElementController* econt2 = (TElementController*)ccomp2->gameController;		
						
						econt2->setHasContact(true);
						alreadyInMoles=true;						
					}
				}
			}*/

		}

		if(!alreadyInMoles)
		{
				
			AudioItem audioUp;
			audioUp.mChunkId = AudioManager::gChunkIndexes[AudioItem::CHUNK_COLLECT1];								
			audioUp.mFlags = AudioItem::PLAYONCE;
			audioUp.mLoop=0;				
			audioUp.mId = GGameApp::getInstance()->getAudioManager()->playSoundNow(audioUp);

			if(mCanVibrate)
			{
				GGameApp::getInstance()->getPreferencesManager()->vibrate();
			}


			int elementId = mole->atoms[0]->GetId();
			std::string s = TGoalManager::intToString(elementId);

			irr::os::Printer::log("Adding totally new mole");
			irr::os::Printer::log(s.c_str());


			mMoles[mMoleCount].setFrom(mole);
			mMoleCount++;
		}
	}

}

void TPlayerController::clearMoles()
{
	mMoleCount=0;

	irr::os::Printer::log("Clear player moles");

	for(int i=0;i<3;i++)
	{
		mMoles[i].clear();
	}

}