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
#include "GBox2dComponent.h"
#include "GObject.h"
#include "GController.h"
#include "Controller/TSynthetizerController.h"


const char* TSynthetizerController::mName = "Synth";


TSynthetizerController::TSynthetizerController()
{
	gcontrollerId = GComponent::GetIdFromName(TSynthetizerController::mName);

	mRadius=0.0f; 
	mTheta=0.0f; 
	mOrigin.Set(0.0f,0.0f);
	mRads=0.0f;
	mTimer = 0;
	mEnableTimer = false;
}


void TSynthetizerController::VUpdate(int deltaMs) 
{
	if(mEnableTimer)
		mTimer = mTimer + deltaMs;

	//set the current angle in rads based on speed
	float32 t = (float32)deltaMs*0.001f;

	//update theta
	mTheta = mTheta + mRads*t;

	//calculate final angle/position of the gameobject in world coordinates
	float32 x = mRadius * cosf(mTheta);
    float32 y =  mRadius * sinf(mTheta);

	//get and update the trasnform component (since box2d will update the trasnform component)
	if(mGameObject)
	{
		GComponentId compId = mGameObject->GetComponentIdByIndex(GC_TRANSFORM_IDX);
		GTransformComponent* bcomp = mGameObject->GetComponent<GTransformComponent>(compId);

		if(bcomp)
			bcomp->mTrans.Set(b2Vec2(mOrigin.x+x,mOrigin.y+y), mTheta);
			
	}

}
bool TSynthetizerController::VInit(const Json::Value& jcomponent) 
{
	
	return true;
}
bool TSynthetizerController::VInit(float32 radius, float32 theta) 
{
	mRadius=radius; 
	mTheta=theta; 
	
	float32 x = mRadius * cosf(mTheta);
    float32 y =  mRadius * sinf(mTheta);

	//get and update the transform component
	//in order to further initialize the box2d component
	if(mGameObject)
	{
		GComponentId transformId = mGameObject->GetComponentIdByIndex(GC_TRANSFORM_IDX);
		GTransformComponent* transform = mGameObject->GetComponent<GTransformComponent>(transformId);

		if(transform)
			transform->mTrans.Set(b2Vec2(x,y), mTheta);
	}

	return true;
}
		

TSynthetizerController& TSynthetizerController::operator= (const TSynthetizerController& other )
{
	GController::operator= (other); //call base assignment operator
	mTimer = 0;
	mEnableTimer = false;
	return *this;
}

