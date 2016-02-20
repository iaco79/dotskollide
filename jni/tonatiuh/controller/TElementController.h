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
#ifndef TELEMENTCONTROLLER_CLASS
#define TELEMENTCONTROLLER_CLASS

#include "GameDefines.h"
#include "GController.h"


/*
 The controller component is an abstract  component 
 that handles the behaviour and state of a particular gameobject
*/
class TElementController : public GController
{
public:
	// 0= green
	// 1= yellow
	// 2= red/orange
	int mColorId;
	int mNewColorId;
	irr::video::SColor mColor;
	irr::video::SColor mOriColor;
	float mRadiationFactor;
	CRASH_STATE mCrashState;
	ELEMENT_CYLCE mLifeCycle;
	bool mCounted;

private:
	int mElementType;
	
	bool mInCrashForce;
	bool mHasContact;
	int mCrashOperation;
	irr::u32 mContactTimer;
	bool mHasContactTimer;
	bool mHasPlayerContact;
	GObjectPtr mSynth;
	irr::u32 mTimer;
	float mInitialSpeed;	
	int mCreationTime;
	bool mInAttraction;
	bool mAllowAttraction;
	bool mIsTemporaryCrash;


public:
	TElementController();
	TElementController& operator= (const TElementController& other );
	virtual void VUpdate(int deltaMs);
	virtual bool VInit(const Json::Value& jcomponent) ;
	void VInit(int elementType);

	void setContactTimer(int value)  { mContactTimer = value;}
	int  getContactTimer()  { return mContactTimer;}

	void setHasContactTimer(bool value)  { mHasContactTimer = value;}
	bool getHasContactTimer()  { return mHasContactTimer;}

	void setCrashOperation(int crashOperation)  { mCrashOperation = crashOperation;}
	int  getCrashOperation()  { return mCrashOperation;}

	void setIsTemporaryCrash(bool isCrash)  { mIsTemporaryCrash = isCrash;}
	bool getIsTemporaryCrash()  { return mIsTemporaryCrash;}

 	void setInCrashForce(bool incrashForce)  { mInCrashForce = incrashForce;}
	bool getInCrashForce()  { return mInCrashForce;}


	
	void setHasContact(bool hasContact)  { mHasContact = hasContact;}
	bool getHasContact()  { return mHasContact;}


	void setHasPlayerContact(bool hasPlayerContact);
	bool getHasPlayerContact()  { return mHasPlayerContact;}

	bool getInAttraction () {return mInAttraction;}
	void setInAttraction (bool value) {mInAttraction=value;}

	bool getAllowAttraction () {return mAllowAttraction;}
	void setAllowAttraction (bool value) {mAllowAttraction=value;}

	int getElementType()  { return mElementType;}

	void setCrashState(CRASH_STATE crashState) {mCrashState = crashState;}
	CRASH_STATE  getCrashState() {return mCrashState;}

	void setInitialSpeed(float value) {mInitialSpeed = value;}
	float getInitialSpeed() {return mInitialSpeed;}
	void setSynth(GObjectPtr synth);
	void FreeSynth();

	static const char* mName;
};

#endif