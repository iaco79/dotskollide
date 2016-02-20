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
#ifndef TPLAYERCONTROLLER_CLASS
#define TPLAYERCONTROLLER_CLASS

#include "GameDefines.h"
#include "GController.h"


/*
 The controller component is an abstract  component 
 that handles the behaviour and state of a particular gameobject
*/
struct MovementData 
{
	float x;
	float y;
	float speedX;
	float speedY;
};


class TPlayerController : public GController
{
	
private:
	
	CRASH_STATE mCrashState;
	MOLE_STATE mMoleState;
	bool  mApplyingForce;
	float mForceFactor;
	
	bool  bCollideOnStopForce;
	float mAttractionForceRadius;
	float mValidCollideRadius;
	float mCursorRadius;
	bool bInvinsible;
	

		//elapsed time in ms
	irr::u32 mElapsedTime;

public:
	int mTotalCount;
	int mGoalCount;
	
	int mCircleCount;
	float mRadiation;
	int mElementsInArea;

	int mCountA;
	int mCountB;
	int mCountC;
	int mCountD;
	int mCountE;

	int mBadCount;
	int mGoodCount;
	int mDifferentColors;
	int mBestColor;
	int mLevel;
	int mLevelChanged;

	int mCrashCount;
	int mMaxJoin;
	int mColorCount;
	bool mScoreChanged;
	bool mLevelCrashesChanged;
	bool mGoalCountChanged;
	int mScore;
	int mLevelCrashes;
	float mEnergy;
	bool mCanVibrate;

	b2Vec2 mPlayerOffset;
	
	std::queue<MovementData> positionQueue;

	void PositionQueueAdd(MovementData data);
	void PositionQueueClear();
	bool PositionQueuePop(MovementData& data);


	float mF;
	TPlayerController();
	TPlayerController& operator= (const TPlayerController& other );
	virtual void VUpdate(int deltaMs);
	virtual bool VInit(const Json::Value& jcomponent) ;
	void VInit(float cursorRadius, float validCollideRadius, float attractionRadius);

		
	void setCrashState(CRASH_STATE crashState) {mCrashState = crashState;}
	CRASH_STATE  getCrashState() {return mCrashState;}

	void setMoleState(MOLE_STATE moleState) {mMoleState= moleState;}
	MOLE_STATE  getMoleState() {return mMoleState;}


	void setLevelCrashes(int value) { 

								if(mLevelCrashes != value)
								{	mLevelCrashesChanged = true;
									mLevelCrashes= value;
								};
	}

	void setCanVibrate(bool value)
	{
		
		mCanVibrate =  value;
	};

	void setLevel(int value)
	{
		mLevel = value;
		mLevelChanged=true;
	};

	void setScore(int value ) { 
						if(mScore != value)
						{   mScoreChanged = true;
							mScore = value;
						};
	}

	void  setGoalCount(int value) {
							mGoalCountChanged = true;
							mGoalCount = value;
							}
	int  getGoalCount() {return mGoalCount; }


	void  setApplyForce(bool forceEnable) ;
	bool  getApplyForce() {return mApplyingForce; }

	
	
	void setForceFactor(float factor) {mForceFactor = factor;}
	float getForceFactor() {return mForceFactor; } 

	void setValidCollideRadius(float radius) { mValidCollideRadius = radius;}
	float getValidCollideRadius() {return mValidCollideRadius;}

	void setAttractionForceRadius(float radius) {mAttractionForceRadius= radius;}
	float getAttractionForceRadius () {return mAttractionForceRadius;}


	void setCursorRadius(float radius) {mCursorRadius= radius;}
	float getCursorRadius() {return mCursorRadius;}

	void setCollideOnStopForce(bool value) { bCollideOnStopForce = value;}
	bool getCollideOnStopForce() {return bCollideOnStopForce;}

	
	

	void setInvincible(bool value)
	{
		bInvinsible = value;
	}

	bool getInvincible()
	{
		return bInvinsible;
	}


	void addEnergy(float factor);
	
	void removeEnergy(float factor);	

	void addRadiation(float factor);
	
	void removeRadiation(float factor);	

	static const char* mName;

	void AddMole(Mole* mole);
	void clearMoles();



	SafeMole mMoles[3];
	int mMoleCount;


};

#endif