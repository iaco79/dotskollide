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
#ifndef GCOMPONENT_CLASS
#define GCOMPONENT_CLASS

#include "GameDefines.h"


class GComponent
{
	friend class GObjectFactory;

protected:

	GObjectPtr mGameObject;

public:
	virtual ~GComponent(void) { mGameObject = NULL; }

    // These functions are meant to be overridden by the implementation classes of the components.
	virtual bool VInit(Json::Value& value) = 0;
	virtual void VPostInit(void) { };
	virtual void VUpdate(int deltaMs){ };
	virtual void VOnChanged(void) { };
	virtual const char *VGetName() const = 0;
 
	virtual GComponent& operator= (const GComponent& other );

	virtual GComponentId VGetId(void) const { return gcomponentId; }

	static GComponentId GetIdFromName(std::string name);

	virtual void SetOwner(GObjectPtr owner) { mGameObject = owner; }
	bool mFrameUpdated; //this is used by gobject to validate if the component was already updated in current frame


protected:
	GComponentId gcomponentId;
	
};

#endif

