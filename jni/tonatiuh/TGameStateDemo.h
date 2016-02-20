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
#ifndef TGAMESTATEDEMO_CLASS
#define TGAMESTATEDEMO_CLASS

#include "GameDefines.h"
#include "GGameState.h"
#include "menu/GRocketEventHandler.h"
#include "TonatiuhDefines.h"


class TGameStateDemo : public GGameState, 
					GRocketEventHandler,  //<-- listen to rockete events
					b2ContactListener, //<-- listen to physics events
					public GScriptable //<-- make this scriptable
{



private:

	struct PositionInfo
	{
		b2Vec2 position;
		int color;
		int fromR;
		int toR;
	};

	SmoothMotionInfo mSmoothPoints;

	static TGameStateDemo* mInstance;
	
	//GObjectPtr mLastPlayerContact;
	int mClickCount;

	
	static u32 mFireColors[12];
	int mFireColorIndex;
	bool mIsMouseDown;



	struct WorldSettings
	{
		WorldSettings():
			hz(60.0f),
			velocityIterations(6),
			positionIterations(2),
			enableWarmStarting(1),
			enableContinuous(1),
			enableSubStepping(0)
			{};

	
		float32 hz;
		int32 velocityIterations;
		int32 positionIterations;	
		int32 enableWarmStarting;
		int32 enableContinuous;
		int32 enableSubStepping;
	};

	WorldSettings mWorldSettings;

	irr::scene::ICameraSceneNode* mCamera;
	irr::scene::ICameraSceneNode* mTargetCamera;

	GObjectPtr mPlayer;
	GObjectPtr mSquareShield;	
	GObjectPtr mBottomShield;	
	GObjectPtr mSpringGrid;
	GObjectPtr mDummy;
	GObjectPtr mEnergyBar;
	GObjectPtr mRadiationBar;
	GObjectPtr mMoleIndicatorKind;
	GObjectPtr mMoleIndicatorCount;
	GObjectPtr mStarBackground;

	void SetGameMode(int mode);


	float mStarFieldAngle;
	int mBatchedSpriteNodeId;
	int mBatchedSpriteNodeIdBits;
	int mBatchedSpriteNodeStaticShieldId;

	int mShieldNodeId;
	int mSpringNodeId;
	int mJointsNodeId;
	int mTestNodeId;
	
	int mMolesC1;
	int mMolesC2;
	int mMolesC3;

	irr::u32 mNextLevelTimer;
	bool mDoNextLevelAnimation;
	irr::u32 mElapsedTime;
	irr::u32 mFrameCount;
	int  mShieldCount;
	irr::u32 mShieldCreationTime;
	irr::u32 mLaunchTime;
	irr::u32 mWaitTime;
	irr::u32 mExplosionTime;
	bool mWaitingForClear;
	bool mWaitingForJoin;
	bool mWaitingForCrash;
	bool mWaiting;
	irr::video::ITexture* renderTarget1;
	irr::video::ITexture* renderTarget2;
	irr::video::ITexture* colorTexture;
	irr::video::ITexture* metaTexture;
	irr::video::ITexture* alphaTexture;
	
	//color animation
	irr::video::SColor mColorTo;
	irr::video::SColor mColorFrom;
	irr::f32 mColorFactor;
	irr::f32 mColorSpeed;		
	irr::s32 mColorIndex;
	irr::video::SColor mColorPalete[4];
		

	b2World* mWorld;
	bool mPaused; //pause the game state flag
	bool mMenuVisible; //toggle show hide menu
	int mGameMode;
	bool mTutorialVisible;
	bool mTutorialLoaded;

	//use this query to apply force to the elements affected by player attraction force
	class ApplyForceQuery : public b2QueryCallback
	{
		
		
		bool ReportFixture(b2Fixture* fixture);				
		int mCount;
		float mCursorRadius;
		float mForceRadius;
		b2Vec2  mForceCenter;
		GObjectPtr mPlayer;
		b2World* mWorld;
		bool mFactor;

	public:
		ApplyForceQuery(GObjectPtr player,b2World* world, float factor);
		void Execute();
	};

	//use this query to count the # of elements that can be crashed within player's collide radius
	
public:
	
	TGameStateDemo(void);
	virtual ~TGameStateDemo(void);
	virtual int VGameStateId();


	virtual void VOnGetFocus(int param);
	virtual void VOnLostFocus();
	virtual void VInit() ;
	virtual void VReInitRenderContext();
	virtual void VFinish() ;
	virtual void VUpdate(int deltaMs);
	virtual void VRender(IrrlichtDevice* device);
	virtual void OnKeyDown(irr::EKEY_CODE keycode);
	virtual void OnKeyUp(irr::EKEY_CODE keycode);
	virtual void OnMouseDown(int x, int y);
	virtual void OnMouseUp(int x, int y);
	virtual void OnMouseMove(int x, int y);
	virtual void ProcessRocketEvent(const Rocket::Core::String& value);

	virtual void BeginContact(b2Contact* contact); 
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	
	int GetGameMode() {return mGameMode;}
	void DoLogicPostSimulation(int deltaMs);
	void DoLogicPreSimulation(int deltaMs);
	int ProcessCrashedObjects();
	void ProcessProcessedMolecules();
	void EnsurePlayerIsInCrashArea();

	void ClearElements();

	GObjectPtr CreatePlayerObject();
	GObjectPtr CreateSynth(float32 radius, float32 theta);
	GObjectPtr CreateSpringGrid();

	GObjectPtr CreateDummyObject();	
	GObjectPtr CreateShield(irr::u32 time);
	GObjectPtr CreateFireworkNode();
	GObjectPtr CreateBarNode(GObjectPtr player);
	GObjectPtr CreateRadiationBarNode(GObjectPtr player);
	
	void SetupRenderTargets(bool reinit);
	void SetupTextures();

	void SetupSceneNodes(bool reinit);

	void SetupStaticShieldNodes();
	void SetupCreatorsPool(bool reinit);
	void SetupColorBars(bool reinit);
	void SetupStarBackground(bool reinit);
	void SetupElementsSpriteNode(bool reinit);
	void SetupFireworkPool(bool reinit);
	void SetupBitsSpriteNode(bool reinit);
	void SetupStaticShieldSpriteNode(bool reinit);
	void SetupDynamicShieldNode(bool reinit);
	void SetupJointsSpriteNode(bool reinit);
	void AnimatePalete();


	//void CreateElement(int elementId, int level);
	GObjectPtr GetAvailableCreator();

	void ResetGame(int level);
	
	void CreateExplosionAt(b2Vec2 position, bool sound, int type,u32 color = 0xFFFFFFFF);
	
	
	GObjectPtr CreateElementAtXYInternal(int elementId, int color, float x,float y, bool isStatic = false, bool withSynth = true);

	
	int CalculateLevel();
	void SetLevelValues(int maxJoin,int maxObjects,int maxLaunch,int maxBad,int colorCount,int moleSize,int maxColors);
	void ShowTutorialMenu(bool show);
	

	void WaitForCrash(int ms);
	void WaitForClear(int ms);
	void WaitForJoin(int ms);
	void CreateElementAtXY         (int x,int y, int element);
	void CreateElementAtXYSpeed    (int x,int y, int angle, int speed,int element);
	void CreateTetra(int x, int y, int radius, int angle,int elementA, int elementB,int elementC, int elementX);
	void CreateTri(int x, int y, int radius, int angle, int elementA,  int elementB, int elementC);
	void CreateTriNoSynth(int x, int y, int radius, int angle, int elementA,  int elementB, int elementC);
	void CreateTri2(int x, int y, int radius, int angle, int elementA,  int elementB, int elementC);
	void CreateTri2NoSynth(int x, int y, int radius, int angle, int elementA,  int elementB, int elementC);

	void CreateDuo(int x, int y, int radius, int angle, int elementA,  int elementB);
	void CreateDuoNoSynth(int x, int y, int radius, int angle, int elementA,  int elementB);
	void CreateElementAtRandXY(int element);
	void CreateElementDistributed(int count);
	void CreateElementDistributedInternal(int count, int maxelements);

	int GetMoleCount();

	int  IsRunningTutorial();
	void OnTutorialFinished();
	void WaitForTap(int ms);
	void SetTutorialText(char* text, int nextButton);


	GObjectPtr CreateSquareShield();
	GObjectPtr CreateBottomShield();


	void ProcessFinishedBits();
	void ClearBits();
	void ClearShield();
	void AddSelectedIdsToPlayer();

	
	void RefreshColorBars(irr::u32 ms);
	
	void DoLevelUpAnimation(irr::u32 ms);

	void UpdateStarBackground(irr::u32 ms);
	//gscriptable
	virtual void OnStartScript();		
	virtual void OnScriptFinished();	
	virtual void OnSetScript(OOLUA::Script *script);

	//crash validations
	//refreshes the mole list and returns the # of moles detected
	//if moles detected 
	//int RefreshCrasheableMoles();
	
	//mcrasheable elements is a list of 3 moles
	
	//count up to 30 moles	
	//Mole mNewMoles[3];
	
	bool mDidCrash;
	bool mCrashWasObjective;
	bool mDidJoin;
	int  mRTSize;


	int mNewMoleCount;
	int mAddType;
	TUTORIAL_STATE mTutorialState;
	bool mWaitingForTap;

	//0:crash 2 + join 2
	//1:crash 3 + join 3
	//2:crash 4 + join 3 + swap 
	//int mGameMode;

	TGoalManager mGoalManager;
	LevelValues mLevelValues;
	irr::u32 mSelectedIds[16];

	irr::core::array<GObjectPtr> mElementList;	//an array containing the active elements
	irr::core::array<GObjectPtr> mShieldList;	//an array containing the shield objects
	irr::core::array<GObjectPtr> mBitList;	//an array containing the list of bits
	irr::core::array<GObjectPtr> mStaticShieldList;	//an array containing the list of bits
	irr::core::array<GObjectPtr> mFireworkPool;	//an array containing a pool of fireworks
	irr::core::array<GObjectPtr> mCreatorsPool;//an array containing a pool of creators
	irr::core::array<GObjectPtr> mColorBars;//an array containing a pool of color bars

};


