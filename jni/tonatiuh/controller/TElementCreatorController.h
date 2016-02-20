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
#ifndef TELCREATORCONTROLLER_CLASS
#define TELCREATORCONTROLLER_CLASS

#include "GameDefines.h"
#include "GController.h"


/*
 The controller component is an abstract  component 
 that handles the behaviour and state of a particular gameobject
*/
class TElementCreatorController : public GController
{
private:

	b2Vec2 mOrigin;
	irr::u32 mTimer;
	int mState;
	bool mActive;
	irr::u32 mCreationTime;
	irr::u32 mFinishTime;
	GObjectPtr mSpringObject; 
	int mSpringIndex;
	float mAngle;
	float mSpeed;
	public:
	TElementCreatorController();
	TElementCreatorController& operator= (const TElementCreatorController& other );
	virtual void VUpdate(int deltaMs) ;
	virtual bool VInit(const Json::Value& jcomponent) ;
	bool VInit();

	//set origin in world coordinates
	void setOrigin(b2Vec2 origin) {mOrigin = origin;}
	b2Vec2 getOrigin() {return mOrigin;}

	void setState(int state) { mState = state;}
	irr::u32 getState() { 
					return mState;
					}

	void Start(b2Vec2 origin, float speed,float angle, GObjectPtr springObject);
	void SetSpringObject(GObjectPtr springObject) {mSpringObject = springObject;}

	
	
	void setTimer() { mTimer = 0;}
	irr::u32 getTimer() { 
					return mTimer;
					}

	void setActive(bool active) { mActive = active;}
	irr::u32 getActive() { 
					return mActive;
					}

	static const char* mName;
};

#endif
