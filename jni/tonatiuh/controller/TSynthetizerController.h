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
#ifndef TSYNTHCONTROLLER_CLASS
#define TSYNTHCONTROLLER_CLASS

#include "GameDefines.h"
#include "GController.h"


/*
 The controller component is an abstract  component 
 that handles the behaviour and state of a particular gameobject
*/
class TSynthetizerController : public GController
{
private:

	//polar coordinates 
	irr::u32 mTimer;

	float32 mRadius; //radius from origin 
	float32 mTheta; //angle in radians
	bool mEnableTimer;
	//polar coordinates origin in world coordinates 
	b2Vec2 mOrigin;

	//angular speed
	float32 mRads;

public:
	TSynthetizerController();
	TSynthetizerController& operator= (const TSynthetizerController& other );
	virtual void VUpdate(int deltaMs) ;
	virtual bool VInit(const Json::Value& jcomponent) ;
	bool VInit(float32 radius, float32 theta);

	//set origin in world coordinates
	void SetOrigin(b2Vec2 origin) {mOrigin = origin;}
	b2Vec2 GetOrigin() {return mOrigin;}

	//set position radius and teta 
	void SetPosition(float32 radius, float32 theta) {mRadius = radius;
													 mTheta = theta;
													}

	  
	float32 GetAngle() {return mTheta;}
	float32 GetRadius() {return mRadius;}


	void SetTimer() { mTimer = 0;}
	irr::u32 GetTimer() { 
					return mTimer;
					}

	//angular speed in rads per second
	void SetAngularSpeed(float32 rads) {mRads = rads;}
	float32 GetAngularSpeed() {return mRads;}


	void SetEnableTimer(bool enable) {mEnableTimer = enable;}
	bool GetEnableTimer() {return mEnableTimer;}

	static const char* mName;
};

#endif
