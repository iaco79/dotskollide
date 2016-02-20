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
#ifndef TGOALMANAGER_CLASS
#define TGOALMANAGER_CLASS

#include "GameDefines.h"
#include "TonatiuhDefines.h"


//Evaluates objects present in the scene and selects a crasheable goal

struct LevelValues
{
	int maxJoin;
	int launchSpeed;
	int maxObjects;
	int maxLaunch;
	int maxBad;
	int colorCount;
	int moleSize;
	int maxColors;	
} ; 


struct Mole
{ int count;
	int colors[3];
	GObjectPtr atoms[3];	  

	//sets values using other mole
	void set(Mole* other);
	void clear();

};


struct SafeMole
{
	int count;
	int colors[3];
	int objectIds[3];	  

	//sets values using other mole
	void setFrom(Mole* other);
	void populateMole(Mole* other, irr::core::array<GObjectPtr>& elements);
	void clear();
};

class CanCollideQuery : public b2QueryCallback
{

	bool ReportFixture(b2Fixture* fixture);

	b2CircleShape m_circle;
	b2Transform m_transform;
	GObjectPtr mPlayer;
	int mMaxJoin;
	int mMinCrash;
	int mColorCount;
	int mMaxSwap;
	b2World* mWorld;
	irr::u32* mObjectIdList;
	irr::u32 mObjectIdListSize;


public:
	CanCollideQuery(GObjectPtr player,b2World* world, irr::u32* objectIdList,irr::u32 objectIdListSize);
	CanCollideQuery();

		
	void setMaxJoin(int value) {mMaxJoin = value;}
	int getMaxJoin() {return mMaxJoin;}
	void setMinCrash(int value) { mMinCrash = value;}
	int getMinCrash() { return mMinCrash;}
	void setColorCount(int value) { mColorCount = value;};
	int getColorCount() { return mColorCount;};

	void setMaxSwap(int value) {mMaxSwap = value;}
	int getMaxSwap() {return mMaxSwap;}

	static GController* GetElementController(GObjectPtr element);		

	void Execute();
	
	bool IsCrashing(Mole* moles, int count, int& mole1crashed, int& mole2crashed, int& mole3crashed);
	int  IsJoining(Mole* moles, int count);

	//return 1 if it is a swap
	//return 2 if it is an special join
	int  IsSwaping(Mole* moles, int count);


	bool IsClosing(GObjectPtr* elements, int count);
	bool IsOpening(GObjectPtr* elements, int count, GObjectPtr* currentTestObject);
	
	


	int32 m_count;
	int32 m_countEnergy;
	int32 m_countEnergy1;
	int32 m_countEnergy2;
	int32 m_countEnergy3;
	int32 m_countEnergy4;
	int32 m_countEnergy5;
	int32 m_countTriangle;
	int32 m_countBox;
	int32 m_countCross;
		

};


struct CrashGoal 
	{
		int colors[5];
		int diferentColors;
		int totalColors;	
		int moleCount;
		bool hasTrio;
		bool hasDuo;
		void reset();
		void set(CrashGoal* other);
		void setFromMoles(Mole* testMoles, int moleCount);
		bool isEqual(CrashGoal* other);
		 
		void ensureColorsAreEqual();
		void removeOneRandomColor();


	};
class TGoalManager 
{
	public:
	
	static int mColorCombinations2[20];	
	static int mColorCombinations3[30];
	static int mColorCombinations4[20];
	static int mGoalCombinations[18][4];


	//the list of all the elements in the scene
	irr::core::array<GObjectPtr>* mElementList;
	
	int mMoleCount;

private:
	GOAL_STATE mGoalState;
	bool mHasGoal;
	LevelValues mLevelValues;

	//rules validator
	CanCollideQuery mCollideQuery;
	
	Mole mMoles[20];
	int mShuffle[20];	
	
	int mTmpGoalCount;	
	CrashGoal mtmpCrashGoals[5]; //test goals



	//CrashGoal mCrashGoals[10]; //current goal list
	//int mGoalCount;


	int mgI;
	int mgJ;
	int mgK;
	int mgL;
	int mCycleCount;
	//int mGoalIndex;
	int GetSceneMoles(Mole* mTestMoles, int maxMoles);

	bool DoLogicFindGoal4(int gi,int gj,int gk,int gl);
	bool DoLogicFindGoal3(int gi,int gj,int gk);
	bool DoLogicFindGoal2(int gi,int gj);
	bool mSceneIsCrashable;

public:
	static std::string intToString(int i);
		
	TGoalManager();
	

	bool IsSceneCrasheable() {return mSceneIsCrashable;}

	void Initialize(irr::core::array<GObjectPtr>* elements);

	//mole manipulation helpers
	void clearMolesFlags(Mole* testMoles, int moleCount);
	void doMoleJoin(Mole* mole3, Mole* moleX, Mole* moleY);		
		
	//validates is the crashed moles are the current objective
	bool validateCrashIsObjective(int moleCount,int &points);
	
	//Test if the moles are a potential goal
	bool testMolesAreGoal(Mole* testMoles,int moleCount, CrashGoal* crashGoal);
	
	//validates that the scene is crasheable
	void startMonitoringScene(LevelValues levelValues);
	

	//true is the current goal is valid 
	//bool hasGoal() 	{		return mGoalIndex >= 0;	};

	//int getGoalCount()		{return mGoalCount;}

	//returns the goal search state
	GOAL_STATE GoalState()
	{
		return mGoalState;
	};

	
	//int SetNextGoal()	{		mGoalIndex--;		if(mGoalIndex<0)			mGoalIndex=-1;		return mGoalIndex;	}


	//void unsetGoal()			{		mGoalIndex = -1;	};

	

	//returns the current goal
	/*
	CrashGoal& CurrentGoal()
	{
		return mCrashGoals[mGoalIndex];

	};
	*/

	//updates the goal search
	//specifies the max # of ms available to search	
	void update(int elapsedMS, int max);	
	
	//int calculateGoals();
	int fact(int n);
};

#endif