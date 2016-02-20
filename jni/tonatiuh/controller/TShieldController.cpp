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
#include "GController.h"
#include "GBox2dComponent.h"
#include "GTransformComponent.h"
#include "TonatiuhDefines.h"
#include "Controller/TShieldController.h"



const char* TShieldController::mName = "Shield";

TShieldController::ShieldVertex::ShieldVertex():mFT(NULL)
{
}

void TShieldController::ShieldVertex::Init(irr::u32 initialTime, irr::u32 maxTime, PositionFunction ft)
{
	mMaxTime = maxTime;
	mFT = ft;	
	SetTime(initialTime);

}

void TShieldController::ShieldVertex::SetTime(irr::u32 time)
{
	if(time>mMaxTime)
		time = time - mMaxTime;
	
	mTime = time;

	mPosition = mFT(mTime);

	
}

void TShieldController::ShieldVertex::Update(irr::u32 delta)
{
	SetTime(mTime + delta);
}

TShieldController::TShieldController()
{
	gcontrollerId = GComponent::GetIdFromName(TShieldController::mName);

	mState= SHIELD_STATE_ACTIVE;
	mFixers = 0;
	mPower = 3;

}	


void TShieldController::VUpdate(int deltaMs) 
{ 

	mTimer = mTimer + deltaMs;

	//update the vertex position
	//mShieldVertexes[0].Update(deltaMs);
	//mShieldVertexes[1].Update(deltaMs);
	
	//update the box2d component transform

	b2Vec2 position =   (mShieldVertexes[1].mPosition + mShieldVertexes[0].mPosition) / 2.0f;
	b2Vec2 positionDif = mShieldVertexes[1].mPosition - position;	
	float angle =  b2Atan2(positionDif.y,positionDif.x);
	GBox2dComponent*  bcomp = mGameObject->GetComponent<GBox2dComponent>(mGameObject->GetComponentIdByIndex(GC_BOX2D_IDX));
	bcomp->mBody->SetTransform(position,angle);


	switch(mState)
	{
		case SHIELD_STATE_DAMAGED:
		{
			mPower = 0;
			mFixers  =0;
			bcomp->mBody->SetActive(false);
			mState = SHIELD_STATE_INACTIVE;
		}
		break;
		case SHIELD_STATE_FIXED:
		{
		
			mPower = 3;
			//get the scene node			
			bcomp->mBody->SetActive(true);
			mState = SHIELD_STATE_ACTIVE;
		}
		break;
	}
	
};
	

void TShieldController::SetTime (irr::u32 time) 
{
	mShieldVertexes[0].Update(time);
	mShieldVertexes[1].Update(time);
	mTimer = time;
}
	



void TShieldController::VInit(irr::u32 shieldLenghtTime, irr::u32 shieldPeriodTime, PositionFunction ft)
{
	
	mShieldVertexes[0].Init(  0,shieldPeriodTime,ft);
	mShieldVertexes[1].Init(shieldLenghtTime,shieldPeriodTime,ft);
	
	
}

bool TShieldController::VInit(const Json::Value& jcomponent) 
{
	
	return true;
}

TShieldController& TShieldController::operator= (const TShieldController& other )
{
	GController::operator= (other); //call base assignment operator

	return *this;
}


