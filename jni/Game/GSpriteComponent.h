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
#ifndef GSPRITECOMPONENT_CLASS
#define GSPRITECOMPONENT_CLASS

#include "GameDefines.h"
#include "GComponent.h"


class GSpriteComponent : public GComponent
{
public:
	
	irr::core::vector2di mUV1;
	irr::core::vector2di mUV2;
	irr::core::vector2df mSize;
	irr::core::vector2df mScale;
	irr::video::SColor mColor;
	irr::video::SColor mColor2;
	bool mVisible;
	bool mUse2Colors;

	irr::core::stringc mSpriteSheet;
	
	virtual GSpriteComponent& operator= (const GSpriteComponent& other );

	GSpriteComponent();

	static const char* mName;
	virtual const char* VGetName() const { return mName; }

	bool VInit(irr::core::stringc spriteSheet,irr::core::vector2di UV1,irr::core::vector2di UV2, irr::core::vector2df size);

	virtual bool VInit(Json::Value& jcomponent) ;

 
};

#endif