OOLUA_CLASS_NO_BASES(TGameStateDemo)//class has no bases
OOLUA_NO_TYPEDEFS
OOLUA_ONLY_DEFAULT_CONSTRUCTOR	
//No_public_constructors
OOLUA_MEM_FUNC(void,OnScriptFinished)
OOLUA_MEM_FUNC(void,CreateElementDistributed,int)
OOLUA_MEM_FUNC(void,CreateElementAtXY         ,int,int,int)
OOLUA_MEM_FUNC(void,CreateDuo,int,int,int,int,int,int)
OOLUA_MEM_FUNC(void,WaitForCrash,int) 
OOLUA_MEM_FUNC(void,WaitForClear,int) 
OOLUA_MEM_FUNC(void,OnTutorialFinished) 
OOLUA_MEM_FUNC(void,SetTutorialText, char*, int)
OOLUA_MEM_FUNC(void,SetLevelValues,int,int,int,int,int,int,int)
OOLUA_MEM_FUNC(void,WaitForTap,int)
OOLUA_MEM_FUNC(void,WaitForJoin,int)
OOLUA_MEM_FUNC_0(int,IsRunningTutorial) 
OOLUA_MEM_FUNC_0(int,CalculateLevel)
OOLUA_MEM_FUNC_0(int,GetMoleCount)
OOLUA_CLASS_END

#endif