/* f(t) defines the path for each vertex of a shield object  (time is in ms)
      t1a
    t0-----t1  L2
t7  /       \ t2
   |         |
t1b|         |  t1b
   |         |
t6  \       / t3
   t5 ----- t4  
        t1a
*/
b2Vec2 TShieldController::ShieldPathFunction (irr::u32 time)
{
	float ftime = (float) time /1000.0f;
	const int t1a = (SHIELD_PATH_LENGTH1a/SHIELD_SPEED) * 1000;
	const int t1b = (SHIELD_PATH_LENGTH1b/SHIELD_SPEED) * 1000;

	const int t2 = (SHIELD_PATH_LENGTH2/SHIELD_SPEED) * 1000;
	
	b2Vec2 result(0.0f,0.0f);

	//t0-t1
	if(time>=0 && time < t1a)
	{
		ftime = (float)(time)/1000.0f;
		result.Set(-CRASH_AREA_HALF_WIDTH + SHIELD_PATH_LENGTH2Q + SHIELD_SPEED * ftime,CRASH_AREA_HALF_HEIGHT);
	}
	//t1-t2
	else if (time >= t1a && time < (t1a+t2))
	{
	
		ftime = (float)(time - t1a)/1000.0f;
		result.Set( CRASH_AREA_HALF_WIDTH - SHIELD_PATH_LENGTH2Q , CRASH_AREA_HALF_HEIGHT - SHIELD_PATH_LENGTH2Q );
		float rad =   (SHIELD_SPEED * ftime) / SHIELD_PATH_LENGTH2Q;
		result =  result + b2Vec2(SHIELD_PATH_LENGTH2Q* sinf(rad),SHIELD_PATH_LENGTH2Q* cosf(rad));

	}
	//t2-t3
	else if (time >= (t1a+t2) && time < (t1a+t1b+t2))
	{
		ftime = (float)(time - (t1a+t2))/1000.0f;

		result.Set(  CRASH_AREA_HALF_WIDTH ,
				     CRASH_AREA_HALF_HEIGHT - SHIELD_PATH_LENGTH2Q - SHIELD_SPEED * ftime);
	
	}
	//t3-t4
	else if (time >= (t1a+t1b+t2) && time < (t1a+t1b+2*t2))
	{
		ftime = (float)(time - (t1a+t1b+t2))/1000.0f;


		result.Set( CRASH_AREA_HALF_WIDTH - SHIELD_PATH_LENGTH2Q , -CRASH_AREA_HALF_HEIGHT + SHIELD_PATH_LENGTH2Q );
		float rad =   (SHIELD_SPEED * ftime) / SHIELD_PATH_LENGTH2Q;
		result =  result + b2Vec2(SHIELD_PATH_LENGTH2Q* cosf(rad),SHIELD_PATH_LENGTH2Q* -sinf(rad));

	
	}
	//t4-t5
	else if (time >= (t1a+t1b+2*t2) && time < (2*t1a+2*t2+t1b))
	{
		ftime = (float)(time - (t1a+t1b+2*t2))/1000.0f;

		result.Set(  CRASH_AREA_HALF_WIDTH - SHIELD_PATH_LENGTH2Q -  SHIELD_SPEED * ftime,
				    -CRASH_AREA_HALF_HEIGHT);
	}

	//t5-56
	else if (time >= (2*t1a+2*t2+t1b) && time < (2*t1a+3*t2+t1b))
	{
		ftime = (float)(time - (2*t1a+2*t2+t1b))/1000.0f;

		result.Set( -CRASH_AREA_HALF_WIDTH+ SHIELD_PATH_LENGTH2Q , -CRASH_AREA_HALF_HEIGHT + SHIELD_PATH_LENGTH2Q );
		float rad =   (SHIELD_SPEED * ftime) / SHIELD_PATH_LENGTH2Q;

		result =  result + b2Vec2(SHIELD_PATH_LENGTH2Q* -sinf(rad),SHIELD_PATH_LENGTH2Q* -cosf(rad));
	}
	//t6-t7
	else if (time >= (2*t1a+3*t2+t1b) && time < (2*t1a+3*t2+2*t1b))
	{

		ftime = (float)(time - (2*t1a+3*t2+t1b))/1000.0f;

		result.Set(  -CRASH_AREA_HALF_WIDTH,
		 		     -CRASH_AREA_HALF_HEIGHT + SHIELD_PATH_LENGTH2Q + SHIELD_SPEED * ftime);
	}
	//t7-t8
	else
	{
		ftime = (float)(time - (2*t1a+3*t2+2*t1b))/1000.0f;

		result.Set( -CRASH_AREA_HALF_WIDTH + SHIELD_PATH_LENGTH2Q , CRASH_AREA_HALF_HEIGHT - SHIELD_PATH_LENGTH2Q );
		float rad =   (SHIELD_SPEED * ftime) / SHIELD_PATH_LENGTH2Q;
		result =  result + b2Vec2(SHIELD_PATH_LENGTH2Q* -cosf(rad),SHIELD_PATH_LENGTH2Q* sinf(rad));

	
	}

	return result;
}
