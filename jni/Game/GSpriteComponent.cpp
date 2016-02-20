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
#include "GSpriteComponent.h"

const char* GSpriteComponent::mName = "Sprite";

GSpriteComponent::GSpriteComponent()
{
	gcomponentId = GComponent::GetIdFromName(GSpriteComponent::mName);

	mScale.set(1.0f, 1.0f);
	mColor.set(255,255,255,255);
	mColor2.set(255,255,255,255);
	mUse2Colors=false;
	mVisible =true;

}


bool GSpriteComponent::VInit(irr::core::stringc spriteSheet,
			irr::core::vector2di UV1,
			irr::core::vector2di UV2, 
			irr::core::vector2df size)
{
	mSpriteSheet= spriteSheet; 
	mUV1.set(UV1);
	mUV2.set(UV2);
	mSize.X = size.X;
	mSize.Y = size.Y;
	

	return true;
}

bool GSpriteComponent::VInit(Json::Value& jcomponent)
{
	mSpriteSheet = jcomponent["sSpriteSheet"].asString().c_str();
	mUV1 = irr::core::vector2di(
		jcomponent["vXY1"][0u].asInt(),
		jcomponent["vXY1"][1].asInt());

	mUV2 = irr::core::vector2di(
		jcomponent["vXY2"][0u].asInt(),
		jcomponent["vXY2"][1].asInt());

	mSize =  irr::core::vector2df(
			jcomponent["vSize"][0u].asDouble(),
			jcomponent["vSize"][1].asDouble());
	
	return true;
}

GSpriteComponent& GSpriteComponent::operator= (const GSpriteComponent& other )
{
	GComponent::operator= (other); //call base assignment operator
	mUse2Colors = other.mUse2Colors;
	mUV1 = other.mUV1;
	mUV2 = other.mUV2;
	mSize = other.mSize;
	mSpriteSheet = other.mSpriteSheet;

	return *this;
}
