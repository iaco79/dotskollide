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
#ifndef GCONTROLLER_CLASS
#define GCONTROLLER_CLASS

#include "GameDefines.h"


/*
 The controller component is an abstract  component 
 that handles the behaviour and state of a particular gameobject
*/
class GController 
{
public:
	GObjectPtr mGameObject;
	
	virtual GController& operator= (const GController& other );
	virtual void VUpdate(int deltaMs) { }
	virtual bool VInit(const Json::Value& jcomponent) = 0;
	
	virtual GControllerId VGetId(void) const { return gcontrollerId; }


	GControllerId gcontrollerId;
};

#endif