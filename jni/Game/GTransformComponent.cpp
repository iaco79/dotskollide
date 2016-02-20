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
#include "GTransformComponent.h"

const char* GTransformComponent::mName = "Transform";


GTransformComponent::GTransformComponent()
{
	mZ = 0.0f;
	gcomponentId = GComponent::GetIdFromName(GTransformComponent::mName);

}



void GTransformComponent::VInit(b2Transform& trans)
{
	mTrans = trans;
}

bool GTransformComponent::VInit(Json::Value& jcomponent)
{


	float32 angle;	
	b2Vec2 pos;

	angle = (irr::f32)jcomponent["fRotation"].asDouble();
		
	pos = b2Vec2(
		(float32)jcomponent["vPosition"][0u].asDouble(),
		(float32)jcomponent["vPosition"][1].asDouble());

	
	mTrans.Set(pos, 0.0f);

	
	return true;
}

GTransformComponent& GTransformComponent::operator= (const GTransformComponent& other )
{
	GComponent::operator= (other); //call base assignment operator

	mTrans = other.mTrans;
	mZ = other.mZ;

	return *this;
}
