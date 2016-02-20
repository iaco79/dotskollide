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
//This is the main game routine.

#include "TonatiuhDefines.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <CIrrDeviceStub.h>
#include "os.h"
#include "GGameApp.h"
#include "GObject.h"
#include "GObjectFactory.h"
#include "GController.h"
#include "GTransformComponent.h"
#include "GBox2dComponent.h"
#include "GControllerComponent.h"
#include "GSpriteComponent.h"
#include "GIrrlichtComponent.h"
#include "irrnode/Base2dNode.h"
#include "irrnode/B2dNode.h"
#include "irrnode/SpriteNode.h"
#include "irrnode/BatchedSpriteNode.h"
#include "preferences/PreferencesManager.h"
#include "springrid/SpringGrid.h"
#include "shaders/SpiralShader.h"
#include "shaders/FireworkShader.h"
#include "shaders/MetaballShader.h"
#include "shaders/MetaballShaderPre.h"
#include "spline/spline.h"
#include "scenenode/SpringNode.h"
#include "scenenode/ShieldNode.h"
#include "scenenode/JointsNode.h"
#include "SceneNode/FireworkNode.h"
#include "SceneNode/EnergyBarNode.h" 
#include "SceneNode/SpiralNode.h"
#include "component/TSpringGridComponent.h"
#include "controller/TElementController.h"
#include "controller/TSynthetizerController.h"
#include "TGoalManager.h"
#include "Controller/TPlayerController.h"
#include "Controller/TShieldController.h"
#include "Controller/TFireWorkController.h"
#include "Controller/TEnergyBarController.h"
#include "Controller/TElementCreatorController.h"
#include "Controller/TBitController.h"
#include "TElementsFactory.h"


#include "TGameStateDemo.h"


#if defined(_IRR_ANDROID_PLATEFORM_)
	#include <android/log.h>
	#define T_USE_OFFSET (0)
#else
	#define T_USE_OFFSET (0)
#endif

#define MAX_FIREWORKS (20)
#define MAX_CREATORS (20)

//#define DUO_WIDTH 6
//#define TRI_WIDTH 7
//#define TRI2_WIDTH 12

#define DUO_WIDTH 5
#define TRI_WIDTH 6
#define TRI2_WIDTH 9
/*
#define COHESION_FACTOR (0.01f)
#define SEPARATION_FACTOR (0.02f)
#define SLOW_DOWN_FACTOR (0.99f)
#define WALLAVOID_FACTOR (0.05f)
#define PLAYER_AVOID_FACTOR (0.05f) 
*/
#define COHESION_FACTOR (0.02f)
#define SEPARATION_FACTOR (0.05f)
#define SLOW_DOWN_FACTOR (0.95f)
#define WALLAVOID_FACTOR (0.05f)
#define PLAYER_AVOID_FACTOR (0.02f) 




EXPORT_OOLUA_FUNCTIONS_NON_CONST(TGameStateDemo,CalculateLevel,SetLevelValues,OnScriptFinished,CreateDuo,CreateElementAtXY,WaitForClear,WaitForJoin,CreateElementDistributed,WaitForCrash,OnTutorialFinished,SetTutorialText,IsRunningTutorial,WaitForTap,GetMoleCount)
EXPORT_OOLUA_FUNCTIONS_0_CONST(TGameStateDemo)




TGameStateDemo* TGameStateDemo::mInstance=NULL;


 
u32 TGameStateDemo::mFireColors[12] =
			{	0xFFFF66B2,
				0xFFFF66FF,
				0xFFB266FF,
				0xFF6666FF,
				0xFF66B2FF,
				0xFF66FFFF,
				0xFF66FF66,
				0xFF66FFFF,
				0xFF66B2FF,
				0xFF6666FF,
				0xFFB266FF,
				0xFFFF66FF
			};

int TGameStateDemo::VGameStateId()
{
	return (int)GAME_STATE_GAME;

}

TGameStateDemo::TGameStateDemo() :GGameState()
{
	mTutorialState = TUTORIAL_STATE_NONE;
	mTutorialLoaded = false;

	mPlayer = NULL;	
	mSpringGrid = NULL;
	mSquareShield= NULL;
	mMoleIndicatorCount=NULL;
	mMoleIndicatorKind=NULL;
	mGameMode=0;

	mDidCrash = false;
	mDidJoin = false;
	mCrashWasObjective = false;
	mStarBackground=NULL;
	mDoNextLevelAnimation=false;
	mNextLevelTimer=0;
	mBatchedSpriteNodeStaticShieldId=0;
	//mLastPlayerContact= NULL;
	//mMoleCount = 0;

	mPaused = false;
	mMenuVisible = false;
	mElapsedTime=0;
	mFrameCount = 0;
	mShieldCount=0;
	mShieldCreationTime = (SHIELD_SIZE_TIME);
	mSpringNodeId=0;
	mLaunchTime = 0;
	mWaitTime = 500;
	mRTSize = 256; //default rt size
	mTutorialVisible=false;
	mWaiting=false;
	mWaitingForClear=false;
	mWaitingForJoin=false;
	mWaitingForCrash=false;
	mWaitingForTap= false;

	mFireColorIndex = 0;
	//lua script
	mScript=NULL;
	mScriptState = (int)GScriptable::NONE;

	mIsMouseDown = false;

	mAddType = 0;

	
	mGoalManager.Initialize(&mElementList);
	
	
	
	mInstance = this;

	//set the color palete animation

	mColorFrom.set(255,255,0,255);
	mColorTo.set(255,0,255,255);
	mColorPalete[0].set(255, 255,   0, 255);
	mColorPalete[1].set(255,  0,  255, 255);
	mColorPalete[2].set(255,  0,  255,   0);
	mColorPalete[3].set(255, 255, 255,  64);	
	mColorFactor=0.0f;
	mColorSpeed= (1.0f/60.0f)*0.5f;
	mColorIndex=0;


}



GObjectPtr TGameStateDemo::CreatePlayerObject()
{
	

	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_PLAYER,0);
	GBox2dComponent::FixtureDefinitionsList fixtures;
	float cursorRadius = 0.3f;
	float collideRadius = 0.5f;
	float attractionForceRadius =  14.0f;


	b2Transform trans;
	trans.Set(b2Vec2(0.0f,0.0f),0.0f);

	

	//create the phyisics definition
	b2BodyDef bd;		
	bd.type =  b2_staticBody;
	bd.position = trans.p;
	bd.angle = trans.q.GetAngle();

	b2CircleShape* circleShape = new b2CircleShape();	
    b2FixtureDef fd;
	circleShape->m_radius=  cursorRadius;
	fd.shape = circleShape;
	fd.density = 0.10f;
	fd.friction =1.0f;
	fd.restitution = 0.2f;
	fixtures.push_back(fd);

	//create the scene node
	irr::video::SColor scolor = irr::video::SColor(255,255,255,255);
	
	
	
	SpriteNode* node = new SpriteNode(
				app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
				app->getIrrlichtDevice()->getSceneManager(),
				object->GetId());				
#if defined(_IRR_ANDROID_PLATEFORM_)
	node->Load("Figures.tga",128,128,0.7f*2.0f,0.7f*2.0f);
#else 
	       node->Load("IrrlichtSdcard/Figures.tga",128,128,1.4f,1.4f);
#endif
	node->setFrame(10);
	

	
	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);
	

	GBox2dComponent* bcomp = new GBox2dComponent();
	bcomp->SetOwner(object);
	bcomp->VInit(mWorld,bd,fixtures);
	object->AddComponent(bcomp);
	
	GIrrlichtComponent* ncomp = new GIrrlichtComponent();
	ncomp->SetOwner(object);
	ncomp->VInit(node);
	object->AddComponent(ncomp);
	

	//add the controller component
	//add the controller
	TPlayerController* zcont = new TPlayerController();
	
	GControllerComponent* ccomp = new GControllerComponent();
	ccomp->VInit(zcont);
	object->AddComponent(ccomp);
	ccomp->SetOwner(object);
	zcont->VInit(cursorRadius, collideRadius,attractionForceRadius);


	object->PostInit(); //post initialize all components 
	zcont->setInvincible(true);


	zcont->setApplyForce(false);


	return object;
}
GObjectPtr TGameStateDemo::CreateSynth(float32 radius, float32 theta)
{
	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_SYNTHETIZER,0);
	GBox2dComponent::FixtureDefinitionsList fixtures;


	b2Transform trans;
	trans.Set(b2Vec2(0.0f,0.0f),0.0f);

	//create the phyisics definition
	/*b2BodyDef bd;		
	bd.type =  b2_staticBody;
	bd.position = trans.p;
	bd.angle = trans.q.GetAngle();

	b2PolygonShape* shape = new (b2PolygonShape);	
    b2FixtureDef fd;
	shape->SetAsBox(0.5,0.5f);
	fd.shape = shape;
	fd.density = 0.10f;
	fd.friction = 0.045f;
	fd.restitution = 0.8f;
	fixtures.push_back(fd);
	*/
	//create the scene node
	irr::video::SColor scolor = irr::video::SColor(255,255,255,255);
	
	SpriteNode* node = new SpriteNode(
				app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
				app->getIrrlichtDevice()->getSceneManager(),
				object->GetId());				
#if defined(_IRR_ANDROID_PLATEFORM_)
	node->Load("Figures.tga",128,128,1.0f,1.0f);
	
#else
	node->Load("IrrlichtSdcard/Figures.tga",128,128,1.0f,1.0f);
#endif
	node->setFrame(3);
	
	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);
	
	//add the controller
	TSynthetizerController* zcont = new TSynthetizerController();
	GControllerComponent* ccomp = new GControllerComponent();
	ccomp->VInit(zcont);
	object->AddComponent(ccomp);
	ccomp->SetOwner(object);
	zcont->VInit(radius,theta);

	//add the box2d component
	/*GBox2dComponent* bcomp = new GBox2dComponent();
	bcomp->SetOwner(object);
	bcomp->VInit(mWorld,bd,fixtures);
	object->AddComponent(bcomp);
	*/


	GIrrlichtComponent* ncomp = new GIrrlichtComponent();
	ncomp->SetOwner(object);
	ncomp->VInit(node);
	object->AddComponent(ncomp);
	
	object->PostInit(); //post initialize all components 
	return object;
}


		

void TGameStateDemo::VUpdate(int deltaMs)
{
	
	if(!mPaused)
	{

		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* pcont = (TPlayerController*)ccomp->gameController;
		

		mElapsedTime = mElapsedTime + deltaMs;
		mShieldCreationTime = mShieldCreationTime+ deltaMs;
		mExplosionTime = mExplosionTime + deltaMs;


		float timeStep = 1.0f / 60.0f;
		bool doPhysics = true;
		

		//if player elements where processed then process them
		ProcessProcessedMolecules();


		
		//Process al objects that have been crashed
		int crashed = ProcessCrashedObjects();
		mDidCrash = ( crashed > 0);

		//this will set the didCrash flag
		ProcessFinishedBits();
		
		EnsurePlayerIsInCrashArea();

		
		if(doPhysics)
			mWorld->Step(timeStep, mWorldSettings.velocityIterations, mWorldSettings.positionIterations);
	
		
		EnsurePlayerIsInCrashArea();


		//do the game logic/rules
		DoLogicPostSimulation(deltaMs);


		//lets create a backup of player moles

		//update the goal manager to ensure a crasheable scene
		mGoalManager.update(deltaMs, 50); 

		
		/*calculate players position */
		#if T_USE_OFFSET
		if (mIsMouseDown)
		{
		
			GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TPlayerController* playerController = (TPlayerController*)ccomp->gameController;
			GBox2dComponent* bcomp = mPlayer->GetComponent<GBox2dComponent>(mPlayer->GetComponentIdByIndex(GC_BOX2D_IDX));
			if(bcomp)
			{
				MovementData md;				
				int pointCount = 1;
				b2Vec2 speed;
				speed.x = 0.0f;
				speed.y = 0.0f;

				mSmoothPoints.points[0] = playerController->mPlayerOffset;

				if(playerController->PositionQueuePop(md))
				{	
					mSmoothPoints.points[1]= b2Vec2(md.x, md.y);
					speed.x += abs(md.speedX);
					speed.y += abs(md.speedY);


					pointCount++;
				}

				if(playerController->PositionQueuePop(md))
				{				
					mSmoothPoints.points[2]= b2Vec2(md.x, md.y);
					speed.x += abs(md.speedX);
					speed.y += abs(md.speedY);

					pointCount++;
				}
				
				if(pointCount>=2)
				{
					b2Vec2 newTouch;
					b2Vec2 playerPos =  bcomp->mBody->GetTransform().p;
					b2Vec2 dif;
					
					speed.x = speed.x + 1.0f;
					speed.y = speed.y + 1.0f;
					

					if(pointCount==2)
					{
						//interpolate 2 points
						newTouch = mSmoothPoints.points[0] + mSmoothPoints.points[1];
						newTouch = newTouch * 0.5f;
						
						dif = newTouch - mSmoothPoints.points[0];						
						
						//dif = dif.Normalize();
						dif = dif * speed;


						//dif = dif * speed;						
						playerPos = playerPos + dif;

						

					}
					else if(pointCount==3)
					{
						speed.x = speed.x/2.0f + 1.0f;
						speed.y = speed.y/2.0f + 1.0f;
					

						//interpolate 3 points
						newTouch = mSmoothPoints.interpolate();						
						
						dif = newTouch - mSmoothPoints.points[0];
						
						//dif = dif.Normalize();
						dif = dif * speed;

						playerPos = playerPos + dif;

						
					}

					bcomp->mBody->SetTransform(playerPos, 0.0f);
					playerController->mPlayerOffset = newTouch;
				}
			}
		}
		#endif

		//update all the objects
		mPlayer->Update(deltaMs);
		
		DoLevelUpAnimation(deltaMs);

		

		if(mSquareShield)
			mSquareShield->Update(deltaMs);

		if(mBottomShield)
			mBottomShield->Update(deltaMs);


		//update elements
		for (irr::u32 i = 0; i < mElementList.size(); i++)
		{	
			mElementList[i]->Update(deltaMs);

		}

		//update shield objects
		for (irr::u32 i = 0; i < mShieldList.size(); i++)
		{	
			mShieldList[i]->Update(deltaMs);

			
		}
		for (irr::u32 i = 0; i < mStaticShieldList.size(); i++)
		{
			mStaticShieldList[i]->Update(deltaMs);
		}

		//update bit objects
		for (irr::u32 i = 0; i < mBitList.size(); i++)
		{	
			mBitList[i]->Update(deltaMs);

			
		}
		//update the fireworks
		for(int i=0; i<mFireworkPool.size(); i++)
		{
			mFireworkPool[i]->Update(deltaMs);
		}


		//update the creators
		for(int i=0; i<mCreatorsPool.size(); i++)
		{
			mCreatorsPool[i]->Update(deltaMs);
		}

		// update the colors
		RefreshColorBars(deltaMs);


		//update the energy bar
		mEnergyBar->Update(deltaMs);
		mRadiationBar->Update(deltaMs);

		
		//update the joins node
		{
			ISceneNode* node =  GGameApp::getInstance()->getIrrlichtDevice()->getSceneManager()->getSceneNodeFromId(mJointsNodeId);
			JointsNode* jn = (JointsNode*)node;

			if(jn) 
			{
				
				jn->setJointList(mWorld->GetJointList(),mWorld->GetJointCount());

				if( pcont->getMoleState()==MOLE_STATE_COLLECTING  ||
					pcont->getMoleState()==MOLE_STATE_PROCESSING)
				{
					if(pcont->mMoleCount>0)
					{
						irr::video::SColor colors[3];
						b2Vec2 points[3];
						GTransformComponent* trcomp = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
						int pointcount=0;
						bool close=true;
						
						//get the player moles
						for (irr::u32 j = 0; j < pcont->mMoleCount; j++)
						{
							Mole playerMole;
							pcont->mMoles[j].populateMole(&playerMole, mElementList);


							for (irr::u32 i = 0; i < playerMole.count; i++)
							{

								GObjectPtr element =  playerMole.atoms[i];
								GTransformComponent* trans  = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));
								GSpriteComponent* spcompb = element->GetComponent<GSpriteComponent>(element->GetComponentIdByIndex(GC_SPRITE_IDX));						
								if(i==0)
								{
									points[j]=trans->mTrans.p;									
									colors[j]=spcompb->mColor;		
									pointcount++;
								}
								
							}
						}
						
						if(pcont->mMoleCount==1 && pcont->getMoleState()==MOLE_STATE_COLLECTING)
						{
							points[1]=trcomp->mTrans.p;									
							colors[1]=irr::video::SColor(255,255,255,255);
							pointcount++;
							close=false;
						}
						else if(pcont->mMoleCount==2 && pcont->getMoleState()==MOLE_STATE_COLLECTING)
						{
							points[2]=trcomp->mTrans.p;									
							colors[2]=irr::video::SColor(255,255,255,255);
							pointcount++;
							close=false;
						}

						jn->setPoints(points,colors,pointcount,close);
					}
					else
					{
						jn->setPoints(NULL,NULL,0,false);
					}
				}
				else
				{
					jn->setPoints(NULL,NULL,0,false);

				}
				
			}
		}

		AnimatePalete();

		//update the star background
		//UpdateStarBackground(deltaMs);

		
		if(doPhysics)
			mSpringGrid->Update(deltaMs);
		
		//update the score
				 
		if(pcont->mLevelChanged )
		{
			pcont->mLevelChanged = false;

			if(mGameMode == 1)
			{
				GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Tutorial"); 
			}
			else if(mGameMode >= 2)
			{
				switch(pcont->mLevel)
				{
					case 1:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level A-1"); 
						break;
					case 2:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level A-2"); 
						break;
					case 3:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level A-3"); 
						break;
					case 4:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level A-4"); 
						break;
					case 5:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level A-5"); 
						break;
					case 6:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level B-1"); 
						break;
					case 7:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level B-2"); 
						break;
					case 8:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level B-3"); 
						break;
					case 9:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level B-4"); 
						break;
					case 10:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level C-1"); 
						break;
					case 11:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level C-2"); 
						break;
					case 12:
						GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level C-3"); 
						break;
					default:
						{						
							if(pcont->mLevel>12)
							{
								GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level C-X"); 
							}
							else
							{
								GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","level_div","Level ?"); 
							}
						}
						break;
				}
			}
		}

		if(pcont->mScoreChanged)
		{
			pcont->mScoreChanged = false;
			GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","score",pcont->mScore); 
			
			
		}

		/*if(pcont->mScoreChanged)
		{
			pcont->mScoreChanged =false;
			GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","waves", );
		}*/


		//GGameApp::getInstance()->getRocketGUI()->SetElementInnerText("game_window","waves",
		//GGameApp::getInstance()->getIrrlichtDevice()->getVideoDriver()->getFPS());

		mDidCrash=false;
		mDidJoin = false;

		mFrameCount++;
	}

	//show the fps in the GUI
	
	

}

void TGameStateDemo::ProcessFinishedBits()
{

	irr::core::array<GObjectPtr> toRemove;
	toRemove.clear();
	bool hasCrash = false;

	for (irr::u32 i = 0; i < mBitList.size(); i++)
	{
		GObjectPtr bit =  mBitList[i];
		GControllerComponent* ccomp = bit->GetComponent<GControllerComponent>(bit->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TBitController* econt = (TBitController*)ccomp->gameController;
		
		if(econt->mState  == BIT_STATE_FINISHED)
		{
	
			toRemove.push_back(bit);
		}
	}


	for (irr::u32 j = 0; j < toRemove.size(); j++)
	{
		for (irr::u32 i = 0; i < mBitList.size(); i++)
		{
			if (mBitList[i] == toRemove[j])
			{				
				mBitList.erase(i);
				break;
			}
		}

		toRemove[j]->Destroy();
		delete toRemove[j];
	}

}

void  TGameStateDemo::ProcessProcessedMolecules()
{
	if(mPlayer)
	{
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* playerController = (TPlayerController*)ccomp->gameController;
		//playerController->setApplyForce(false);

		int toAdd=0;
		bool didSomething = false;
		

		if(!(playerController->getMoleState() == MOLE_STATE_PROCESSED))
			return;

		playerController->setApplyForce(false);

		irr::os::Printer::log("Processing processed");

		irr::os::Printer::log("setMoleState MOLE_STATE_NONE");
		int totalMoles = playerController->mMoleCount;

		playerController->setMoleState(MOLE_STATE_NONE);
		playerController->clearMoles();

	
		if (playerController->getCrashState() == CRASH_STATE_CANJOIN  && playerController->getCollideOnStopForce())		
		{
			toAdd = 0;			
			for (irr::u32 i = 0; i < mElementList.size(); i++)
			{
				GObjectPtr element =  mElementList[i];

				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;
				econt->setInAttraction(false);
				if(econt->getCrashState() == CRASH_STATE_CANCRASH)
				{
					toAdd++;
				}
			}
		}



		//if the player can crash
		if(   (playerController->getCrashState() == CRASH_STATE_CANCRASH) 
			&& playerController->getCollideOnStopForce())
		{
			didSomething=true;
			
			GTransformComponent* tcomp = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		
			
			TSpringGridComponent* sgcomp = mSpringGrid->GetComponent<TSpringGridComponent>(mSpringGrid->GetComponentIdByIndex(T_GC_SPRINGRID_IDX));
			if(sgcomp && sgcomp->mSpringGrid)
			{
				mExplosionTime = 0; 
				sgcomp->mSpringGrid->mPinchForces[1].mForcePosition.x  = tcomp->mTrans.p.x;
				sgcomp->mSpringGrid->mPinchForces[1].mForcePosition.y =  tcomp->mTrans.p.y; 
				sgcomp->mSpringGrid->mPinchForces[1].mForceEnable=true;

			}
	
			int multiplier  = 1;

			switch(playerController->mGoodCount)
			{
				case 2:
					multiplier = 1;
					break;
				case 3:
				case 4:
					multiplier = 2;				
					break;
				case 5:
				case 6:
					multiplier = 3;				
					break;
				default:
					multiplier = 4;
					break;
			}

			float energyPerObject = (float)playerController->mGoodCount * 1.5f;
				
			//playerController->addEnergy((float)playerController->mGoodCount* 3.0f);
			playerController->mCrashCount += playerController->mGoodCount;


			
			
			playerController->setLevelCrashes( playerController->mLevelCrashes + playerController->mGoodCount);
			
			
			
			

			int passCount = 20;
			int level = playerController->mLevel;

			if(level > 15)
				passCount = 50;
			else if(level > 12)
				passCount = 40;
			else if(level > 10)
				passCount = 30;
			else if(level > 7)
				passCount = 30;
			else if(level > 3)
				passCount = 25;
			else
				passCount = 20;
			
			

		

			int crashedElements = playerController->mGoodCount;

			playerController->mGoodCount = 0;
			playerController->mDifferentColors = 0;
			
			bool hasCrash = false;
			bool isFirst = true;
			//mark all crashing objects as crashed

			playerController->setCollideOnStopForce(false);
			//mark objects that are ready to crash
			for (irr::u32 i = 0; i < mElementList.size(); i++)
			{
				GObjectPtr element =  mElementList[i];

				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;
				econt->setInAttraction(false);

				if(econt->getCrashState() == CRASH_STATE_CANCRASH)
				{
					econt->setCrashState(CRASH_STATE_CRASHED);
					hasCrash = true;

					if(econt->getElementType() == T_OS_ELEMENT_ENERGY)
					{
						GTransformComponent* econtcomp = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		

							//create energy bit
						//mBitList.push_back(TElementsFactory::CreateBitElement(mPlayer, element, mEnergyBar, energyPerObject,econt->mColor));
						playerController->addEnergy(energyPerObject);

						//create the explosions

						//create the explosion of player 
						CreateExplosionAt(econtcomp->mTrans.p,isFirst,crashedElements>=6?1:0,econt->mColor.color);
						isFirst = false;


					}
					else if(econt->getElementType() == T_OS_ELEMENT_SHIELD)
					{
							//look for a shield that needs power

						//update shield objects
						for (irr::u32 j = 0; j < mShieldList.size(); j++)
						{	
							GObjectPtr shield = mShieldList[j];
							//get the shield controller

							//get the shield controller
							GControllerComponent* shieldComp= shield->GetComponent<GControllerComponent>(shield->GetComponentIdByIndex(GC_CONTROLLER_IDX));
							TShieldController* shieldCont = (TShieldController*)shieldComp->gameController;
				
							if(shieldCont->mState == SHIELD_STATE_INACTIVE
								&& 
								shieldCont->mFixers<1)
							{
								shieldCont->mFixers++;
								//mBitList.push_back(TElementsFactory::CreateBitElement(mPlayer, element, shield, 0.0f, irr::video::SColor(255,255,32,255)));
								break; //exit for
							}

						}
							//create energy bit
							
					}


				}
			}

			if(hasCrash)
			{
				//mDidCrash = true;
				int points;
				
				bool isTargetCrash = false;
				
				mGoalManager.validateCrashIsObjective(totalMoles,points);				
		
				if(mLevelValues.moleSize ==3)
				{
					isTargetCrash = (mMolesC3 >= 2);
				}
				else
				if(mLevelValues.moleSize ==2)
				{
					if(mMolesC3 >=2)
					{
						isTargetCrash = true;

					}
					else if(mMolesC3 >=1)
					{
						isTargetCrash = mMolesC2 >= 1;
					}
					else
					{
						isTargetCrash = mMolesC2 >= 2;
					}
					
				}
				else
				if(mLevelValues.moleSize ==1)
				{
					isTargetCrash = true;
						
				}





				if(isTargetCrash)
				{
					int crashColors[5];
					crashColors[0] = 0;
					crashColors[1] = 0;
					crashColors[2] = 0;
					crashColors[3] = 0;
					crashColors[4] = 0;
					
					for (irr::u32 i = 0; i < mElementList.size(); i++)
					{
						GObjectPtr element =  mElementList[i];
						GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						TElementController* econt = (TElementController*)ccomp->gameController;
						GTransformComponent* tcomp = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));

						if(econt->getCrashState() == CRASH_STATE_CRASHED)
						{
							int color = econt->mColorId;
			
							if( (color>=0 && color<=4) )
							{
								if(crashColors[color]== -1)
									crashColors[color] =0;

								crashColors[color]= crashColors[color] + 1;													
							
								//if(crashColors[color]>=2)
								{
									float bitValue = 1.0f; 
								
									GIrrlichtComponent* barIrr = mColorBars[color]->GetComponent<GIrrlichtComponent>(mColorBars[color]->GetComponentIdByIndex(GC_IRRNODE_IDX));
									EnergyBarNode* energyNode = (EnergyBarNode*)barIrr->mNode;
	
									if(energyNode->getEnergyValue()<1.0f)
									{
										mBitList.push_back(
											TElementsFactory::CreateBitElement(
													mPlayer, 
													element, mColorBars[color],  
													bitValue,
													econt->mColor));
									}

								}
							}
						}
					}				
			
				}
			
				playerController->setScore( playerController->mScore + points);
			}


			
			if(mWaitingForCrash && hasCrash)
			{
				mWaitTime = 0;
			}

			
			
			
		}					
		else if( playerController->getCrashState() == CRASH_STATE_INVALIDCRASH)
		{
			for (irr::u32 i = 0; i < mElementList.size(); i++)
				{
					GObjectPtr element =  mElementList[i];

					GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					TElementController* econt = (TElementController*)ccomp->gameController;
					econt->setInAttraction(false);
					GBox2dComponent* bcomp = element->GetComponent<GBox2dComponent>(element->GetComponentIdByIndex(GC_BOX2D_IDX));
					
					if(econt->getHasPlayerContact() || econt->getHasContact())
					{
						bcomp->mBody->SetLinearVelocity(bcomp->mBody->GetLinearVelocity()*-0.05f);
					
					}
				}
		
		}
		else if (playerController->getCrashState() == CRASH_STATE_CANSWAP && playerController->getCollideOnStopForce())		
		{

			didSomething=true;
			GTransformComponent* tcomp = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
			playerController->mGoodCount = 0;
			playerController->mDifferentColors = 0;
			playerController->setCollideOnStopForce(false);


			//get the list of objects and swap the colors
			for (irr::u32 i = 0; i < mElementList.size(); i++)
			{
				GObjectPtr element =  mElementList[i];

				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;
				econt->setInAttraction(false);
				if(econt->getCrashState() == CRASH_STATE_CANCRASH)
				{
					econt->mColorId = econt->mNewColorId;
					econt->setCrashState(CRASH_STATE_NONE);

					GSpriteComponent* spcomp = element->GetComponent<GSpriteComponent>(element->GetComponentIdByIndex(GC_SPRITE_IDX));			

					//set color
					switch(econt->mColorId)
					{
						case 1:
								econt->mColor= irr::video::SColor(255,255,1,169);
						break;
						case 2:
								econt->mColor = irr::video::SColor(255,139,8,232);
						break;	
						case 3:
								econt->mColor = irr::video::SColor(255,10,51,255);
						break;	
						case 4:
								econt->mColor = irr::video::SColor(255,23,196,232);
						break;	
						default:
								econt->mColor = irr::video::SColor(255,18,255,131);
						break;
					}
					econt->mOriColor = econt->mColor;

					//set the color accordingly
				}
			}


		}
		else if (toAdd==2 && playerController->getCrashState() == CRASH_STATE_CANJOIN  && playerController->getCollideOnStopForce())		
		{
			didSomething=true;
			mDidJoin = true;

			//destroy elements and create new element
			GTransformComponent* tcomp = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
			playerController->mGoodCount = 0;
			playerController->mDifferentColors = 0;
			playerController->setCollideOnStopForce(false);

			int c1=0;
			int c2=0;
			GObjectPtr oA=NULL;
			GObjectPtr oB=NULL;

			int j=0;
			for (irr::u32 i = 0; i < mElementList.size(); i++)
			{
				GObjectPtr element =  mElementList[i];

				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;
				econt->setInAttraction(false);
				if(econt->getCrashState() == CRASH_STATE_CANCRASH)
				{
					econt->setCrashState(CRASH_STATE_CRASHED);						
					
					switch(j)
					{
						case 0:
							{
							c1 = econt->mColorId;
							oA= element;
							}
						break;
						case 1:
							{
							c2 = econt->mColorId;
							oB= element;
							}
						break;						
					}
					j++;
				}	
			}

			if(! (oA==oB))
			{		 
				GTransformComponent* transA = oA->GetComponent<GTransformComponent>(oA->GetComponentIdByIndex(GC_TRANSFORM_IDX));
				GTransformComponent* transB=  oB->GetComponent<GTransformComponent>(oB->GetComponentIdByIndex(GC_TRANSFORM_IDX));
				b2Vec2 distance = transA->mTrans.p - transB->mTrans.p;
				b2Vec2 pos  = transA->mTrans.p + transB->mTrans.p;
				pos = pos / 2.0f;
				float angle = b2Atan2(distance.y,distance.x);
				angle = angle /3.1416 * 180;
				this->CreateDuoNoSynth(pos.x*10.0f, pos.y*10.0,DUO_WIDTH, (int)angle,c1,c2);
			}

		}
		else if (toAdd==3 && playerController->getCrashState() == CRASH_STATE_CANJOIN  && playerController->getCollideOnStopForce())		
		{	didSomething=true;
			mDidJoin = true;
			//destroy elements and create new element
			GTransformComponent* tcomp = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
			playerController->mGoodCount = 0;
			playerController->mDifferentColors = 0;
			playerController->setCollideOnStopForce(false);

			
			int ca=0;
			int cb=0;
			int cc=0;
			GObjectPtr oA=NULL;
			GObjectPtr oB=NULL;
			GObjectPtr oC=NULL;

			int j=0;
			for (irr::u32 i = 0; i < mElementList.size(); i++)
			{
				GObjectPtr element =  mElementList[i];

				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;
				econt->setInAttraction(false);
				if(econt->getCrashState() == CRASH_STATE_CANCRASH)
				{
					econt->setCrashState(CRASH_STATE_CRASHED);						
					
					switch(j)
					{
						case 0:
							{
							ca = econt->mColorId;
							oA= element;
							}
						break;
						case 1:
							{
							cb = econt->mColorId;
							oB= element;
							}
						break;
						case 2:
							{
							cc = econt->mColorId;
							oC= element;
							}
						break;
					}
					j++;
				}	
			}			
			if(mAddType != 1)
			{	
				GTransformComponent* transA = oA->GetComponent<GTransformComponent>(oA->GetComponentIdByIndex(GC_TRANSFORM_IDX));
				GTransformComponent* transB=  oB->GetComponent<GTransformComponent>(oB->GetComponentIdByIndex(GC_TRANSFORM_IDX));
				GTransformComponent* transC=  oC->GetComponent<GTransformComponent>(oC->GetComponentIdByIndex(GC_TRANSFORM_IDX));

				b2Vec2 pos  = (transA->mTrans.p + transB->mTrans.p  + transC->mTrans.p) /3.0f;

				//create a unit vector from pos 
				b2Vec2 oVector = b2Vec2(pos.x+1.0f, pos.y);

				//get distances from  position
				b2Vec2 distanceA = transA->mTrans.p- pos;
				b2Vec2 distanceB = transB->mTrans.p- pos;
				b2Vec2 distanceC = transC->mTrans.p- pos;
					
				//get the angle between distance and 
				float angleA = b2Atan2(distanceA.y,distanceA.x);
				float angleB = b2Atan2(distanceB.y,distanceB.x);
				float angleC = b2Atan2(distanceC.y,distanceC.x);

				if (angleA < 0.0f ) angleA = 3.1416f*2.0f + angleA;
				if (angleB < 0.0f ) angleB = 3.1416f*2.0f + angleB;
				if (angleC < 0.0f ) angleC = 3.1416f*2.0f + angleC;

				int jj=0;
				jj++;
				float angle=0.0f;

				b2Vec2 d1;
				b2Vec2 d2;
				b2Vec2 distance12;
				int c1=0;
				int c2=0;
				int c3=0;
					
				//now sort by angles
				if (angleA <= angleB && angleA <= angleC) 
				{
					if (angleB <= angleC)   //a, b, c
					{	
					c1= ca;
					c2= cb;
					c3= cc;
					d1 = transB->mTrans.p;
					d2 = transC->mTrans.p;
					}
					else //a, c, b					  
					{
					c1= ca;
					c2= cc;
					c3= cb;
					d1 = transC->mTrans.p;
					d2 = transB->mTrans.p;
					}
				}
				else if (angleB <= angleA && angleB <= angleC) 
				{
					if (angleA <= angleC) // b, a, c
					{
					c1= cb;
					c2= ca;
					c3= cc;
					d1 = transA->mTrans.p;
					d2 = transC->mTrans.p;
					}
					else  //b, c, a
					{	
					c1= cb;
					c2= cc;
					c3= ca;
					d1 = transC->mTrans.p;
					d2 = transA->mTrans.p;
					}
				}
				else
				{
					if (angleA <= angleB) //c, a, b
					{
					c1= cc;
					c2= ca;
					c3= cb;
					d1 = transA->mTrans.p;
					d2 = transB->mTrans.p;
					}
					else  //c, b, a
					{
					c1= cc;
					c2= cb;
					c3= ca;
					d1 = transB->mTrans.p;
					d2 = transA->mTrans.p;
					}
				}
				distance12 = d1-d2;

				angle = b2Atan2(distance12.y,distance12.x);

				if (angle < 0.0f ) angle = 3.1416f*2.0f + angle;

				//to degrees
				angle = angle /3.1416 * 180;
				angle = angle + 30.0f;
				CreateTriNoSynth(pos.x*10.0f, pos.y*10.0,TRI_WIDTH,angle,c3,c1,c2);				
			
			}
			else
			{

				GTransformComponent* transA = oA->GetComponent<GTransformComponent>(oA->GetComponentIdByIndex(GC_TRANSFORM_IDX));
				GTransformComponent* transB=  oB->GetComponent<GTransformComponent>(oB->GetComponentIdByIndex(GC_TRANSFORM_IDX));
				GTransformComponent* transC=  oC->GetComponent<GTransformComponent>(oC->GetComponentIdByIndex(GC_TRANSFORM_IDX));
				b2Vec2 distanceAB = transA->mTrans.p - transB->mTrans.p;
				b2Vec2 distanceAC = transA->mTrans.p - transC->mTrans.p;
				b2Vec2 distanceBC = transB->mTrans.p - transC->mTrans.p;
				float lAB = distanceAB.LengthSquared();
				float lAC = distanceAC.LengthSquared();
				float lBC = distanceBC.LengthSquared();					
				float angle = 0.0f;
				b2Vec2 pos  = transA->mTrans.p + transB->mTrans.p  + transC->mTrans.p;
				pos = pos / 3.0f;


				if(lAB >= lAC && lAB >= lBC)
				{
					angle = b2Atan2(distanceAB.y,distanceAB.x);
					angle = angle /3.1416 * 180;
					this->CreateTri2NoSynth(pos.x*10.0f, pos.y*10.0,TRI2_WIDTH,angle,cb,cc,ca);
							

				}
				else if(lAC >= lAB && lAC > lBC)
				{ 
					angle = b2Atan2(distanceAC.y,distanceAC.x);
					angle = angle /3.1416 * 180;						
					this->CreateTri2NoSynth(pos.x*10.0f, pos.y*10.0,TRI2_WIDTH,angle,cc,cb,ca);
							
				}
				else
				{
					angle = b2Atan2(distanceBC.y,distanceBC.x);
					angle = angle /3.1416 * 180;
					this->CreateTri2NoSynth(pos.x*10.0f, pos.y*10.0,TRI2_WIDTH,angle,cc,ca,cb);
							
				}				
			}
		}
		else
		{
		
			for (irr::u32 i = 0; i < mElementList.size(); i++)
			{
				GObjectPtr element =  mElementList[i];
				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;
				econt->setInAttraction(false);
				
			}
		
		}
	
	}


	if(mWaitingForJoin&& mDidJoin)
	{
		mWaitTime = 0;
	}


}
int TGameStateDemo::ProcessCrashedObjects()
{
	irr::core::array<GObjectPtr> toRemove;
	toRemove.clear(); 
	bool hasCrash = false;
	int deletedObjects = 0;

	GControllerComponent* pcomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
	TPlayerController* pcont = (TPlayerController*)pcomp->gameController;


	for (irr::u32 i = 0; i < mElementList.size(); i++)
	{
		GObjectPtr element =  mElementList[i];
		GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TElementController* econt = (TElementController*)ccomp->gameController;
		GTransformComponent* tcomp = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));

		if(econt->getCrashState() == CRASH_STATE_CRASHED)
		{
	
			if(econt->mLifeCycle == ELEMENT_CYCLE_CREATING )
			{
				econt->FreeSynth();
			}

			toRemove.push_back(element);
		}
		else
		{   //check if it is out of bounds
		
			if(	tcomp->mTrans.p.x >   (CRASH_AREA_HALF_WIDTH  + 1.0f)
			||  tcomp->mTrans.p.x <  (-CRASH_AREA_HALF_WIDTH  - 1.0f )

			||  tcomp->mTrans.p.y >  ( CRASH_AREA_HALF_HEIGHT + 18.0f - 1.0f)
			||  tcomp->mTrans.p.y <  (-CRASH_AREA_HALF_HEIGHT - 0.5f - 1.0f))
			{

				if(econt->mLifeCycle == ELEMENT_CYCLE_CREATING )
				{
					econt->FreeSynth();
				}

				toRemove.push_back(element);

				if(   econt->getElementType() == T_OS_ELEMENT_ENERGY
					||econt->getElementType() == T_OS_ELEMENT_ENERGY2
					)			
				{
					pcont->addRadiation(8.0f*econt->mRadiationFactor);

				}
			}
			
			
		}



	}
	bool alreadyInMoles = false;


	for (irr::u32 k = 0; k < toRemove.size(); k++)
	{
		//before removing the object we must make sure that it is not in the player mole list
		if(		pcont->getMoleState() == MOLE_STATE_COLLECTING
			||  pcont->getMoleState() == MOLE_STATE_PROCESSING)
		{

			for (irr::u32 j = 0; j < pcont->mMoleCount; j++)
			{			
				Mole playerMole;
				pcont->mMoles[j].populateMole(&playerMole, mElementList);

				for (irr::u32 i = 0; i < playerMole.count; i++)
				{
					GObjectPtr existingAtom = playerMole.atoms[i];
					if (toRemove[k]->GetId() == existingAtom->GetId())
					{
						alreadyInMoles=true;
						break;
					}
				}

				if(alreadyInMoles)
					break;
			}
		}
	}


	
	if(	alreadyInMoles
		 &&
		 (   pcont->getMoleState() == MOLE_STATE_COLLECTING
		||  pcont->getMoleState() == MOLE_STATE_PROCESSING)
		)
	{
			//if there is leak then cancel everything
			irr::os::Printer::log("Cancelling when leak or crash");
			irr::os::Printer::log("setMoleState MOLE_STATE_PROCESSED");
			pcont->setMoleState(MOLE_STATE_PROCESSED);
			pcont->setCrashState(CRASH_STATE_NONE);
		
			for (irr::u32 i = 0; i < mElementList.size(); i++)
			{
				GObjectPtr element =  mElementList[i];

				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;
				econt->setCrashState(CRASH_STATE_NONE);
			}			
			ProcessProcessedMolecules();

	}


	for (irr::u32 k = 0; k < toRemove.size(); k++)
	{
		for (irr::u32 i = 0; i < mElementList.size(); i++)
		{
			if (mElementList[i] == toRemove[k])
			{				
				mElementList.erase(i);
				break;
			}
		}

		toRemove[k]->Destroy();
		delete toRemove[k];
		deletedObjects++;
	}

	return deletedObjects;
}

void TGameStateDemo::EnsurePlayerIsInCrashArea()
{
	GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
	TPlayerController* pcont = (TPlayerController*)ccomp->gameController;
	GTransformComponent* tcomp = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
	GBox2dComponent* bcomp = mPlayer->GetComponent<GBox2dComponent>(mPlayer->GetComponentIdByIndex(GC_BOX2D_IDX));
	
	bool needsCorrection=false;
	b2Vec2 corrected = tcomp->mTrans.p;

	if(	corrected.x >   CRASH_AREA_HALF_WIDTH)
	{
		needsCorrection=true;
		corrected.x = CRASH_AREA_HALF_WIDTH- 0.01f;

	}
	if(	corrected.x <  -CRASH_AREA_HALF_WIDTH )
	{
		needsCorrection=true;
		corrected.x =  -CRASH_AREA_HALF_WIDTH+ 0.01f;
		
	}
	if(	corrected.y >  (CRASH_AREA_HALF_HEIGHT - 1.0f))
	{		
		needsCorrection=true;
		corrected.y =  (CRASH_AREA_HALF_HEIGHT - 1.0f)-0.01f;
	}
	if(	corrected.y <   (-CRASH_AREA_HALF_HEIGHT - 1.0f))
	{
		needsCorrection=true;
		corrected.y =  (-CRASH_AREA_HALF_HEIGHT - 1.0f)+0.01f;
	}
		
	if(needsCorrection)
	{
		tcomp->mTrans.p= corrected;
		bcomp->mBody->SetTransform(corrected,0.0f);
	}


}
void TGameStateDemo::DoLogicPreSimulation(int deltaMs)
{
		GGameApp* app = GGameApp::getInstance();
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* pcont = (TPlayerController*)ccomp->gameController;
		GTransformComponent* tcomp = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		GBox2dComponent* bcomp = mPlayer->GetComponent<GBox2dComponent>(mPlayer->GetComponentIdByIndex(GC_BOX2D_IDX));
	

		for (irr::u32 i = 0; i < mElementList.size(); i++)
		{
			GObjectPtr element =  mElementList[i];

			GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TElementController* econt = (TElementController*)ccomp->gameController;
					
			econt->setHasPlayerContact(false);
			econt->setHasContact(false);
			econt->setIsTemporaryCrash(false);
			econt->setAllowAttraction(false);

		}

		//Test if there are any elements in the collide region
		//this will set the IsInForce flag to true
		
		//CanCollideQuery collideQuery(mPlayer,mWorld);
		//collideQuery.Execute();




}
void TGameStateDemo::AddSelectedIdsToPlayer()
{
	GObjectPtr chain3[3];
	chain3[0]=NULL;
	chain3[1]=NULL;
	chain3[2]=NULL;		
	int mc=1;

	for(int k=0; k<16;k++)
	{
		
		

		if(mSelectedIds[k]>0)
		{
			GObjectPtr element = NULL;

			for(int i=0;i<mElementList.size();i++)
			{				
				if(mSelectedIds[k]  == mElementList[i]->GetId())
				{
					element= mElementList[i];
					break;
				}
			}

			if(element!=NULL)
			{
			
				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;
				GBox2dComponent* box2d = element->GetComponent<GBox2dComponent>(element->GetComponentIdByIndex(GC_BOX2D_IDX));
	
					
				if(!econt->getHasContact())
				{
						
					//irr::os::Printer::log("Element has contact.");
					mc=1;	
					chain3[0]=element;
					chain3[1]=NULL;
					chain3[2]=NULL;		
	

						
					b2JointEdge* edge = box2d->mBody->GetJointList();					
					if(edge!=NULL) //this is at least a duo
					{	
								
						chain3[1]=(GObjectPtr)edge->other->GetUserData();
						mc = 2;

						edge = edge->next;											
						//this has 2 edges , then it is a trio
						if(edge)
						{
							chain3[2]=(GObjectPtr)edge->other->GetUserData();
							mc = 3;
						}
						else
						{
							GControllerComponent* ccomp2 = chain3[1]->GetComponent<GControllerComponent>(chain3[1]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
							GBox2dComponent* box2d2 = chain3[1]->GetComponent<GBox2dComponent>(chain3[1]->GetComponentIdByIndex(GC_BOX2D_IDX));
							TElementController* econt2 = (TElementController*)ccomp2->gameController;		
							b2JointEdge* edge2 = box2d2->mBody->GetJointList();
							//check if other body has edge then it is a trio
							if(edge2)
							{
								if(edge2->other->GetUserData() == element)
									edge2 = edge2->next;

								if(edge2)
								{
									chain3[2]=(GObjectPtr)edge2->other->GetUserData();
									mc = 3;
								}
							}
						}
					}
					
					Mole mole;
					mole.count=0;

					for(int i=0;i<mc;i++)
					{
						GControllerComponent* ccomp2 = chain3[i]->GetComponent<GControllerComponent>(chain3[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						TElementController* econt2 = (TElementController*)ccomp2->gameController;		
						mole.atoms[i]= chain3[i];
						mole.colors[i]=  econt2->mColorId;
						mole.count++;
					}
					//ad this to the player moles					
					GControllerComponent* pcomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					TPlayerController* playerController = (TPlayerController*)pcomp->gameController;		
					playerController->AddMole(&mole);

				}
			
			}
		
		}
	
	}

}
void TGameStateDemo::DoLogicPostSimulation(int deltaMs)
{
		GGameApp* app = GGameApp::getInstance();
		
		//get the player controller
		GControllerComponent* ccomp1 = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* playerController = (TPlayerController*)ccomp1->gameController;
		GTransformComponent* playerTransform = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		

	//	mCamera->setPosition(irr::core::vector3df(playerTransform->mTrans.p.x /2.5f,playerTransform->mTrans.p.y/2.5f,-9.0f));
	//	mCamera->setTarget(irr::core::vector3df(playerTransform->mTrans.p.x/4.0f,playerTransform->mTrans.p.y/4.0f,0.0f));



		//set the pinch force on the spring grid if player is applying force
		if(mSpringGrid && playerController  && playerTransform)
		{
			TSpringGridComponent* sgcomp = mSpringGrid->GetComponent<TSpringGridComponent>(mSpringGrid->GetComponentIdByIndex(T_GC_SPRINGRID_IDX));
			if(sgcomp && sgcomp->mSpringGrid)
			{
				

				sgcomp->mSpringGrid->mPinchForces[0].mForceEnable = 
						(playerController->getApplyForce()) || ((playerController->getMoleState() == MOLE_STATE_PROCESSING) && (playerController->mMoleCount > 1));
						
				sgcomp->mSpringGrid->mPinchForces[0].type  = 0;
				sgcomp->mSpringGrid->mPinchForces[0].mRadius = 4.0f;
				sgcomp->mSpringGrid->mPinchForces[0].mForceFactor = 2.0f;
				sgcomp->mSpringGrid->mPinchForces[0].mForcePosition.x  = playerTransform->mTrans.p.x;
				sgcomp->mSpringGrid->mPinchForces[0].mForcePosition.y =  playerTransform->mTrans.p.y; 
				sgcomp->mSpringGrid->mPinchForces[0].mForcePosition.z = 0.0f;
				
			}

			//apply 
			sgcomp->mSpringGrid->mPinchForces[1].type  = 1;
			sgcomp->mSpringGrid->mPinchForces[1].mRadius = 2.5f;
			sgcomp->mSpringGrid->mPinchForces[1].mForceFactor = 60.0f;			
			sgcomp->mSpringGrid->mPinchForces[1].mForcePosition.z = 0.0f;
		
			if(sgcomp->mSpringGrid->mPinchForces[1].mForceEnable)
				sgcomp->mSpringGrid->mPinchForces[1].mForceEnable = (mExplosionTime < 600);

				

		}


		
		int level= playerController->mLevel;

		int launchTime = 500;


		if(mWaiting)
		{
			launchTime = mWaitTime;
			/*
			if(mWaitingForClear)
			{
			

				int countA=0;
				int countB=0;
				int countC=0;
				int countT = 0;

				for (irr::u32 i = 0; i < mElementList.size(); i++)
				{
					GObjectPtr element =  mElementList[i];
					GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					TElementController* econt = (TElementController*)ccomp->gameController;
		
					if( econt->getElementType() == T_OS_ELEMENT_ENERGY)
					{	
						countA+= (econt->mColorId==0)?1:0;
						countB+= (econt->mColorId==1)?1:0;
						countC+= (econt->mColorId==2)?1:0;
					}
					else if( econt->getElementType() == T_OS_ELEMENT_SHIELD)
					{
						countT++;
					}
				}

				if(countA <= 1 && countB <= 1 && countC <= 1  && countT <= 2)
				{
					launchTime = 0;
				}
			}
			*/
		}


		if(mLaunchTime >= launchTime)
		{
			mLaunchTime=0;
			mWaiting=false;
			mWaitingForClear=false;
			mWaitingForCrash = false;
			mWaitingForTap = false;


			//call the lua script every sec
			if(mScript != NULL)
			{

				


				if(mScriptState == GScriptable::NONE)
				{
					app->getScriptManager()->RunScriptable(this);
				}

				if(mScriptState == GScriptable::RUNNING)
				{
					mScript->call("update",this);
					
				}

				if(mTutorialState == TUTORIAL_STATE_START)
				{
					mTutorialState = TUTORIAL_STATE_RUNNING;
				}
			}	

			

		}	


	
		
		if(mDidCrash || mDidJoin)
		{	
			mGoalManager.startMonitoringScene(mLevelValues);		

		}
		
		mLaunchTime = mLaunchTime+ deltaMs;
						
		// if the player is applying force then we must apply the force to all elements 
		// within the attraction radius
		int energyElementCount = 0;
		int molesNearPlayer = 0;


		if( playerController->getMoleState() == MOLE_STATE_COLLECTING)
		{
			for(int i=0;i<16;i++)
				mSelectedIds[i]=0;

			CanCollideQuery collideQuery(mPlayer,mWorld,mSelectedIds,16);
			collideQuery.Execute();

			AddSelectedIdsToPlayer();

			//ApplyForceQuery forceQuery(mPlayer,mWorld,0.001f);
			//forceQuery.Execute();

		}
		else
		if( playerController->getMoleState() == MOLE_STATE_PROCESSING)
		{
			//move all selected elements towards the play
		


			ApplyForceQuery forceQuery(mPlayer,mWorld,1.0f);
			forceQuery.Execute();

			
		
		}

		//clear crash/join state
		playerController->setCrashState(CRASH_STATE_NONE);
		
		for (irr::u32 j = 0; j < playerController->mMoleCount; j++)
		{
			Mole playerMole;
			playerController->mMoles[j].populateMole(&playerMole, mElementList);


			for (irr::u32 i = 0; i < playerMole.count; i++)
			{
				GObjectPtr element =  playerMole.atoms[i];
				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				GTransformComponent* etcomp = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));				
				TElementController* econt = (TElementController*)ccomp->gameController;					
				econt->setCrashState(CRASH_STATE_NONE);


				if (playerController->getMoleState() == MOLE_STATE_PROCESSING)
				{
					if(econt->getHasPlayerContact())
					{
						b2Vec2 dist = playerTransform->mTrans.p - etcomp->mTrans.p;

						if(dist.Length() < 0.8f)
						{
							molesNearPlayer++;

						}
					}					
				}
			}
		}


		//now get all the elements that have player contact and check if the are all near the crasher 
		//if(molesNearPlayer == playerController->mMoleCount)
		if(playerController->mMoleCount==0 && playerController->getMoleState() == MOLE_STATE_PROCESSING)
		{
				irr::os::Printer::log("CRASH_STATE_NONE");
				irr::os::Printer::log("setMoleState MOLE_STATE_PROCESSED");
				playerController->setCrashState(CRASH_STATE_NONE);
				playerController->setMoleState(MOLE_STATE_PROCESSED);
		}
		else 		
		if(playerController->mMoleCount==1)
		{
			if(molesNearPlayer == playerController->mMoleCount && playerController->getMoleState() == MOLE_STATE_PROCESSING)
			{
				irr::os::Printer::log("CRASH_STATE_NONE");
				irr::os::Printer::log("setMoleState MOLE_STATE_PROCESSED");
				playerController->setMoleState(MOLE_STATE_PROCESSED);
			}

		}
		else
		if(playerController->mMoleCount>1){

			//Test if there are any elements in the collide region again		
			CanCollideQuery collideQuery(mPlayer,mWorld,NULL,0);
			//collideQuery.Execute();		
			collideQuery.setMaxJoin(mLevelValues.maxJoin);
			collideQuery.setMinCrash(2);
			collideQuery.setMaxSwap(0);
		
			for (irr::u32 j = 0; j < playerController->mMoleCount; j++)
			{
				Mole playerMole;
				playerController->mMoles[j].populateMole(&playerMole, mElementList);


				for (irr::u32 i = 0; i <playerMole.count; i++)
				{
					GObjectPtr element =  playerMole.atoms[i];
					GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					GTransformComponent* etcomp = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));
					TElementController* econt = (TElementController*)ccomp->gameController;					

					econt->setInCrashForce(true);

					collideQuery.m_count++;

					switch(econt->getElementType())
					{
						case (int)T_OS_ELEMENT_ENERGY:
						case (int)T_OS_ELEMENT_ENERGY2:
							{
							++collideQuery.m_countEnergy;

								switch(econt->mColorId)
								{
									case 0:
									++collideQuery.m_countEnergy1;
									break;
									case 1:
									++collideQuery.m_countEnergy2;
									break;
									case 2:
									++collideQuery.m_countEnergy3; 
									break;
									case 3:
									++collideQuery.m_countEnergy4; 
									break;
									case 4:
									++collideQuery.m_countEnergy5; 
									break;
								}
							}
							break;
						case (int)T_OS_ELEMENT_POINTS:
							++collideQuery.m_countBox;
							break;
						case (int)T_OS_ELEMENT_SHIELD:
							++collideQuery.m_countTriangle;
							break;
						case (int)T_OS_ELEMENT_BAD1:
							++collideQuery.m_countCross;
							break;
					}
						
				}
			}



			playerController->mBadCount =collideQuery.m_countCross;
			playerController->mTotalCount   = collideQuery.m_count;


			if(collideQuery.m_count>0 && collideQuery.m_count<=18)
			{
						
				bool validCrash = false;
				bool isClosing = false;
				bool isOpening = false;
				int  isJoining = 0;
				int  isSwaping = 0;
				int molesC1=0;
				int molesC2=0;
				int molesC3=0;

				//mMoleCount = 0;
				if(collideQuery.m_count >=2)
				{ 
					Mole playerMoles[3];
					playerMoles[0].clear();
					playerMoles[1].clear();
					playerMoles[2].clear();

					for(int j=0;j<playerController->mMoleCount;j++)
					{
						playerController->mMoles[j].populateMole(&playerMoles[j], mElementList);
					}					
				
					int objectCount =  playerController->mMoleCount; //RefreshCrasheableMoles();
				
			
					if(objectCount>=1 && objectCount<=3)
					{
						//mMoleCount = objectCount;

						//if(playerController->getApplyForce())
						{
							
							//we must have at least 2 moles to crash
							if(objectCount ==2 || objectCount ==3)
							{
							
								validCrash = collideQuery.IsCrashing(playerMoles,objectCount,molesC1,molesC2,molesC3);

							}


						
							if(!validCrash && (!isClosing && !isOpening))
							{	

								//we must have exactly 2 moles to crash
								if(objectCount ==2 || objectCount ==3)
									isJoining= collideQuery.IsJoining(playerMoles, objectCount);

									
							}

						}
					}
					
			
					if(validCrash)
					{
						playerController->mGoodCount = collideQuery.m_countEnergy;
						playerController->mDifferentColors =(collideQuery.m_countEnergy1>0?1:0)+
															(collideQuery.m_countEnergy2>0?1:0)+
															(collideQuery.m_countEnergy3>0?1:0)+
															(collideQuery.m_countEnergy4>0?1:0);

						playerController->mBestColor =1;				
						if(collideQuery.m_countEnergy3 >0 )
							playerController->mBestColor =3;
						if(collideQuery.m_countEnergy2 >0 )
							playerController->mBestColor =2;

						playerController->setCrashState(CRASH_STATE_CANCRASH);

						
						if(molesNearPlayer == playerController->mMoleCount && playerController->getMoleState() == MOLE_STATE_PROCESSING)
						{
							irr::os::Printer::log("CRASH_STATE_CANCRASH");
							irr::os::Printer::log("setMoleState MOLE_STATE_PROCESSED");
							playerController->setMoleState(MOLE_STATE_PROCESSED);
						}

						mMolesC1=molesC1;
						mMolesC2=molesC2;
						mMolesC3=molesC3;




						//if player is applying force then the next
						//time he releases the mouse the elements will crash
						//if(playerController->getApplyForce())
						{
							playerController->setCollideOnStopForce(true);
							int contactCount = 0;

							//count contacts
							for (irr::u32 i = 0; i < objectCount; i++)
							{
								for (irr::u32 j = 0; j < playerController->mMoles[i].count; j++)
								{

									GObjectPtr element =  playerMoles[i].atoms[j];

									GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
									TElementController* econt = (TElementController*)ccomp->gameController;
					
									if(econt->getInCrashForce())
									{
										contactCount ++;
							
									}
								}
							}

							//we must have at least one object in contact inside the collide radius
							//to enable crashing
							if(contactCount>=1)
							{

								for (irr::u32 i = 0; i < objectCount; i++)
								{
									for (irr::u32 j = 0; j < playerController->mMoles[i].count; j++)
									{

										GObjectPtr element =  playerMoles[i].atoms[j];


										GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
										TElementController* econt = (TElementController*)ccomp->gameController;
					
										if(econt->getInCrashForce() && econt->getIsTemporaryCrash())
										{
											econt->setCrashOperation(0);
											econt->setCrashState(CRASH_STATE_CANCRASH);	
										}
									}
								}
							}
						}
					}
					else
					{
						if(isJoining>0  && collideQuery.m_countCross == 0)
						{
				
							//if(playerController->getApplyForce())
							{	
								playerController->setCollideOnStopForce(true);
								int contactCount = 0;

								//count contacts
								for (irr::u32 i = 0; i < objectCount; i++)
								{
									for (irr::u32 j = 0; j < playerController->mMoles[i].count; j++)
									{

										GObjectPtr element =  playerMoles[i].atoms[j];

										GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
										TElementController* econt = (TElementController*)ccomp->gameController;
					
										if(econt->getInCrashForce())
										{
											contactCount ++;
							
										}
									}
								}	


								//we must have at least one object in contact inside the collide radius
								//to enable crashing
								if(contactCount>=1)
								{		
									playerController->setCrashState(CRASH_STATE_CANJOIN);					

									
										if(molesNearPlayer == playerController->mMoleCount && playerController->getMoleState() == MOLE_STATE_PROCESSING)
										{
											irr::os::Printer::log("CRASH_STATE_CANJOIN");
											irr::os::Printer::log("setMoleState MOLE_STATE_PROCESSED");
											playerController->setMoleState(MOLE_STATE_PROCESSED);
										}
		
										
									mAddType = isJoining;

									for (irr::u32 i = 0; i < objectCount; i++)
									{
										for (irr::u32 j = 0; j < playerController->mMoles[i].count; j++)
										{
											GObjectPtr element =  playerMoles[i].atoms[j];
											
											GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
											TElementController* econt = (TElementController*)ccomp->gameController;
					
										
											if(econt->getInCrashForce() )
											{
												econt->setCrashOperation(1);
												econt->setCrashState(CRASH_STATE_CANCRASH);	
											}

										}
									}
								}
							}
						}							
						else
						{
							
								if(molesNearPlayer == playerController->mMoleCount && playerController->getMoleState() == MOLE_STATE_PROCESSING)
								{
									irr::os::Printer::log("CRASH_STATE_NONE/INVALID");
									irr::os::Printer::log("setMoleState MOLE_STATE_PROCESSED");

									playerController->setMoleState(MOLE_STATE_PROCESSED);
								}


						
							/*if(playerController->getApplyForce())
							{
									
								playerController->setCrashState(CRASH_STATE_INVALIDCRASH);
							}
							else*/
							{								
								playerController->setCrashState(CRASH_STATE_NONE);
							}
						}
					
					}
				}
				else
				{
					
					if(molesNearPlayer == playerController->mMoleCount && playerController->getMoleState() == MOLE_STATE_PROCESSING)
					{
						irr::os::Printer::log("CRASH_STATE_NONE/INVALID");
						irr::os::Printer::log("setMoleState MOLE_STATE_PROCESSED");

						playerController->setMoleState(MOLE_STATE_PROCESSED);
					}


					

					/*if(playerController->getApplyForce())
								playerController->setCrashState(CRASH_STATE_INVALIDCRASH);
							else
							*/
					playerController->setCrashState(CRASH_STATE_NONE);
				}
			}
			else
			{
				
				
				if(molesNearPlayer == playerController->mMoleCount && playerController->getMoleState() == MOLE_STATE_PROCESSING)
				{
					irr::os::Printer::log("CRASH_STATE_NONE/INVALID");
					irr::os::Printer::log("setMoleState MOLE_STATE_PROCESSED");


					playerController->setMoleState(MOLE_STATE_PROCESSED);
				}

				/*if(playerController->getApplyForce())
					playerController->setCrashState(CRASH_STATE_INVALIDCRASH);
				else*/
					playerController->setCrashState(CRASH_STATE_NONE);
			}
		}
		

		/*apply flock */
		for (irr::u32 i = 0; i < mElementList.size(); i++)
		{
			GObjectPtr element =  mElementList[i];
			GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TElementController* econt = (TElementController*)ccomp->gameController;					
			GTransformComponent* tcomp = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));
			GBox2dComponent* boxcomp  = element->GetComponent<GBox2dComponent>(element->GetComponentIdByIndex(GC_BOX2D_IDX));				
					

			b2Vec2 cohesion;
			b2Vec2 separation;
			b2Vec2 wallAvoid;
			b2Vec2 playerAvoid;
			int neighborCount = 0;
			
			cohesion.Set(0.0f,0.0f);
			separation.Set(0.0f,0.0f);
			wallAvoid.Set(0.0f,0.0f);
			playerAvoid.Set(0.0f,0.0f);
			int playerAvoidCount =0;
			if(!econt->getInAttraction())
			{

				if(element->GetSubType()!= T_OS_ELEMENT_BAD1)
				{
					for (irr::u32 j = 0; j < mElementList.size(); j++)
					{
						if(i!=j)
						{
            				GObjectPtr element2 =  mElementList[j];
							GControllerComponent* ccomp2 = element2->GetComponent<GControllerComponent>(element2->GetComponentIdByIndex(GC_CONTROLLER_IDX));
							TElementController* econt2 = (TElementController*)ccomp2->gameController;					
							GTransformComponent* tcomp2 = element2->GetComponent<GTransformComponent>(element2->GetComponentIdByIndex(GC_TRANSFORM_IDX));
						
							b2Vec2 dist = tcomp2->mTrans.p - tcomp->mTrans.p;

							//check if it is a join

							b2JointEdge* edge = boxcomp->mBody->GetJointList();					
							bool isjoint = false;
							while(edge)
							{
								if(edge->other->GetUserData() == element2)
								{
									isjoint = true;
									break;
								}
								edge = edge->next;
							}

							if (dist.Length() < 2.0f && element2->GetSubType()!= T_OS_ELEMENT_BAD1  && !isjoint && !econt2->getInAttraction())
							{
								cohesion.x += tcomp2->mTrans.p.x;
								cohesion.y += tcomp2->mTrans.p.y;

								separation.x += tcomp2->mTrans.p.x - tcomp->mTrans.p.x;
								separation.y += tcomp2->mTrans.p.y - tcomp->mTrans.p.y;

								neighborCount++;										
							}
						}
					}

				}
								
				if(!playerController->getApplyForce())
				{
					b2Vec2 distPlayer = tcomp->mTrans.p - playerTransform->mTrans.p;
					if (distPlayer.Length() < 1.6f && element->GetSubType()!= T_OS_ELEMENT_BAD1  && !econt->getInAttraction())
					{
					
						playerAvoid.x += playerTransform->mTrans.p.x - tcomp->mTrans.p.x;
						playerAvoid.y += playerTransform->mTrans.p.y - tcomp->mTrans.p.y;
						playerAvoidCount++;											
					}
				}


				if (tcomp->mTrans.p.x < -(CRASH_AREA_HALF_WIDTH-0.5f))
				{
					wallAvoid.x = 1.0f;
				}
				if (tcomp->mTrans.p.x > (CRASH_AREA_HALF_WIDTH-0.5f))
				{
					wallAvoid.x = -1.0f;
				}
				if (
					(tcomp->mTrans.p.y < (-(CRASH_AREA_HALF_HEIGHT-0.5f) - 1.0f)) 					
				//	(tcomp->mTrans.p.y > (-(CRASH_AREA_HALF_HEIGHT)  - 1.0f))
					)
				{
				//	wallAvoid.y = 1.0f;
				}
				if (
					   tcomp->mTrans.p.y > (CRASH_AREA_HALF_HEIGHT-0.5f - 1.0f)
					&& tcomp->mTrans.p.y < (CRASH_AREA_HALF_HEIGHT- 1.0f)
					)
				{
					wallAvoid.y = -1.0f;
				}
				bool hastMove=false;

				if(neighborCount>0)
				{
					hastMove=true;
					
					separation.x = separation.x/(float)neighborCount;
					separation.y = separation.y/(float)neighborCount;
					separation.x *= -1.0f;
					separation.y *= -1.0f;
					separation.Normalize();

					cohesion.x = cohesion.x/(float)neighborCount;
					cohesion.y = cohesion.y/(float)neighborCount;
					cohesion.Set ( cohesion.x - tcomp->mTrans.p.x, cohesion.y - tcomp->mTrans.p.y);
					cohesion.Normalize();

					boxcomp->mBody->SetLinearVelocity(boxcomp->mBody->GetLinearVelocity()+cohesion*COHESION_FACTOR);
					boxcomp->mBody->SetLinearVelocity(boxcomp->mBody->GetLinearVelocity()+separation*SEPARATION_FACTOR);

						
				}
				if(playerAvoidCount>0)
				{
					playerAvoid.x = playerAvoid.x/(float)playerAvoidCount;
					playerAvoid.y = playerAvoid.y/(float)playerAvoidCount;
					playerAvoid.x *= -1.0f;
					playerAvoid.y *= -1.0f;
					playerAvoid.Normalize();
					boxcomp->mBody->SetLinearVelocity(boxcomp->mBody->GetLinearVelocity()+playerAvoid*PLAYER_AVOID_FACTOR);
					hastMove=true;
		
				}



				if(!hastMove)
				{
					if(boxcomp->mBody->GetLinearVelocity().Length()>0.05)
					{
						boxcomp->mBody->SetLinearVelocity(boxcomp->mBody->GetLinearVelocity()*SLOW_DOWN_FACTOR);
					}
				}
				boxcomp->mBody->SetLinearVelocity(boxcomp->mBody->GetLinearVelocity()+wallAvoid*WALLAVOID_FACTOR);
			}

		}

		//player is dead reset
		if( playerController->mRadiation> 1.0f )
		{	
			AudioItem audioUp;
			audioUp.mChunkId = AudioManager::gChunkIndexes[AudioItem::CHUNK_LOST];
			audioUp.mFlags = AudioItem::PLAYONCE;
			audioUp.mLoop=0;				
			audioUp.mId = GGameApp::getInstance()->getAudioManager()->playSound(audioUp);

			//explode everything

			CreateExplosionAt(playerTransform->mTrans.p,true,1);
			for (irr::u32 i = 0; i < mElementList.size(); i++)
			{
				GObjectPtr element =  mElementList[i];

				GTransformComponent* ctcmp = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));
				GControllerComponent* ctccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* ctecont = (TElementController*)ctccomp->gameController;
		

				if(ctcmp)
				{						
					CreateExplosionAt(ctcmp->mTrans.p,false,0,ctecont->mColor.color);
				}
			}
	


			//set game mode finished
			ResetGame(1);			
			mScript->run_chunk("level=200");

			

			mGameMode = 0;
			
			//goto main menu
			//app->mGameManager->VSetNextGameState(GAME_STATE_MAINMENU,0);
			
		}

}

void TGameStateDemo::ResetGame(int level)
{
	GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
	TPlayerController* playerController = (TPlayerController*)ccomp->gameController;
	GTransformComponent* playerTransform = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
	

	


	for(int i=0; i<mCreatorsPool.size(); i++)
	{
		GControllerComponent* gcontcreator =  mCreatorsPool[i]->GetComponent<GControllerComponent>(mCreatorsPool[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TElementCreatorController* creator =  (TElementCreatorController*)gcontcreator->gameController ;		
		creator->setState(CREATOR_STATE_FINISHED);
	}

	playerController->mEnergy = 1.0f;
	playerController->mRadiation = 0.0f;
	playerController->setLevel(level);
	playerController->mCrashCount = 0;
	playerController->setScore(0);
	playerController->mCountA=0;
	playerController->mCountB=0;
	playerController->mCountC=0;
	playerController->mCountD=0;
	playerController->mCountE=0;

	playerController->mElementsInArea = 0;
	playerController->mBestColor = 0;
	playerController->clearMoles();
	playerController->setCrashState(CRASH_STATE_NONE);
	playerController->setMoleState(MOLE_STATE_NONE);


	ClearElements();
	ClearBits();
	//ClearShield();
	mGoalManager.startMonitoringScene(mLevelValues);

	mElapsedTime=0;
	mFrameCount = 0;
	mShieldCount=0;
	mShieldCreationTime = (SHIELD_SIZE_TIME);
	mExplosionTime=0;


	mScript->run_chunk("lastlevel=0");


}

GObjectPtr TGameStateDemo::GetAvailableCreator()
{

	GObjectPtr creator = NULL;
	TElementCreatorController* creatorcont =NULL; 	

	for(int i=0; i<mCreatorsPool.size(); i++)
	{
		GObjectPtr o = mCreatorsPool[i];
		GControllerComponent* ccomp = o->GetComponent<GControllerComponent>(o->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		creatorcont = (TElementCreatorController*)ccomp->gameController;
	
		if(!creatorcont->getActive())
		{
			creator = o;			
			break;			
		}
	}

	return creator;
}


void TGameStateDemo::CreateElementAtXYSpeed( int x,int y, int angle, int speed,int element)
{
	
	GObjectPtr newElement = NULL;
	float fx = (float)x/10.0f;
	float fy = (float)y/10.0f;
	float sp = (float)speed/10.0f;
	float fangle = (float)angle *3.1416f /180.0f;

	
	int colorId = (element&0x0F);
	if(colorId == 0x0F) colorId = -1;
	
	int elementType = (element>>4)&0x0F;
	
	if(elementType == 0x0F) elementType = -1;

	if(elementType==-1)
	{
		elementType = std::rand()%4;
	}

	if(colorId == -1)
	{
		colorId= std::rand()%4;
	}

		if(elementType==0)
	{
		elementType = T_OS_ELEMENT_ENERGY;
	}
	else
	if(elementType==1)
	{
		elementType = T_OS_ELEMENT_ENERGY2;
	}
	else
	if(elementType==2)
	{
		elementType = T_OS_ELEMENT_SHIELD;
		colorId = 0;
	}
	else
	if(elementType==3)
	{
		colorId = 0;
		elementType = T_OS_ELEMENT_POINTS;
	}

	if(elementType>0)
	{
			GObjectPtr creator = GetAvailableCreator();

			if(creator!=NULL)
			{

				TElementCreatorController* creatorcont =NULL; 	
				GControllerComponent* ccomp = creator->GetComponent<GControllerComponent>(creator->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				creatorcont = (TElementCreatorController*)ccomp->gameController;
				creatorcont->Start(b2Vec2(fx,fy), sp, fangle, mSpringGrid);
		
				if(elementType==T_OS_ELEMENT_ENERGY) //create energy element
				{
					newElement = TElementsFactory::CreateEnergyElement(mWorld,creator,0.0f,0.0f, sp, colorId);
				}	
				else if(elementType==T_OS_ELEMENT_ENERGY2) //create shield element
				{
	
					newElement = TElementsFactory::CreateEnergyHeavyElement(mWorld,creator, sp, colorId);
				}
				else if(elementType==T_OS_ELEMENT_SHIELD) //create shield element
				{
	
					newElement = TElementsFactory::CreateShieldElement(mWorld,creator, sp);
				}
				else if(elementType==T_OS_ELEMENT_POINTS) //create shield element
				{
	
					newElement = TElementsFactory::CreatePointsElement(mWorld,creator, sp);
				}

				if(newElement)
				{

						mElementList.push_back(newElement);
				}
			}
	 }
}

void TGameStateDemo::CreateElementAtXY(int x,int y,int element)
{
	float fx = (float)x/10.0f;
	float fy = (float)y/10.0f;
	
	bool sta = (element&0x100)==0x100;
	

	int colorId = (element&0x0F);
	if(colorId == 0x0F) colorId = -1;
	
	element = (element>>4)&0x0F;
	
	if(element == 0x0F) element = -1;
	
	
	CreateElementAtXYInternal(element,colorId, fx,fy,sta, false);
	
}


GObjectPtr TGameStateDemo::CreateElementAtXYInternal(int elementId, int color, float x,float y, bool isStatic, bool withSynth)
{
	int elementType = elementId;
	int colorId = color;
	GObjectPtr newElement = NULL;
	
	if(elementType==-1)
	{
		elementType = std::rand()%4;
	}

	if(colorId == -1)
	{
		colorId= std::rand()%5;
	}

	if(elementType==0)
	{
		elementType = T_OS_ELEMENT_ENERGY;
	}
	else
	if(elementType==1)
	{
		elementType = T_OS_ELEMENT_ENERGY2;
	}
	else
	if(elementType==2)
	{
		colorId = 0;
		elementType = T_OS_ELEMENT_SHIELD;
	}
	else
	if(elementType==3)
	{
		colorId = 0;
		elementType = T_OS_ELEMENT_POINTS;
	}
	else
	if(elementType==4)
	{
		colorId = 0;
		elementType = T_OS_ELEMENT_BAD1;
	}

	if(elementType>0 )
	{
			GObjectPtr creator = GetAvailableCreator();

			if(creator!=NULL && withSynth)
			{

				TElementCreatorController* creatorcont =NULL; 	
				GControllerComponent* ccomp = creator->GetComponent<GControllerComponent>(creator->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				creatorcont = (TElementCreatorController*)ccomp->gameController;
				creatorcont->Start(b2Vec2(x,y), 0.0f, 0.0f, mSpringGrid);
		
				if(elementType==T_OS_ELEMENT_ENERGY) //create energy element
				{
					newElement = TElementsFactory::CreateEnergyElement(mWorld,creator, 0.0f,0.0f, 0.0f, colorId, isStatic);
				}	
				else if(elementType==T_OS_ELEMENT_ENERGY2) //create shield element
				{
	
					newElement = TElementsFactory::CreateEnergyHeavyElement(mWorld,creator, 0.0f, colorId, isStatic);
				}
				else if(elementType==T_OS_ELEMENT_SHIELD) //create shield element
				{
	
					newElement = TElementsFactory::CreateShieldElement(mWorld,creator, 0.0f, isStatic);
				}
				else if(elementType==T_OS_ELEMENT_POINTS) //create shield element
				{
	
					newElement = TElementsFactory::CreatePointsElement(mWorld,creator, 0.0f, isStatic);
				}
				else if(elementType==T_OS_ELEMENT_BAD1) //create shield element
				{

					
					int badcount = 0;
					for (irr::u32 i = 0; i < mElementList.size(); i++)
					{
						if (mElementList[i]->GetSubType() == T_OS_ELEMENT_BAD1)
						{
							badcount++;
							
						}
					}
	
					if(badcount< mLevelValues.maxBad)
					{
						int createBadProb = rand()%10;

						if(createBadProb==1)
						{
							newElement = TElementsFactory::CreateCrossElement(mWorld,creator, 0.0f, isStatic);
						}
					}
				}

				if(newElement)
				{

						mElementList.push_back(newElement);
				}
			}
			else
			{
				
				if(!withSynth)
				{
					newElement = TElementsFactory::CreateEnergyElement(mWorld,NULL,x,y, 0.0f, colorId, isStatic);
					if(newElement)
					{

							mElementList.push_back(newElement);
					}
				}
			
			}
	 }

	return newElement;
}
void TGameStateDemo::CreateDuo(int x, int y, int radius, int angle, int elementA,  int elementB)
{
	float fangle = (float)angle*3.1416f/180.0f;
	float fradius = (float)radius / 10.0f;
	const float fstep = (3.1416f*2.0f)/2.0f;
	float fx = (float)x/10.0f;
	float fy = (float)y/10.0f;

	b2Vec2 pa( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	fangle = fangle + fstep;
	b2Vec2 pb( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	bool sta = (elementA&0x100)==0x100;
	bool stb = (elementB&0x100)==0x100;
	
	int colorIdA = (elementA&0x0F);
	int colorIdB = (elementB&0x0F);
	if(colorIdA == 0x0F) colorIdA = -1;
	if(colorIdB == 0x0F) colorIdB = -1;
	
	elementA = (elementA>>4)&0x0F;
	elementB = (elementB>>4)&0x0F;
	
	if(elementA == 0x0F) elementA = -1;
	if(elementB == 0x0F) elementB = -1;
	

	if(colorIdA == -1 && colorIdB == -1)
	{
		int g = rand()%10;
		int color1 = TGoalManager::mColorCombinations2[g*2];
		int color2 = TGoalManager::mColorCombinations2[g*2+1];
	}

	GObjectPtr oa= CreateElementAtXYInternal(elementA,colorIdA,pa.x,pa.y,sta,false);
	GObjectPtr ob =CreateElementAtXYInternal(elementB,colorIdB,pb.x,pb.y,stb,false);
	
	if(oa!=NULL && ob!=NULL )
	{
		//create the joints
		b2DistanceJointDef jointDef; 
		jointDef.dampingRatio = 1.0;
		jointDef.frequencyHz = 10.0;

		GBox2dComponent* cA  = oa->GetComponent<GBox2dComponent>(oa->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cB  = ob->GetComponent<GBox2dComponent>(ob->GetComponentIdByIndex(GC_BOX2D_IDX));				
		
		b2Body* ba= cA->mBody;
		b2Body* bb= cB->mBody;
		
		
		jointDef.Initialize(ba, bb, pa,pb);
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);

	
	}

}


void TGameStateDemo::CreateTri(int x, int y, int radius, int angle, int elementA,  int elementB, int elementC)
{
	
	float fangle = (float)angle*3.1416f/180.0f;
	float fradius = (float)radius / 10.0f;
	const float fstep = (3.1416f*2.0f)/3.0f;
	float fx = (float)x/10.0f;
	float fy = (float)y/10.0f;

	b2Vec2 pa( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	fangle = fangle + fstep;
	b2Vec2 pb( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	fangle = fangle + fstep;
	b2Vec2 pc( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	bool sta = (elementA&0x100)==0x100;
	bool stb = (elementB&0x100)==0x100;
	bool stc = (elementC&0x100)==0x100;
	

	int colorIdA = (elementA&0x0F);
	int colorIdB = (elementB&0x0F);
	int colorIdC = (elementC&0x0F);
	if(colorIdA == 0x0F) colorIdA = -1;
	if(colorIdB == 0x0F) colorIdB = -1;
	if(colorIdC == 0x0F) colorIdC = -1;
	
	elementA = (elementA>>4)&0x0F;
	elementB = (elementB>>4)&0x0F;
	elementC = (elementC>>4)&0x0F;

	if(elementA == 0x0F) elementA = -1;
	if(elementB == 0x0F) elementB = -1;
	if(elementC == 0x0F) elementC = -1;
	
	if(colorIdA == -1 && colorIdB == -1 && colorIdC == -1)
	{

		int retries = 3;
		bool valid = false;

		while(!valid && retries >0)
		{

			valid = true;
			retries--;

			int maxPosibilities=1;

			switch(mLevelValues.maxColors)
			{
				case 3:
					maxPosibilities=1;
					break;
				case 4:
					maxPosibilities=4;
					break;
				case 5:
					maxPosibilities=10;
					break;
			}


			int g = rand()%maxPosibilities;
			int scramble = rand()%3;
			int oc1=0;
			int oc2=1;
			int oc3=2;
		

			switch(scramble)
			{
				case 0:
				{
					oc1=0;oc2=1,oc3=2;
				}
				break;
				case 1:
				{
					oc1=0;oc2=2,oc3=1;
				}
				break;
				case 2:
				{
					oc1=1;oc2=0,oc3=2;
				}			
				break;
		
		
			}

			colorIdA = TGoalManager::mColorCombinations3[g*3+oc1];
			colorIdB = TGoalManager::mColorCombinations3[g*3+oc2];
			colorIdC = TGoalManager::mColorCombinations3[g*3+oc3];


			//if trio exists chose other combination

			for(int i=0;i<mElementList.size();i++)
			{
			
				GControllerComponent* ccomp = mElementList[i]->GetComponent<GControllerComponent>(mElementList[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				GBox2dComponent* box2d = mElementList[i]->GetComponent<GBox2dComponent>(mElementList[i]->GetComponentIdByIndex(GC_BOX2D_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;		
				//get the box2d component
				b2JointEdge* edge = box2d->mBody->GetJointList();
				GObjectPtr chain3[3];
				chain3[0]=NULL;
				chain3[1]=NULL;
				chain3[2]=NULL;
				bool trio = false;

				if(edge!=NULL) //this is at leat a duo
				{	
					chain3[0]=mElementList[i];
					chain3[1]=(GObjectPtr)edge->other->GetUserData();
					edge = edge->next;

					//this has 2 edges , then it is a trio
					if(edge)
					{
						chain3[2]=(GObjectPtr)edge->other->GetUserData();
						trio=true;
					}
					else
					{
						GControllerComponent* ccomp2 = chain3[1]->GetComponent<GControllerComponent>(chain3[1]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						GBox2dComponent* box2d2 = chain3[1]->GetComponent<GBox2dComponent>(chain3[1]->GetComponentIdByIndex(GC_BOX2D_IDX));
						TElementController* econt2 = (TElementController*)ccomp2->gameController;		
						b2JointEdge* edge2 = box2d2->mBody->GetJointList();
						//check if other body has edge then it is a trio
						if(edge2)
						{
							if(edge2->other->GetUserData() == chain3[0])
								edge2 = edge2->next;

							if(edge2)
							{

								chain3[2]=(GObjectPtr)edge2->other->GetUserData();
								trio=true;
							}
						}
					}
				}

				if(trio)
				{
					//check colors
					int tcA=0;
					int tcB=0;
					int tcC=0;
					for(int i=0;i<3;i++)
					{
						GControllerComponent* ccomp2 = chain3[i]->GetComponent<GControllerComponent>(chain3[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						TElementController* econt2= (TElementController*)ccomp2->gameController;		

						if(colorIdA == econt2->mColorId)
							tcA++;
						if(colorIdB == econt2->mColorId)
							tcB++;
						if(colorIdC == econt2->mColorId)
							tcC++;
					}

					if(tcA ==1 && tcB==1 && tcC ==1)
					{
						valid = false;
						break; //for
					}
				}
			}
		}
	};

	


	GObjectPtr oa= CreateElementAtXYInternal(elementA,colorIdA,pa.x,pa.y,sta);
	GObjectPtr ob =CreateElementAtXYInternal(elementB,colorIdB,pb.x,pb.y,stb);
	GObjectPtr oc= CreateElementAtXYInternal(elementC,colorIdC,pc.x,pc.y,stc);
	
	if(oa!=NULL && ob!=NULL && oc!=NULL)
	{
		//create the joints
		b2DistanceJointDef jointDef; 
		  	jointDef.dampingRatio = 1.0f;
		jointDef.frequencyHz = 10.0;

		GBox2dComponent* cA  = oa->GetComponent<GBox2dComponent>(oa->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cB  = ob->GetComponent<GBox2dComponent>(ob->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cC  = oc->GetComponent<GBox2dComponent>(oc->GetComponentIdByIndex(GC_BOX2D_IDX));				
	
		b2Body* ba= cA->mBody;
		b2Body* bb= cB->mBody;
		b2Body* bc= cC->mBody;
	

		jointDef.Initialize(ba, bb, pa,pb);
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);

		jointDef.Initialize(ba, bc, pa,pc);
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);

		jointDef.Initialize(bb, bc, pb,pc);
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);
		
	}
	
}


void TGameStateDemo::CreateTriNoSynth(int x, int y, int radius, int angle, int elementA,  int elementB, int elementC)
{
	
	float finangle = (float)angle*3.1416f/180.0f;
	float fangle = 0.0f;
	float fradius = (float)radius / 10.0f;
	const float fstep = (3.1416f*2.0f)/3.0f;
	float fx = (float)x/10.0f;
	float fy = (float)y/10.0f;
	

	fangle  = finangle + fstep * 1.0f;
	b2Vec2 pa( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	fangle  = finangle + fstep * 2.0f;
	b2Vec2 pb( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	fangle  = finangle + fstep * 0.0f;
	b2Vec2 pc( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	bool sta = (elementA&0x100)==0x100;
	bool stb = (elementB&0x100)==0x100;
	bool stc = (elementC&0x100)==0x100;
	

	int colorIdA = (elementA&0x0F);
	int colorIdB = (elementB&0x0F);
	int colorIdC = (elementC&0x0F);
	if(colorIdA == 0x0F) colorIdA = -1;
	if(colorIdB == 0x0F) colorIdB = -1;
	if(colorIdC == 0x0F) colorIdC = -1;
	
	elementA = (elementA>>4)&0x0F;
	elementB = (elementB>>4)&0x0F;
	elementC = (elementC>>4)&0x0F;

	if(elementA == 0x0F) elementA = -1;
	if(elementB == 0x0F) elementB = -1;
	if(elementC == 0x0F) elementC = -1;
	
	if(colorIdA == -1 && colorIdB == -1 && colorIdC == -1)
	{

		int retries = 3;
		bool valid = false;

		while(!valid && retries >0)
		{

			valid = true;
			retries--;

			int maxPosibilities=1;

			switch(mLevelValues.maxColors)
			{
				case 3:
					maxPosibilities=1;
					break;
				case 4:
					maxPosibilities=4;
					break;
				case 5:
					maxPosibilities=10;
					break;
			}



			int g = rand()%maxPosibilities;
			int scramble = rand()%3;
			int oc1=0;
			int oc2=1;
			int oc3=2;
		

			switch(scramble)
			{
				case 0:
				{
					oc1=0;oc2=1,oc3=2;
				}
				break;
				case 1:
				{
					oc1=0;oc2=2,oc3=1;
				}
				break;
				case 2:
				{
					oc1=1;oc2=0,oc3=2;
				}			
				break;
		
		
			}

			colorIdA = TGoalManager::mColorCombinations3[g*3+oc1];
			colorIdB = TGoalManager::mColorCombinations3[g*3+oc2];
			colorIdC = TGoalManager::mColorCombinations3[g*3+oc3];


			//if trio exists chose other combination

			for(int i=0;i<mElementList.size();i++)
			{
			
				GControllerComponent* ccomp = mElementList[i]->GetComponent<GControllerComponent>(mElementList[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				GBox2dComponent* box2d = mElementList[i]->GetComponent<GBox2dComponent>(mElementList[i]->GetComponentIdByIndex(GC_BOX2D_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;		
				//get the box2d component
				b2JointEdge* edge = box2d->mBody->GetJointList();
				GObjectPtr chain3[3];
				chain3[0]=NULL;
				chain3[1]=NULL;
				chain3[2]=NULL;
				bool trio = false;

				if(edge!=NULL) //this is at leat a duo
				{	
					chain3[0]=mElementList[i];
					chain3[1]=(GObjectPtr)edge->other->GetUserData();
					edge = edge->next;

					//this has 2 edges , then it is a trio
					if(edge)
					{
						chain3[2]=(GObjectPtr)edge->other->GetUserData();
						trio=true;
					}
					else
					{
						GControllerComponent* ccomp2 = chain3[1]->GetComponent<GControllerComponent>(chain3[1]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						GBox2dComponent* box2d2 = chain3[1]->GetComponent<GBox2dComponent>(chain3[1]->GetComponentIdByIndex(GC_BOX2D_IDX));
						TElementController* econt2 = (TElementController*)ccomp2->gameController;		
						b2JointEdge* edge2 = box2d2->mBody->GetJointList();
						//check if other body has edge then it is a trio
						if(edge2)
						{
							if(edge2->other->GetUserData() == chain3[0])
								edge2 = edge2->next;

							if(edge2)
							{

								chain3[2]=(GObjectPtr)edge2->other->GetUserData();
								trio=true;
							}
						}
					}
				}

				if(trio)
				{
					//check colors
					int tcA=0;
					int tcB=0;
					int tcC=0;
					for(int i=0;i<3;i++)
					{
						GControllerComponent* ccomp2 = chain3[i]->GetComponent<GControllerComponent>(chain3[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						TElementController* econt2= (TElementController*)ccomp2->gameController;		

						if(colorIdA == econt2->mColorId)
							tcA++;
						if(colorIdB == econt2->mColorId)
							tcB++;
						if(colorIdC == econt2->mColorId)
							tcC++;
					}

					if(tcA ==1 && tcB==1 && tcC ==1)
					{
						valid = false;
						break; //for
					}
				}
			}
		}
	};

	


	GObjectPtr oa= CreateElementAtXYInternal(elementA,colorIdA,pa.x,pa.y,sta,false);
	GObjectPtr ob =CreateElementAtXYInternal(elementB,colorIdB,pb.x,pb.y,stb,false);
	GObjectPtr oc= CreateElementAtXYInternal(elementC,colorIdC,pc.x,pc.y,stc,false);
	
	if(oa!=NULL && ob!=NULL && oc!=NULL)
	{
		//create the joints
		b2DistanceJointDef jointDef; 
	 	jointDef.dampingRatio = 0.0;
		jointDef.frequencyHz = 10.0;
		jointDef.collideConnected = false;

		GBox2dComponent* cA  = oa->GetComponent<GBox2dComponent>(oa->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cB  = ob->GetComponent<GBox2dComponent>(ob->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cC  = oc->GetComponent<GBox2dComponent>(oc->GetComponentIdByIndex(GC_BOX2D_IDX));				
	
		b2Body* ba= cA->mBody;
		b2Body* bb= cB->mBody;
		b2Body* bc= cC->mBody;
	

		jointDef.Initialize(ba, bb, pa,pb);
		jointDef.collideConnected = false;
		mWorld->CreateJoint(&jointDef);

		jointDef.Initialize(ba, bc, pa,pc);
		jointDef.collideConnected = false;
		mWorld->CreateJoint(&jointDef);

		jointDef.Initialize(bb, bc, pb,pc);
		jointDef.collideConnected = false;
		mWorld->CreateJoint(&jointDef);
		
	}
	
}

void TGameStateDemo::CreateDuoNoSynth(int x, int y, int radius, int angle, int elementA,  int elementB)
{
	float fangle = (float)angle*3.1416f/180.0f;
	float fradius = (float)radius / 10.0f;
	const float fstep = (3.1416f*2.0f)/2.0f;
	float fx = (float)x/10.0f;
	float fy = (float)y/10.0f;

	b2Vec2 pa( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	fangle = fangle + fstep;
	b2Vec2 pb( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	bool sta = (elementA&0x100)==0x100;
	bool stb = (elementB&0x100)==0x100;
	
	int colorIdA = (elementA&0x0F);
	int colorIdB = (elementB&0x0F);
	if(colorIdA == 0x0F) colorIdA = -1;
	if(colorIdB == 0x0F) colorIdB = -1;
	
	elementA = (elementA>>4)&0x0F;
	elementB = (elementB>>4)&0x0F;
	
	if(elementA == 0x0F) elementA = -1;
	if(elementB == 0x0F) elementB = -1;
	

	if(colorIdA == -1 && colorIdB == -1)
	{
		int g = rand()%10;
		int color1 = TGoalManager::mColorCombinations2[g*2];
		int color2 = TGoalManager::mColorCombinations2[g*2+1];
	}

	GObjectPtr oa= CreateElementAtXYInternal(elementA,colorIdA,pa.x,pa.y,sta,false);
	GObjectPtr ob =CreateElementAtXYInternal(elementB,colorIdB,pb.x,pb.y,stb,false);
	
	if(oa!=NULL && ob!=NULL )
	{
		GBox2dComponent* cA  = oa->GetComponent<GBox2dComponent>(oa->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cB  = ob->GetComponent<GBox2dComponent>(ob->GetComponentIdByIndex(GC_BOX2D_IDX));				
		
		//create the joints
		b2DistanceJointDef jointDef; 
		jointDef.dampingRatio =0.0;
		jointDef.frequencyHz = 10.0;
		jointDef.collideConnected = false;
		jointDef.Initialize(cA->mBody, cB->mBody, pa,pb);

		/*
		b2RevoluteJointDef jointDef;
		jointDef.collideConnected = false;
		jointDef.localAnchorA.Set(-0.25f,-0.0f);
		jointDef.localAnchorB.Set( 0.25f,0.0f);
		jointDef.enableLimit = true;
		jointDef.lowerAngle = -45.0f * 3.14160f/180.0f;
		jointDef.upperAngle =  45.0f * 3.1416f/180.0f;
		jointDef.bodyA = cA->mBody;
		jointDef.bodyB = cB->mBody;
	
		*/
		
		mWorld->CreateJoint(&jointDef);

	
	}
}

void TGameStateDemo::CreateTri2NoSynth(int x, int y, int radius, int angle, int elementA,  int elementB, int elementC)
{
	float finangle = (float)angle*3.1416f/180.0f;
	float fangle =  0.0f;
	float fradius = (float)radius / 10.0f;
	const float fstep = (3.1416f*2.0f)/2.0f;
	float fx = (float)x/10.0f;
	float fy = (float)y/10.0f; 

	fangle = finangle+fstep*1.0;
	b2Vec2 pa( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	b2Vec2 pb(fx,fy);	
	
	fangle = finangle+fstep*0.0f;
	b2Vec2 pc( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	bool sta = (elementA&0x100)==0x100;
	bool stb = (elementB&0x100)==0x100;
	bool stc = (elementC&0x100)==0x100;
	

	int colorIdA = (elementA&0x0F);
	int colorIdB = (elementB&0x0F);
	int colorIdC = (elementC&0x0F);
	if(colorIdA == 0x0F) colorIdA = -1;
	if(colorIdB == 0x0F) colorIdB = -1;
	if(colorIdC == 0x0F) colorIdC = -1;
	
	elementA = (elementA>>4)&0x0F;
	elementB = (elementB>>4)&0x0F;
	elementC = (elementC>>4)&0x0F;

	if(elementA == 0x0F) elementA = -1;
	if(elementB == 0x0F) elementB = -1;
	if(elementC == 0x0F) elementC = -1;
	
	
	if(colorIdA == -1 && colorIdB == -1 && colorIdC == -1)
	{

		int retries = 3;
		bool valid = false;

		while(!valid && retries >0)
		{

			valid = true;
			retries--;

			int maxPosibilities=1;

			switch(mLevelValues.maxColors)
			{
				case 3:
					maxPosibilities=1;
					break;
				case 4:
					maxPosibilities=4;
					break;
				case 5:
					maxPosibilities=10;
					break;
			}



			int g = rand()%maxPosibilities;
			int scramble = rand()%3;
			int oc1=0;
			int oc2=1;
			int oc3=2;
		

			switch(scramble)
			{
				case 0:
				{
					oc1=0;oc2=1,oc3=2;
				}
				break;
				case 1:
				{
					oc1=0;oc2=2,oc3=1;
				}
				break;
				case 2:
				{
					oc1=1;oc2=0,oc3=2;
				}			
				break;
		
		
			}

			colorIdA = TGoalManager::mColorCombinations3[g*3+oc1];
			colorIdB = TGoalManager::mColorCombinations3[g*3+oc2];
			colorIdC = TGoalManager::mColorCombinations3[g*3+oc3];


			//if trio exists chose other combination

			for(int i=0;i<mElementList.size();i++)
			{
			
				GControllerComponent* ccomp = mElementList[i]->GetComponent<GControllerComponent>(mElementList[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				GBox2dComponent* box2d = mElementList[i]->GetComponent<GBox2dComponent>(mElementList[i]->GetComponentIdByIndex(GC_BOX2D_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;		
				//get the box2d component
				b2JointEdge* edge = box2d->mBody->GetJointList();
				GObjectPtr chain3[3];
				chain3[0]=NULL;
				chain3[1]=NULL;
				chain3[2]=NULL;
				bool trio = false;

				if(edge!=NULL) //this is at leat a duo
				{	
					chain3[0]=mElementList[i];
					chain3[1]=(GObjectPtr)edge->other->GetUserData();
					edge = edge->next;

					//this has 2 edges , then it is a trio
					if(edge)
					{
						chain3[2]=(GObjectPtr)edge->other->GetUserData();
						trio=true;
					}
					else
					{
						GControllerComponent* ccomp2 = chain3[1]->GetComponent<GControllerComponent>(chain3[1]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						GBox2dComponent* box2d2 = chain3[1]->GetComponent<GBox2dComponent>(chain3[1]->GetComponentIdByIndex(GC_BOX2D_IDX));
						TElementController* econt2 = (TElementController*)ccomp2->gameController;		
						b2JointEdge* edge2 = box2d2->mBody->GetJointList();
						//check if other body has edge then it is a trio
						if(edge2)
						{
							if(edge2->other->GetUserData() == chain3[0])
								edge2 = edge2->next;

							if(edge2)
							{

								chain3[2]=(GObjectPtr)edge2->other->GetUserData();
								trio=true;
							}
						}
					}
				}

				if(trio)
				{
					//check colors
					int tcA=0;
					int tcB=0;
					int tcC=0;
					for(int i=0;i<3;i++)
					{
						GControllerComponent* ccomp2 = chain3[i]->GetComponent<GControllerComponent>(chain3[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						TElementController* econt2= (TElementController*)ccomp2->gameController;		

						if(colorIdA == econt2->mColorId)
							tcA++;
						if(colorIdB == econt2->mColorId)
							tcB++;
						if(colorIdC == econt2->mColorId)
							tcC++;
					}

					if(tcA ==1 && tcB==1 && tcC ==1)
					{
						valid = false;
						break; //for
					}
				}
			}
		}
	};


	GObjectPtr oa= CreateElementAtXYInternal(elementA,colorIdA,pa.x,pa.y,sta,false);
	GObjectPtr ob =CreateElementAtXYInternal(elementB,colorIdB,pb.x,pb.y,stb,false);
	GObjectPtr oc= CreateElementAtXYInternal(elementC,colorIdC,pc.x,pc.y,stc,false);
	
	if(oa!=NULL && ob!=NULL && oc!=NULL)
	{
		//create the joints
		b2DistanceJointDef jointDef; 
		jointDef.dampingRatio = 0.0f;
		jointDef.frequencyHz = 10.0;
		

		GBox2dComponent* cA  = oa->GetComponent<GBox2dComponent>(oa->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cB  = ob->GetComponent<GBox2dComponent>(ob->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cC  = oc->GetComponent<GBox2dComponent>(oc->GetComponentIdByIndex(GC_BOX2D_IDX));				
	
		b2Body* ba= cA->mBody;
		b2Body* bb= cB->mBody;
		b2Body* bc= cC->mBody;
	

		jointDef.Initialize(ba, bb, pa,pb);
		jointDef.collideConnected = false;
		mWorld->CreateJoint(&jointDef);

		jointDef.Initialize(bb, bc, pb,pc);
		jointDef.collideConnected = false;
		mWorld->CreateJoint(&jointDef);

	}
}


void TGameStateDemo::CreateTri2(int x, int y, int radius, int angle, int elementA,  int elementB, int elementC)
{
	float fangle = (float)angle*3.1416f/180.0f;
	float fradius = (float)radius / 10.0f;
	const float fstep = (3.1416f*2.0f)/2.0f;
	float fx = (float)x/10.0f;
	float fy = (float)y/10.0f; 

	b2Vec2 pa(fx,fy);// fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	//fangle = fangle + fstep;
	b2Vec2 pb( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	fangle = fangle + fstep;
	b2Vec2 pc( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	bool sta = (elementA&0x100)==0x100;
	bool stb = (elementB&0x100)==0x100;
	bool stc = (elementC&0x100)==0x100;
	

	int colorIdA = (elementA&0x0F);
	int colorIdB = (elementB&0x0F);
	int colorIdC = (elementC&0x0F);
	if(colorIdA == 0x0F) colorIdA = -1;
	if(colorIdB == 0x0F) colorIdB = -1;
	if(colorIdC == 0x0F) colorIdC = -1;
	
	elementA = (elementA>>4)&0x0F;
	elementB = (elementB>>4)&0x0F;
	elementC = (elementC>>4)&0x0F;

	if(elementA == 0x0F) elementA = -1;
	if(elementB == 0x0F) elementB = -1;
	if(elementC == 0x0F) elementC = -1;
	
	
	if(colorIdA == -1 && colorIdB == -1 && colorIdC == -1)
	{

		int retries = 3;
		bool valid = false;

		while(!valid && retries >0)
		{

			valid = true;
			retries--;

			int maxPosibilities=1;

			switch(mLevelValues.maxColors)
			{
				case 3:
					maxPosibilities=1;
					break;
				case 4:
					maxPosibilities=4;
					break;
				case 5:
					maxPosibilities=10;
					break;
			}



			int g = rand()%maxPosibilities;
			int scramble = rand()%3;

			int oc1=0;
			int oc2=1;
			int oc3=2;
		

			switch(scramble)
			{
				case 0:
				{
					oc1=0;oc2=1,oc3=2;
				}
				break;
				case 1:
				{
					oc1=0;oc2=2,oc3=1;
				}
				break;
				case 2:
				{
					oc1=1;oc2=0,oc3=2;
				}			
				break;
		
		
			}

			colorIdA = TGoalManager::mColorCombinations3[g*3+oc1];
			colorIdB = TGoalManager::mColorCombinations3[g*3+oc2];
			colorIdC = TGoalManager::mColorCombinations3[g*3+oc3];


			//if trio exists chose other combination

			for(int i=0;i<mElementList.size();i++)
			{
			
				GControllerComponent* ccomp = mElementList[i]->GetComponent<GControllerComponent>(mElementList[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				GBox2dComponent* box2d = mElementList[i]->GetComponent<GBox2dComponent>(mElementList[i]->GetComponentIdByIndex(GC_BOX2D_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;		
				//get the box2d component
				b2JointEdge* edge = box2d->mBody->GetJointList();
				GObjectPtr chain3[3];
				chain3[0]=NULL;
				chain3[1]=NULL;
				chain3[2]=NULL;
				bool trio = false;

				if(edge!=NULL) //this is at leat a duo
				{	
					chain3[0]=mElementList[i];
					chain3[1]=(GObjectPtr)edge->other->GetUserData();
					edge = edge->next;

					//this has 2 edges , then it is a trio
					if(edge)
					{
						chain3[2]=(GObjectPtr)edge->other->GetUserData();
						trio=true;
					}
					else
					{
						GControllerComponent* ccomp2 = chain3[1]->GetComponent<GControllerComponent>(chain3[1]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						GBox2dComponent* box2d2 = chain3[1]->GetComponent<GBox2dComponent>(chain3[1]->GetComponentIdByIndex(GC_BOX2D_IDX));
						TElementController* econt2 = (TElementController*)ccomp2->gameController;		
						b2JointEdge* edge2 = box2d2->mBody->GetJointList();
						//check if other body has edge then it is a trio
						if(edge2)
						{
							if(edge2->other->GetUserData() == chain3[0])
								edge2 = edge2->next;

							if(edge2)
							{

								chain3[2]=(GObjectPtr)edge2->other->GetUserData();
								trio=true;
							}
						}
					}
				}

				if(trio)
				{
					//check colors
					int tcA=0;
					int tcB=0;
					int tcC=0;
					for(int i=0;i<3;i++)
					{
						GControllerComponent* ccomp2 = chain3[i]->GetComponent<GControllerComponent>(chain3[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						TElementController* econt2= (TElementController*)ccomp2->gameController;		

						if(colorIdA == econt2->mColorId)
							tcA++;
						if(colorIdB == econt2->mColorId)
							tcB++;
						if(colorIdC == econt2->mColorId)
							tcC++;
					}

					if(tcA ==1 && tcB==1 && tcC ==1)
					{
						valid = false;
						break; //for
					}
				}
			}
		}
	};


	GObjectPtr oa= CreateElementAtXYInternal(elementA,colorIdA,pa.x,pa.y,sta);
	GObjectPtr ob =CreateElementAtXYInternal(elementB,colorIdB,pb.x,pb.y,stb);
	GObjectPtr oc= CreateElementAtXYInternal(elementC,colorIdC,pc.x,pc.y,stc);
	
	if(oa!=NULL && ob!=NULL && oc!=NULL)
	{
		//create the joints
		b2DistanceJointDef jointDef; 
		  	jointDef.dampingRatio = 1.0f;
		jointDef.frequencyHz = 10.0;

		GBox2dComponent* cA  = oa->GetComponent<GBox2dComponent>(oa->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cB  = ob->GetComponent<GBox2dComponent>(ob->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cC  = oc->GetComponent<GBox2dComponent>(oc->GetComponentIdByIndex(GC_BOX2D_IDX));				
	
		b2Body* ba= cA->mBody;
		b2Body* bb= cB->mBody;
		b2Body* bc= cC->mBody;
	

		jointDef.Initialize(bc, ba, pc,pa);
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);

		jointDef.Initialize(ba, bb, pa,pb);
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);

	}


}


void TGameStateDemo::CreateTetra(int x, int y, int radius, int angle, int elementA,  int elementB, int elementC, int elementX)
{
	/* TEMPORARY DISABLED
	float fangle = (float)angle*3.1416f/180.0f;
	float fradius = (float)radius / 10.0f;
	const float fstep = (3.1416f*2.0f)/3.0f;
	float fx = (float)x/10.0f;
	float fy = (float)y/10.0f;

	b2Vec2 pa( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	fangle = fangle + fstep;
	b2Vec2 pb( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	fangle = fangle + fstep;
	b2Vec2 pc( fx + fradius*cosf(fangle),fy +  fradius*sinf(fangle));
	
	b2Vec2 px(fx,fy);

	bool sta = (elementA&0x100)==0x100;
	bool stb = (elementB&0x100)==0x100;
	bool stc = (elementC&0x100)==0x100;
	bool stx = (elementX&0x100)==0x100;

	int colorIdA = (elementA&0x0F);
	int colorIdB = (elementB&0x0F);
	int colorIdC = (elementC&0x0F);
	int colorIdX = (elementX&0x0F);
	if(colorIdA == 0x0F) colorIdA = -1;
	if(colorIdB == 0x0F) colorIdB = -1;
	if(colorIdC == 0x0F) colorIdC = -1;
	if(colorIdX == 0x0F) colorIdX = -1;

	elementA = (elementA>>4)&0x0F;
	elementB = (elementB>>4)&0x0F;
	elementC = (elementC>>4)&0x0F;
	elementX = (elementX>>4)&0x0F;

	if(elementA == 0x0F) elementA = -1;
	if(elementB == 0x0F) elementB = -1;
	if(elementC == 0x0F) elementC = -1;
	if(elementX == 0x0F) elementX = -1;
	

	GObjectPtr oa= CreateElementAtXYInternal(elementA,colorIdA,pa.x,pa.y,sta);
	GObjectPtr ob =CreateElementAtXYInternal(elementB,colorIdB,pb.x,pb.y,stb);
	GObjectPtr oc= CreateElementAtXYInternal(elementC,colorIdC,pc.x,pc.y,stc);
	GObjectPtr ox= CreateElementAtXYInternal(elementX,colorIdX,px.x,px.y,stx);
	
	if(oa!=NULL && ob!=NULL && oc!=NULL && ox!= NULL)
	
	{
		//create the joints
		b2DistanceJointDef jointDef; 
		 
		GBox2dComponent* cA  = oa->GetComponent<GBox2dComponent>(oa->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cB  = ob->GetComponent<GBox2dComponent>(ob->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cC  = oc->GetComponent<GBox2dComponent>(oc->GetComponentIdByIndex(GC_BOX2D_IDX));				
		GBox2dComponent* cX  = ox->GetComponent<GBox2dComponent>(ox->GetComponentIdByIndex(GC_BOX2D_IDX));				
	
		b2Body* ba= cA->mBody;
		b2Body* bb= cB->mBody;
		b2Body* bc= cC->mBody;
		b2Body* bx= cX->mBody;
	
		jointDef.Initialize(bx, ba, px,pa);
		jointDef.userData = NULL;
		jointDef.collideConnected = true;
		//jointDef.frequencyHz = 2.0f;
		//jointDef.dampingRatio = 0.5f;
		mWorld->CreateJoint(&jointDef);

		
		jointDef.Initialize(bx, bb, px,pb);
		jointDef.userData = NULL;
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);


		jointDef.Initialize(bx, bc,px,pc);
		jointDef.userData = NULL;
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);

		uintptr_t value = 1;
		
		jointDef.Initialize(ba, bb, pa,pb);
		jointDef.userData = (void*)value;
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);

		jointDef.Initialize(ba, bc, pa,pc);
		jointDef.userData = (void*)value;
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);

		jointDef.Initialize(bb, bc, pb,pc);
		jointDef.userData = (void*)value;
		jointDef.collideConnected = true;
		mWorld->CreateJoint(&jointDef);
		
	}

	*/
}



void TGameStateDemo::ClearElements()
{
	for (irr::u32 i = 0; i < mElementList.size(); i++)
	{	
		mElementList[i]->Destroy();		
		delete mElementList[i];
	}

	mElementList.clear();

	for(int i=0; i<mCreatorsPool.size(); i++)
	{
		GControllerComponent* gcontcreator =  mCreatorsPool[i]->GetComponent<GControllerComponent>(mCreatorsPool[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TElementCreatorController* creator =  (TElementCreatorController*)gcontcreator->gameController ;		
		creator->setState(CREATOR_STATE_FINISHED);
	}


	//mLastPlayerContact = NULL;


}

void TGameStateDemo::ClearBits()
{
	for (irr::u32 i = 0; i < mBitList.size(); i++)
	{	
		mBitList[i]->Destroy();		
		delete mBitList[i];
	}

	mBitList.clear();
}


void TGameStateDemo::ClearShield()
{

	for (irr::u32 i = 0; i < mShieldList.size(); i++)
	{	
		mShieldList[i]->Destroy();		
		delete mShieldList[i];
	}


	mShieldList.clear();


}

void TGameStateDemo::VInit() 
{ 
	GGameApp* app = GGameApp::getInstance();


	//register as input listener
	if(app->getEventManager())
		app->getEventManager()->AttachListener(*this);

	//register as menu event listener
	if(app->getEventManager())
		app->getEventManager()->AttachRocketListener(*this);


#if defined(_IRR_ANDROID_PLATEFORM_)
	if(app->getScriptManager())
		app->getScriptManager()->AttachScript("testLevel.lua",(GScriptable*) this);
#else
	if(app->getScriptManager())
		app->getScriptManager()->AttachScript("IrrlichtSdcard/testLevel.lua",(GScriptable*) this);	
#endif
	mScriptState = (int)GScriptable::NONE;

	//setup the irrlicht camera
	mCamera = app->getIrrlichtDevice()->getSceneManager()->addCameraSceneNode(0, vector3df(0.0f,.0f,-1.0f), vector3df(0.0f,-0.0f,0.0f));

	irr::scene::ISceneManager* smgr= GGameApp::getInstance()->getIrrlichtDevice()->getSceneManager();	
	irr::f32 worldHeight = 0.0f;
	irr::core::line3df ray;
	core::vector3df cameraPos = core::vector3df(0.0f, 0.0f, -1.0f);
	core::plane3df plane(core::vector3df(0.0f, 0.0f, 0.0f), cameraPos);
	core::vector3df p3dout;
	while(worldHeight<9.0f)
	{
		ray =  smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
				irr::core::vector2di(0,0), 
				mCamera);
		
		plane.getIntersectionWithLine(ray.start, ray.getVector(), p3dout);
		cameraPos.Z = cameraPos.Z-0.2f;
		
		mCamera->setPosition(cameraPos);
		mCamera->updateAbsolutePosition();
		mCamera->render();


		worldHeight = p3dout.Y;
	}


	//setup the world
	b2Vec2 gravity;
	gravity.Set(0.0f, -2.1f);	 //set gravity
	mWorld = new b2World(gravity, false);	
	mWorld->SetWarmStarting(mWorldSettings.enableWarmStarting > 0);
	mWorld->SetContinuousPhysics(mWorldSettings.enableContinuous > 0);
	mWorld->SetSubStepping(mWorldSettings.enableSubStepping > 0);


	mWorld->SetContactListener(this); //<-- listen to collision events

	SetupTextures();
	SetupRenderTargets(false);
	SetupSceneNodes(false);
		



	//creat the shield
	/*
	for(int i=0; i<SHIELD_MAX_OBJECTS;i++)
	{
		irr::u32 time=0;

		time = ((i/2) * 3*(SHIELD_SIZE_TIME) + (i%2) * (SHIELD_SIZE_TIME)); 
		time = time + 1.5f*SHIELD_SIZE_TIME;
	
		GObjectPtr shield = CreateShield(time);
		if(shield != NULL)
		{				
			mShieldList.push_back(shield);
			mShieldCount++;
		}
	}
	*/
	mShieldCreationTime = 0;

	


	mSmoothPoints.init();
	


}

void TGameStateDemo::UpdateStarBackground(irr::u32 ms)
{

	if(mStarBackground)
	{
		float delta = ((float)ms/1000.0f) * 3.1416f  / 4.0f;

		mStarFieldAngle = mStarFieldAngle + delta;
			 
		GIrrlichtComponent* ircomp = mStarBackground->GetComponent<GIrrlichtComponent>(mStarBackground->GetComponentIdByIndex(GC_IRRNODE_IDX));
		SpriteNode* node = (SpriteNode*)ircomp->mNode;
		float sh = 0.25f;
		float sw = sh * ((float)CRASH_AREA_HALF_WIDTH/(float)CRASH_AREA_HALF_HEIGHT);

		vector3df p1= vector3df(     0.5f - sw, 0.5f -sh,0.0f);
		vector3df p2= vector3df(     0.5f + sw, 0.5f +sh,0.0f);
		core::matrix4 translateMat;
		core::matrix4 translateMat2;
		core::matrix4 scaleMat;
		core::matrix4 rotateMat;

		scaleMat.setScale(irr::core::vector3df(0.5f,0.5f,0.0f));
		translateMat.setTranslation(irr::core::vector3df(0.5f,0.5f,0.0f));
		translateMat2.setTranslation(irr::core::vector3df(-0.5f,-0.5f,0.0f));
		rotateMat.setRotationDegrees(irr::core::vector3df(0.0f,0.0f,mStarFieldAngle));
		
		translateMat*= scaleMat;
		translateMat*= rotateMat;
		translateMat*= translateMat2;		
		node->getMaterial(0).getTextureMatrix(0).setM(translateMat.pointer());

		
		
	}

}
void TGameStateDemo::SetupStarBackground(bool reinit)
{	//calculate v2

	if(!reinit)
	{
		mStarBackground = TElementsFactory::CreateBasicSprite(0.0f,-1.0f,CRASH_AREA_HALF_WIDTH*2.0f, CRASH_AREA_HALF_HEIGHT*2.0f,0);
	
		if(mStarBackground)
		{
			GIrrlichtComponent* ircomp = mStarBackground->GetComponent<GIrrlichtComponent>(mStarBackground->GetComponentIdByIndex(GC_IRRNODE_IDX));
			GTransformComponent* trcomp = mStarBackground->GetComponent<GTransformComponent>(mStarBackground->GetComponentIdByIndex(GC_TRANSFORM_IDX));
			SpriteNode* node = (SpriteNode*)ircomp->mNode;
			//load the star texture
			#if defined(_IRR_ANDROID_PLATEFORM_)		
			node->Load("StarBG.tga",256,256,CRASH_AREA_HALF_WIDTH*2.0f,CRASH_AREA_HALF_HEIGHT*2.0f);
			#else
			node->Load("IrrlichtSdcard/StarBG.tga",256,256,CRASH_AREA_HALF_WIDTH*2.0f,CRASH_AREA_HALF_HEIGHT*2.0f);
			#endif
			node->setTransparent(false);		
			trcomp->mTrans.Set(b2Vec2(0.0f,0.05),0.0f);
			node->setPosition(irr::core::vector3df(0.0f,-1.0f,0.06f));

			node->getMaterial(0).MaterialType = EMT_SOLID;
			node->getMaterial(0).ZBuffer = ECFN_NEVER;
			node->setVisible(false);
			mStarFieldAngle = 0.0f;

			UpdateStarBackground(0);
		}
	}
	else
	{
		GGameApp* app = GGameApp::getInstance();

		GIrrlichtComponent* ircomp = mStarBackground->GetComponent<GIrrlichtComponent>(mStarBackground->GetComponentIdByIndex(GC_IRRNODE_IDX));
		SpriteNode* node = (SpriteNode*)ircomp->mNode;
		node->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());
	
	}

	
		

}
void TGameStateDemo::SetupStaticShieldNodes()
{
	mStaticShieldList.clear();
	
	GObjectPtr gameObject= NULL;
	GSpriteComponent* spcomp = NULL;
	irr::video::SColor clrfrom(255,255,0,255);
	irr::video::SColor clrto(255,0,0,255);
		
		
	gameObject = TElementsFactory::CreateBasicBatchedSprite(CRASH_AREA_HALF_WIDTH,-1.0f,
																			CRASH_AREA_HALF_HEIGHT*2.0f,0.2f,
																			b2Vec2(8.0f,  178.0f),
																			b2Vec2(102.0f,195.0f),
																			90.0f*3.1416f/180.0f);

	spcomp = gameObject->GetComponent<GSpriteComponent>(gameObject->GetComponentIdByIndex(GC_SPRITE_IDX));
	spcomp->mColor = clrfrom;
	spcomp->mColor2 = clrto;
	spcomp->mUse2Colors=true;
	mStaticShieldList.push_back(gameObject);

	gameObject = TElementsFactory::CreateBasicBatchedSprite( -CRASH_AREA_HALF_WIDTH,-1.0f,
																			CRASH_AREA_HALF_HEIGHT*2.0f,0.2f,
																			b2Vec2(8.0f,  178.0f),
																			b2Vec2(102.0f,195.0f),
																			90.0f*3.1416f/180.0f);
	
	spcomp = gameObject->GetComponent<GSpriteComponent>(gameObject->GetComponentIdByIndex(GC_SPRITE_IDX));
	spcomp->mColor = clrfrom;
	spcomp->mColor2 = clrto;
	spcomp->mUse2Colors=true;
	mStaticShieldList.push_back(gameObject);
}
void TGameStateDemo::WaitForClear(int ms)
{
	
	mLaunchTime = 0;
	mWaitTime = ms; 
	mWaiting=true;

	if(mWaitTime==-1)
		mWaitTime= 1000*60*24;
	
	mWaitingForClear = true;
	

}

void TGameStateDemo::WaitForCrash(int ms)
{	
	mLaunchTime = 0;
	mWaitTime = ms; 
	mWaiting=true;
	if(mWaitTime==-1)
		mWaitTime= 1000*60*24; 
	
	mWaitingForCrash = true;

}

void TGameStateDemo::WaitForTap(int ms)
{
	if(ms==-1)
		mPaused=true;

	if(ms==-2)
		ms = -1;

	mLaunchTime = 0;


	mWaitTime = ms; 
	mWaiting=true;



	if(mWaitTime==-1)
		mWaitTime= 1000*60*24; 
	
	mWaitingForTap = true;

	if(mGameMode == 1)
	{
		mGoalManager.startMonitoringScene(mLevelValues);

	}

}


void TGameStateDemo::WaitForJoin(int ms)
{
	
	mLaunchTime = 0;
	mWaitTime = ms; 
	mWaiting=true;
	if(mWaitTime==-1)
		mWaitTime= 1000*60*24; 
	
	mWaitingForJoin=true;	
	
}
void TGameStateDemo::VFinish() 
{
	GGameApp* app = GGameApp::getInstance();

	
	//remove the scene nodes
	GIrrlichtComponent* ncomp = mPlayer->GetComponent<GIrrlichtComponent>(mPlayer->GetComponentIdByIndex(GC_IRRNODE_IDX));
	ncomp->mNode->remove();


	ncomp = mSpringGrid->GetComponent<GIrrlichtComponent>(mSpringGrid->GetComponentIdByIndex(GC_IRRNODE_IDX));
	ncomp->mNode->remove();

	ncomp = mEnergyBar->GetComponent<GIrrlichtComponent>(mEnergyBar->GetComponentIdByIndex(GC_IRRNODE_IDX));
	ncomp->mNode->remove();

	ncomp = mRadiationBar->GetComponent<GIrrlichtComponent>(mEnergyBar->GetComponentIdByIndex(GC_IRRNODE_IDX));	
	ncomp->mNode->remove();

	mRadiationBar->Destroy();
	delete mRadiationBar;



	mEnergyBar->Destroy();
	delete mEnergyBar;


	mPlayer->Destroy();
	delete mPlayer;

	
	//mSynth3->Destroy();
	//delete mSynth3;


	mSpringGrid->Destroy();
	delete mSpringGrid;


	mPlayer = NULL;
	
	mSpringGrid = NULL;


	if(app->getEventManager())
		app->getEventManager()->DetachListener(*this);

	if(app->getEventManager())
		app->getEventManager()->DetachRocketListener(*this);


}



//process a librocket menu event
void TGameStateDemo::ProcessRocketEvent(const Rocket::Core::String& value)
{
	GGameApp* app = GGameApp::getInstance();

	
	if(value== "ToggleMenu")
	{

		app->mGameManager->VSetNextGameState(GAME_STATE_MAINMENU,0);
	}	
	else if(value == "ContinueTutorial")
	{

		if (mWaitingForTap)
		{
			mPaused=false;
			mWaitTime = 0;		
		}

		app->getRocketGUI()->SetDocumentVisible("tutorial_window",false);		
		
	}
	
}

void TGameStateDemo::OnKeyDown(irr::EKEY_CODE keycode)
{
}
void TGameStateDemo::OnKeyUp(irr::EKEY_CODE keycode)
{
	float ox = 0.0f;
	float oy = 0.0f;
	int angle = (rand()%18)*10;
	GGameApp* app = GGameApp::getInstance();


	if(!mHasFocus)
		return;


	if(mPlayer!=NULL)
	{
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* playerController = (TPlayerController*)ccomp->gameController;
		GTransformComponent* playerTransform = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));

		ox = playerTransform->mTrans.p.x * 10.0f;
		oy = playerTransform->mTrans.p.y * 10.0f;

	}

	switch(keycode)
	{
		case KEY_KEY_1:
			 CreateElementAtXY((int)ox, (int)oy, 0x00);
			break;
		case KEY_KEY_2:
			CreateElementAtXY ((int)ox, (int)oy,0x01);
		break;
		case KEY_KEY_3:
			CreateElementAtXY ((int)ox, (int)oy,0x02);
			break;
		case KEY_KEY_4:
			CreateElementAtXY ((int)ox, (int)oy,0x03);
			break;
		case KEY_KEY_5:
			CreateElementAtXY ((int)ox, (int)oy,0x04);
			break;
		case KEY_KEY_6:
			CreateElementAtXY ((int)ox, (int)oy,0x105);
			break;
		case KEY_KEY_7:
			
			break;
		case KEY_KEY_9:
			CreateElementAtXY((int)ox, (int)oy, 0x40);
			break;
		case KEY_KEY_R:
			//CreateElementAtRandXY(0x0f);
			CreateElementDistributed(12);
			break;
		case KEY_KEY_S:
			{


				if(mScriptState == (int)GScriptable::RUNNING)
					mScriptState = (int)GScriptable::FINALIZED;
				else
					if(mScriptState == (int)GScriptable::FINALIZED)
						mScriptState = (int)GScriptable::RUNNING;
			}
			break;
		
			/*
		case KEY_KEY_W:
			CreateElementAtXY ((int)ox, (int)oy,0x11);
		break;
		case KEY_KEY_E:
			CreateElementAtXY ((int)ox, (int)oy,0x12);
		break;
		case KEY_KEY_R:
			CreateDuo((int)ox, (int)oy, 15,angle, 0x1f,0x1f);
			break;
		case KEY_KEY_T:
			CreateTri((int)ox, (int)oy, 15,angle, 0x1f,0x1f,0x1f);
			break;
		case KEY_KEY_Y:
		CreateTetra((int)ox, (int)oy, 15,angle, 0x10,0x11,0x12,0x020);
		break;
		*/
		case KEY_KEY_C:
			ClearElements();
		break;
		case KEY_KEY_B: //back key
			{
				

				app->mGameManager->VSetNextGameState(GAME_STATE_MAINMENU,0);
			}
		break;
		case KEY_KEY_A:
			CreateElementAtXY ((int)ox, (int)oy,0x20);
		break;
		case KEY_KEY_Z:
			CreateElementAtXY ((int)ox, (int)oy,0x030);
		break;
		case KEY_KEY_I:
			{

				if(mPlayer!=NULL)
				{
					GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					TPlayerController* pcont = (TPlayerController*)ccomp->gameController;
					pcont->setInvincible(!pcont->getInvincible());
				}
			}
			
		break;
	}
}
void TGameStateDemo::OnMouseDown(int x, int y)
{
	mIsMouseDown = true;


	if(mPlayer)
	{
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* playerController = (TPlayerController*)ccomp->gameController;
		GBox2dComponent* bcomp = mPlayer->GetComponent<GBox2dComponent>(mPlayer->GetComponentIdByIndex(GC_BOX2D_IDX));
			
		mSmoothPoints.pointCount=0;
		
		if(playerController->getMoleState() != MOLE_STATE_NONE)
			return;

		
		irr::os::Printer::log("On MOuse Down");


		//if touch left then treat this as a mouse down
		#if T_USE_OFFSET

		//int screenWidth = GGameApp::getInstance()->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Width;

		if(true)
		#endif
		
		{
		

		
			playerController->setApplyForce(true);
			playerController->setMoleState(MOLE_STATE_COLLECTING);
			playerController->clearMoles();
			irr::os::Printer::log("setMoleState MOLE_STATE_COLLECTING");

			OnMouseMove(x,y);


			DoLogicPreSimulation(0);


			/*if(mLastPlayerContact != NULL)
			{
				GControllerComponent* testgc = mLastPlayerContact->GetComponent<GControllerComponent>(mLastPlayerContact->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController*   testec = (TElementController*)testgc->gameController;

				testec->setContactTimer(0);
				testec->setHasContactTimer(false);
				mLastPlayerContact =NULL;
			}
			*/
		}

		#if T_USE_OFFSET
		else
		{
			//cancel anything
			playerController->setCrashState(CRASH_STATE_NONE);

		}
		

		//calculate player offset
		//x=x*2;
		//y=y*2;
		playerController->PositionQueueClear();
		
		
		irr::scene::ISceneManager* smgr= GGameApp::getInstance()->getIrrlichtDevice()->getSceneManager();
		irr::core::line3df ray = 
					smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
					irr::core::vector2di(x,y), 
					smgr->getActiveCamera());

		core::plane3df plane(core::vector3df(0, 0, 0), core::vector3df(0, 0, -1));
		core::vector3df mouseWorldPosition;

		if(plane.getIntersectionWithLine(ray.start, ray.getVector(), mouseWorldPosition))
		{		 	
			b2Vec2 xyPos = b2Vec2(mouseWorldPosition.X, mouseWorldPosition.Y);
			playerController->mPlayerOffset= xyPos;

			//get current player position
			/*GTransformComponent* tcomp = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
			b2Vec2 xyPos = b2Vec2(mouseWorldPosition.X, mouseWorldPosition.Y);
			xyPos = tcomp->mTrans.p - xyPos;
			playerController->mPlayerOffset= xyPos / 2.0f;

			b2Vec2 newPlayerPosition = b2Vec2(mouseWorldPosition.X,mouseWorldPosition.Y);
			newPlayerPosition = newPlayerPosition +2.0f*playerController->mPlayerOffset;
			bcomp->mBody->SetTransform(newPlayerPosition, 0.0f);
			*/

		}
		
		#endif
		
		


		
	}

}
void TGameStateDemo::OnMouseUp(int x, int y)
{
	mIsMouseDown = false;

	if(mPlayer)
	{
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* playerController = (TPlayerController*)ccomp->gameController;
		//

		int toAdd=0;
		bool didSomething = false;
		

		irr::os::Printer::log("OnMouseUp");

		if(playerController->getMoleState() == MOLE_STATE_COLLECTING)
		{
			if(playerController->mMoleCount>0)
			{


				playerController->setMoleState(MOLE_STATE_PROCESSING);
				irr::os::Printer::log("setMoleState MOLE_STATE_PROCESSING");
				b2Vec2 positions;
				positions.SetZero();
				

				AudioItem audioUp;
				audioUp.mChunkId = AudioManager::gChunkIndexes[AudioItem::CHUNK_PULL];
				audioUp.mFlags = AudioItem::PLAYONCE;
				audioUp.mLoop=0;				
				audioUp.mId = GGameApp::getInstance()->getAudioManager()->playSound(audioUp);

				//set the flags
				
				for (irr::u32 j = 0; j < playerController->mMoleCount; j++)
				{
					Mole playerMole;
					playerController->mMoles[j].populateMole(&playerMole, mElementList);


					for (irr::u32 i = 0; i < playerMole.count; i++)
					{
						GObjectPtr element =  playerMole.atoms[i];
						GControllerComponent* eeccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						TElementController* econt = (TElementController*)eeccomp->gameController;					
						GTransformComponent* trans  = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));
						if(i==0)
						{

							positions.x =  positions.x + trans->mTrans.p.x;
							positions.y =  positions.y + trans->mTrans.p.y;
							
							econt->setHasPlayerContact(true);
							econt->setAllowAttraction(true);
						}
						econt->setHasContact(true);
						econt->setInCrashForce(true);
					}
				}

				//center the player

				if(playerController->mMoleCount>1)
				{
					GBox2dComponent* bcomp = mPlayer->GetComponent<GBox2dComponent>(mPlayer->GetComponentIdByIndex(GC_BOX2D_IDX));
		
					if(bcomp)
					{

						positions.x = positions.x/ (float)playerController->mMoleCount;
						positions.y = positions.y/ (float)playerController->mMoleCount;

						bcomp->mBody->SetTransform(positions, 0.0f);
					}
				}
				else
				{
				
					GBox2dComponent* bcomp = mPlayer->GetComponent<GBox2dComponent>(mPlayer->GetComponentIdByIndex(GC_BOX2D_IDX));
		
					if(bcomp)
					{

						positions.x = (positions.x+bcomp->mBody->GetTransform().p.x) / 2.0f;
						positions.y = (positions.y+bcomp->mBody->GetTransform().p.y) / 2.0f;
						bcomp->mBody->SetTransform(positions, 0.0f);
					}
				
				}

				playerController->setApplyForce(false);


			}
			else
			{
				playerController->setMoleState(MOLE_STATE_NONE);
				irr::os::Printer::log("setMoleState MOLE_STATE_NONE");
				playerController->setApplyForce(false);
			}

		}
		

		return;
		//STOP HERE

	}

}


void TGameStateDemo::OnMouseMove(int x, int y)
{
	#if T_USE_OFFSET
	if(!mIsMouseDown)
		return;
	#endif

	if(mPlayer)
	{

		irr::scene::ISceneManager* smgr= GGameApp::getInstance()->getIrrlichtDevice()->getSceneManager();

		//calculate world x/y
		#if T_USE_OFFSET
		/*
		x=x*2;
		y=y*2;
		*/
		#endif
			
		irr::core::line3df ray = 
					smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
					irr::core::vector2di(x,y), 
					smgr->getActiveCamera());

		core::plane3df plane(core::vector3df(0, 0, 0), core::vector3df(0, 0, -1));
		core::vector3df mouseWorldPosition;

		if(plane.getIntersectionWithLine(ray.start, ray.getVector(), mouseWorldPosition))
		{		 	
			//set player position in world coordinates
			GComponentId compId = mPlayer->GetComponentIdByIndex(GC_BOX2D_IDX);
			GBox2dComponent* bcomp = mPlayer->GetComponent<GBox2dComponent>(compId);
			GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TPlayerController* playerController = (TPlayerController*)ccomp->gameController;
				
			float speedX = 0.0f; 
			float speedY = 0.0f;			
			
			#if T_USE_OFFSET

				CIrrDeviceStub* stub = (CIrrDeviceStub*)(GGameApp::getInstance()->getIrrlichtDevice());
				stub->GetMoveSpeed(speedX, speedY);

				MovementData md;
				md.x =mouseWorldPosition.X;
				md.y =mouseWorldPosition.Y;
				md.speedX = speedX;
				md.speedY = speedY;
				playerController->PositionQueueAdd(md);
	
		


				/*
				GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TPlayerController* playerController = (TPlayerController*)ccomp->gameController;
				if(bcomp)
				{
					b2Vec2 newPlayerPosition = b2Vec2(mouseWorldPosition.X,mouseWorldPosition.Y);
					newPlayerPosition = newPlayerPosition + 2.0f* playerController->mPlayerOffset;
					bcomp->mBody->SetTransform(newPlayerPosition, 0.0f);
				}
				*/
			#else
				if(bcomp)
				{	

					if(playerController->getMoleState() == MOLE_STATE_COLLECTING)
					{
						GTransformComponent* trans  = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));

						bcomp->mBody->SetTransform(b2Vec2(mouseWorldPosition.X,mouseWorldPosition.Y), 0.0f);
						trans->mTrans.p.Set(mouseWorldPosition.X,mouseWorldPosition.Y);

						if(playerController->mMoleCount==3)
						{
							playerController->setApplyForce(false);
						}
					

					}
				
				}

			#endif
		}
	}	




}


void TGameStateDemo::DoLevelUpAnimation(irr::u32 ms)
{	
	
	if(mDoNextLevelAnimation)
	{
		mNextLevelTimer+=ms;
		
		if(mNextLevelTimer<1000)
		{
			
			int step =  (mNextLevelTimer%1000);
			step = step / 25; 
			step = step%3;

			
			if(mSpringGrid)
			{
				GIrrlichtComponent* ncomp = mSpringGrid->GetComponent<GIrrlichtComponent>(mSpringGrid->GetComponentIdByIndex(GC_IRRNODE_IDX));
				SpringNode* sn = (SpringNode*)ncomp->mNode;
				irr::video::SColor clr(255,200,200,0);
				
				if(step==0)
				{			
					clr.set(255,200,200,0);
				}
				else if(step==1)
				{
					clr.set(255,200,0,200);	
				}
				else if(step==2)
				{
					clr.set(255,200,0,0);
				}

				

				sn->SetVertexColor(clr);	

			}
			

		}
		else	
		{
			mDoNextLevelAnimation=false;
			mNextLevelTimer=0;

			if(mSpringGrid)
			{
				GIrrlichtComponent* ncomp = mSpringGrid->GetComponent<GIrrlichtComponent>(mSpringGrid->GetComponentIdByIndex(GC_IRRNODE_IDX));
				SpringNode* sn = (SpringNode*)ncomp->mNode;			
				sn->SetVertexColor(irr::video::SColor(255,128,0,196));
			}
		}	
	}

}

//create the spring grid object
GObjectPtr TGameStateDemo::CreateSpringGrid()
{

	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_SPRINGGRID,0);
	

	//create the spring 
	//Create Spring Node
/*	springrid::SpringGrid* springGrid = new springrid::SpringGrid(
			441,		//mass count					
			0.6,					// Each Particle Has A Weight Of 50 Grams
			400.0f,				// springConstant In The Rope
			0.7,					// Normal Length Of Springs In The Rope
			3.0,					// Spring Inner Friction Constant
			springrid::Vector3D(0, 0.0f, 0), // Gravitational Acceleration
			0.00f,					// Air Friction Constant
			100.0f,					// Ground Repel Constant
			0.0f,					// Ground Slide Friction Constant
			2.0f,					// Ground Absoption Constant
			-2.5f													
			,21);
	*/
	springrid::SpringGrid* springGrid = new springrid::SpringGrid(
			165,		//mass count					
			0.6,					// Each Particle Has A Weight Of 50 Grams
			400.0f,				// springConstant In The Rope
			1.0f,					// Normal Length Of Springs In The Rope
			2.0,					// Spring Inner Friction Constant
			springrid::Vector3D(0, 0.0f, 0), // Gravitational Acceleration
			0.00f,					// Air Friction Constant
			100.0f,					// Ground Repel Constant
			0.0f,					// Ground Slide Friction Constant
			2.0f,					// Ground Absoption Constant
			-2.5f													
			,11);

	 
	 
	//create the scene node
	
	SpringNode* springNode = new SpringNode(
		app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
				app->getIrrlichtDevice()->getSceneManager(),
				object->GetId(),
				springGrid);

#if defined(_IRR_ANDROID_PLATEFORM_)
	springNode->Load("Figures.tga",128,128,1.0,1.0);	
#else
	springNode->Load("IrrlichtSdcard/Figures.tga",128,128,1.0,1.0);	
#endif

	springNode->setFrame(5);
	springNode->setTransparent(true); //<-- trasparent are rendered in top of non transparent objects
	springNode->setVisible(true); 
	//springNode->SetVertexColor(irr::video::SColor(255,64,0,96));
	springNode->SetVertexColor(irr::video::SColor(255,128,0,196));
	springNode->setPosition(irr::core::vector3df(0.0f,0.0,0.05f));

	//add the components to the object

	//add the spring conmponent
	TSpringGridComponent* bcomp = new TSpringGridComponent();
	bcomp->SetOwner(object);
	bcomp->VInit(springGrid);
	object->AddComponent(bcomp);
	
	//add the irrlicht component
	GIrrlichtComponent* ncomp = new GIrrlichtComponent();
	ncomp->SetOwner(object);
	ncomp->VInit(springNode);
	object->AddComponent(ncomp);
	
	object->PostInit(); //post initialize all components 
	return object;
}






GObjectPtr TGameStateDemo::CreateShield(irr::u32 time)
{

	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_SHIELD,0);

	GBox2dComponent::FixtureDefinitionsList fixtures;
	
	b2Transform trans;
	trans.Set(b2Vec2(-CRASH_AREA_HALF_WIDTH + SHIELD_PATH_LENGTH2Q+(SHIELD_SIZE/2.0f),CRASH_AREA_HALF_HEIGHT),0.0f);

	
	//create the phyisics definition
	b2BodyDef bd;		
	bd.type =  b2_staticBody;
	bd.position = trans.p;
	bd.angle = trans.q.GetAngle();

	
	b2EdgeShape* shape = new  b2EdgeShape();
	shape->Set(b2Vec2(-1.0f * SHIELD_SIZE/2.0f,0.0f),b2Vec2(SHIELD_SIZE/2.0f, 0.0f));
	
	b2FixtureDef fd;		
	fd.shape = shape;
	/*
	fd.density = 1.0f;
	fd.friction = 0.02f;
	fd.restitution = 1.0f;
	*/
	fixtures.push_back(fd);


	//add the components to the object

	//transform
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);
	
	//box2d
	GBox2dComponent* bcomp = new GBox2dComponent();
	bcomp->SetOwner(object);
	bcomp->VInit(mWorld,bd,fixtures);
	object->AddComponent(bcomp);
	
	
	//add the controller component
	TShieldController* zcont = new TShieldController();
	
	GControllerComponent* ccomp = new GControllerComponent();
	ccomp->VInit(zcont);
	object->AddComponent(ccomp);
	ccomp->SetOwner(object);
	zcont->VInit(SHIELD_SIZE_TIME,SHIELD_PERIOD,TShieldController::ShieldPathFunction);

	object->PostInit(); //post initialize all components 
	zcont->SetTime(time);

	return object;

}





GObjectPtr TGameStateDemo::CreateDummyObject()
{
	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_OTHER,0);
	

	b2Transform trans;
	trans.Set(b2Vec2(0.0f,0.0f),0.0f);


	//create the scene node
	irr::video::SColor scolor = irr::video::SColor(255,255,255,255);
	
	SpriteNode* node = new SpriteNode(
				app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
				app->getIrrlichtDevice()->getSceneManager(),
				object->GetId());				
#if defined(_IRR_ANDROID_PLATEFORM_)
	node->Load("Figures.tga",128,128,0.3f*2.0f,0.3f*2.0f);
#else
	node->Load("IrrlichtSdcard/Figures.tga",128,128,0.3f*2.0f,0.3f*2.0f);
#endif
	node->setFrame(3);
	

	
	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);
	

	GIrrlichtComponent* ncomp = new GIrrlichtComponent();
	ncomp->SetOwner(object);
	ncomp->VInit(node);
	object->AddComponent(ncomp);

	object->PostInit(); //post initialize all components 

	return object;
}


//setup 
void TGameStateDemo::SetupFireworkPool(bool reinit)
{

	if(!reinit)
	{
		mFireworkPool.clear();

		for(int i=0; i<MAX_FIREWORKS; i++)
		{
			mFireworkPool.push_back(CreateFireworkNode());
		}
	}
	else
	{
		GGameApp* app = GGameApp::getInstance();

		//reinit the vbo
		FireworkNode::InitialzeVBO(64,64,app->getIrrlichtDevice()->getSceneManager());

		for(int i=0; i<MAX_FIREWORKS; i++)
		{
			GObjectPtr firework = mFireworkPool[i];
			GIrrlichtComponent* ircomp = firework->GetComponent<GIrrlichtComponent>(firework->GetComponentIdByIndex(GC_IRRNODE_IDX));
			FireworkNode* node = (FireworkNode*)ircomp->mNode;	
			node->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());			
			node->setMaterialType((video::E_MATERIAL_TYPE)FireWorkShader::gFireWorkMaterialType);	


		}
	
	}

}
void TGameStateDemo::CreateExplosionAt(b2Vec2 position, bool sound,int type , u32 color)
{


	if(sound)
	{
		AudioItem audioBlast;
		if(type==0)
			audioBlast.mChunkId = AudioManager::gChunkIndexes[AudioItem::CHUNK_BLAST1];
		else
			audioBlast.mChunkId = AudioManager::gChunkIndexes[AudioItem::CHUNK_BLAST2];

		audioBlast.mFlags = AudioItem::PLAYONCE;
		audioBlast.mLoop=0;				
		audioBlast.mId = GGameApp::getInstance()->getAudioManager()->playSoundNow(audioBlast);
	}

	for(int i=0; i<mFireworkPool.size(); i++)
	{
		GObjectPtr o = mFireworkPool[i];
		GControllerComponent* ccomp = o->GetComponent<GControllerComponent>(o->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TFireWorkController* zcont = (TFireWorkController*)ccomp->gameController;
	
		if(!zcont->IsActive())
		{	
			zcont->ExplodeAt(position, color);

			return;
		}
	}
}

GObjectPtr TGameStateDemo::CreateFireworkNode()
{

	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_FIREWORK,0);
	
	b2Transform trans;
	trans.Set(b2Vec2(0.0f,0.0f),0.0f);

	
	//create the scene node
	irr::video::SColor scolor = irr::video::SColor(255,255,255,255);
	
	
	
	FireworkNode* node = new FireworkNode(
				app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
				app->getIrrlichtDevice()->getSceneManager(),
				object->GetId());	

	
#if defined(_IRR_ANDROID_PLATEFORM_)	
	node->Load("fire.tga",64,64,1.0,1.0);
#else
	node->Load("IrrlichtSdcard/fire.tga",64,64,1.0,1.0);
	
#endif


	
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setMaterialType((video::E_MATERIAL_TYPE)FireWorkShader::gFireWorkMaterialType);	


	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);
	
	
	GIrrlichtComponent* ncomp = new GIrrlichtComponent();
	ncomp->SetOwner(object);
	ncomp->VInit(node);
	object->AddComponent(ncomp);
	

	//add the controller component
	//add the controller
	TFireWorkController* zcont = new TFireWorkController();
	  
	
	GControllerComponent* ccomp = new GControllerComponent();
	ccomp->VInit(zcont);
	object->AddComponent(ccomp);
	ccomp->SetOwner(object);
	zcont->VInit(1500, 0.15f); 
	 
	object->PostInit(); //post initialize all components 

	 
	return object;

}


GObjectPtr TGameStateDemo::CreateBarNode(GObjectPtr player)
{

	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_BAR,1);
	
	b2Transform trans;
	trans.Set(b2Vec2(0.0f ,6.2f),0.0f);

	
	//create the scene node
	//create the energy Bar
	
	EnergyBarNode* energyNode = new 
		EnergyBarNode(app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
					  app->getIrrlichtDevice()->getSceneManager(),
					  object->GetId(),
					CRASH_AREA_HALF_WIDTH*2.0f,0.2f,
					SColor(200,255,0,0), 
					SColor(200,0,255,0),
					SColor(255,64,0,128),1);

	#if defined(_IRR_ANDROID_PLATEFORM_)
	energyNode->Load("Figures.tga",128,128);
	#else
	energyNode->Load("IrrlichtSdcard/Figures.tga",128,128);
	#endif
	energyNode->setVisible(false);




	GIrrlichtComponent* ncomp = new GIrrlichtComponent();
	ncomp->SetOwner(object);
	ncomp->VInit(energyNode);
	object->AddComponent(ncomp);
	

	
	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);

	
	

	//add the controller component
	//add the controller
	TEnergyBarController* zcont = new TEnergyBarController();
	
	GControllerComponent* ccomp = new GControllerComponent();
	ccomp->VInit(zcont);
	object->AddComponent(ccomp);
	ccomp->SetOwner(object);
	zcont->VInit(player,0);

	object->PostInit(); //post initialize all components 

	return object;


}



GObjectPtr TGameStateDemo::CreateRadiationBarNode(GObjectPtr player)
{

	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_BAR,0);
	
	b2Transform trans;
	trans.Set(b2Vec2(0.0f ,6.25f),0.0f);

	
	
	//create the scene node
	//create the energy Bar
	
	EnergyBarNode* energyNode = new 
		EnergyBarNode(app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
					  app->getIrrlichtDevice()->getSceneManager(),
					  object->GetId(),
					CRASH_AREA_HALF_WIDTH*2.0f,0.5f,
					SColor(200,255,255,0), 
					SColor(200,255,0,0),
					SColor(255,0,0,0),1);

	#if defined(_IRR_ANDROID_PLATEFORM_)
	energyNode->Load("Figures.tga",128,128);
	#else
	energyNode->Load("IrrlichtSdcard/Figures.tga",128,128);
	#endif
	
	



	GIrrlichtComponent* ncomp = new GIrrlichtComponent();
	ncomp->SetOwner(object);
	ncomp->VInit(energyNode);
	object->AddComponent(ncomp);
	

	
	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);

	
	

	//add the controller component
	//add the controller
	TEnergyBarController* zcont = new TEnergyBarController();
	
	GControllerComponent* ccomp = new GControllerComponent();
	ccomp->VInit(zcont);
	object->AddComponent(ccomp);
	ccomp->SetOwner(object);
	zcont->VInit(player,1);

	object->PostInit(); //post initialize all components 

	return object;


}

TGameStateDemo::~TGameStateDemo(void)
{
	
}

void TGameStateDemo::SetupCreatorsPool(bool reinit)
{
	if(!reinit)
	{
		mCreatorsPool.clear();
		for(int i=0; i<MAX_CREATORS; i++)
		{
			mCreatorsPool.push_back(TElementsFactory::CreateCreator());
		}
	}
	else
	{
		GGameApp* app = GGameApp::getInstance();

		for(int i=0; i<MAX_CREATORS; i++)
		{
			GObjectPtr go = mCreatorsPool[i];

			if(go)
			{
				GIrrlichtComponent* ircomp = go->GetComponent<GIrrlichtComponent>(go->GetComponentIdByIndex(GC_IRRNODE_IDX));
				Base2dNode* node = (Base2dNode*)ircomp->mNode;	
				node->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());
			
			}

		}
	
	
	}

}


void TGameStateDemo::RefreshColorBars(irr::u32 ms)
{
	GObjectPtr objective = NULL;
	GIrrlichtComponent* irrComp= NULL;			
	GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
	TPlayerController* pcont = (TPlayerController*)ccomp->gameController;
	
	
	for(int i=0;i<5;i++)
	{
	
		objective = mColorBars[i];
		irrComp = objective->GetComponent<GIrrlichtComponent>(objective->GetComponentIdByIndex(GC_IRRNODE_IDX));
		EnergyBarNode* energyNode = (EnergyBarNode*)irrComp->mNode;
		float value = 0.0f; 

		if(i<mLevelValues.maxColors)
		{

			if(mLevelValues.colorCount>0)
			{
				switch(i)
				{
					case 0:
						value = (float)pcont->mCountA/(float)mLevelValues.colorCount;
						break;
					case 1:
						value = (float)pcont->mCountB/(float)mLevelValues.colorCount;
						break;
					case 2:
						value = (float)pcont->mCountC/(float)mLevelValues.colorCount;
						break;
					case 3:
						value = (float)pcont->mCountD/(float)mLevelValues.colorCount;
						break;
					case 4:
						value = (float)pcont->mCountE/(float)mLevelValues.colorCount;
						break;
				}
			}

			value = std::min(1.0f,value);		
			energyNode->setEnergyValue(value);		
			objective->Update(ms);
			energyNode->setVisible(true);
		}
		else
		{
			energyNode->setVisible(false);
		}

	}

	/*if(mMoleIndicatorKind!=NULL)
	{
		irrComp = mMoleIndicatorKind->GetComponent<GIrrlichtComponent>(mMoleIndicatorKind->GetComponentIdByIndex(GC_IRRNODE_IDX));;			
		SpriteNode* spriteNode = (SpriteNode*)irrComp->mNode;

		switch(mLevelValues.moleSize)
		{
			case 1:
				spriteNode->setFrame(0);
				break;
			case 2:
				spriteNode->setFrame(1);
				break;
			case 3:
				spriteNode->setFrame(2);
				break;
		}
	}*/
	/*
	if(mMoleIndicatorCount!=NULL)
	{
		irrComp = mMoleIndicatorCount->GetComponent<GIrrlichtComponent>(mMoleIndicatorCount->GetComponentIdByIndex(GC_IRRNODE_IDX));;			
		SpriteNode* spriteNode = (SpriteNode*)irrComp->mNode;

		if(mLevelValues.moleCount == 3)
		{
			spriteNode->setFrame(3);
		}
		else
		{
			spriteNode->setFrame(4);
		}
	}
	*/
	//mMoleIndicatorKind->Update(ms);
	//mMoleIndicatorCount->Update(ms);

	//


	/*if(mGoalManager.hasGoal())
	{
		CrashGoal& crashGoal = mGoalManager.CurrentGoal();
	
		for(int i=0;i<5;i++)
		{
			if(crashGoal.colors[i]>0)
				totalObjects++;

		}



		if(pcont->getCrashState() == CRASH_STATE_CANCRASH)
		{
				
			for (irr::u32 i = 0; i < mElementList.size(); i++)
			{
				GObjectPtr element =  mElementList[i];
				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;
			
				if(econt->getCrashState() == CRASH_STATE_CANCRASH)
				{
					isCrashing=true;
					int colorId = econt->mColorId;
					if(colorId>=0  && colorId<=4)
					{
						currentCrash.colors[colorId]= currentCrash.colors[colorId]+1;
					}
				}

			}
		}



		sx = 0.0f -  (totalObjects*1.0f)/2.0f;
		
		int step =  (mElapsedTime%1000);
		step = step / 25; 
		step = step%3;

		for(int i=0; i<5;i++)
		{

			int colorId = i;
			int qty = crashGoal.colors[i];
			int totalObjects = 0;

			//get the sprite node
			//count total # of colors 

			if(qty>0 && qty<=9)
			{
				objective = mObjectivesPool[j];
				spcomp = objective->GetComponent<GSpriteComponent>(objective->GetComponentIdByIndex(GC_SPRITE_IDX));			
				GTransformComponent* trans = objective->GetComponent<GTransformComponent>(objective->GetComponentIdByIndex(GC_TRANSFORM_IDX));
				qty=qty-1;

				float x = (qty % 3)*128;
				float y = (qty / 3)*128;
				float x2= x+128;
				float y2= y+128;

				spcomp->mUV1.set(x,y);
				spcomp->mUV2.set(x2,y2);
				spcomp->mVisible=true;

				trans->mTrans.p.Set(sx,sy);
				sx+=1.0f;


			
				//set color
				switch(i)
				{
					case 1:
						{												 
							spcomp->mColor= irr::video::SColor(255,255,1,169);					
						}
					break;
					case 2:
						{
							spcomp->mColor = irr::video::SColor(255,139,8,232);					
						}
					break;	
					case 3:
						{					
							spcomp->mColor = irr::video::SColor(255,10,51,255);
						}
					break;	
					case 4:
						{
					
							spcomp->mColor = irr::video::SColor(255,23,196,232);
						}
					break;	
					default:
						{
					
							spcomp->mColor = irr::video::SColor(255,18,255,131);
						}
					break;
				}

			
				if(isCrashing && currentCrash.colors[i] >= crashGoal.colors[i] )
				{
					if(step==0)
					{
					
						spcomp->mColor = irr::video::SColor(255,255,255,0);	
					}
					else if(step==1)
					{
						spcomp->mColor = irr::video::SColor(255,255,0,255);	
					
					}
					else if(step==2)
					{
						spcomp->mColor = irr::video::SColor(255,255,0,0);						
					}		
				}


				j++;
			}
		
		}
	}
	*/

}

void TGameStateDemo::SetupColorBars(bool reinit)
{

	if(!reinit)
	{
		mColorBars.clear();

		float height =  0.8f;
		float width = 2.0f;

		float x = -2.0f;
		float y =7.5f;
		irr::video::SColor color;

		for(int i=0; i<5; i++)
		{
			switch(i)
			{
				case 0:
					{
						color.set(255,18,255,131);
					}
				break;
				case 1:
					{
						color.set(255,255,1,169);
					}
				break;
				case 2:
					{
						color.set(255,139,8,232);
					}
				break;	
				case 3:
					{
						color.set(255,10,51,255);
					}
				break;	
				case 4:
					{
						color.set(255,23,196,232);
					}			
				break;
			}

			mColorBars.push_back(TElementsFactory::CreateColorBar(color,x,y,width,height,i));
			x+=0.8f;
		}
	}
	else
	{
		GGameApp* app = GGameApp::getInstance();

		for(int i=0; i<5; i++)
		{	
			GObjectPtr bar = mColorBars[i];

			if(bar)
			{
				GIrrlichtComponent* ircomp = bar->GetComponent<GIrrlichtComponent>(bar->GetComponentIdByIndex(GC_IRRNODE_IDX));
				Base2dNode* node = (Base2dNode*)ircomp->mNode;	

				node->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());

			}
	
		}
	}
}


void TGameStateDemo::OnSetScript(OOLUA::Script *script)
{
	script->register_class(this);
}

void TGameStateDemo::OnStartScript()
{

}


void TGameStateDemo::OnScriptFinished()
{
	mScriptState = (int)GScriptable::FINALIZED;

}
	


void TGameStateDemo::VRender(IrrlichtDevice* device)
{
	GIrrlichtComponent* ccomp = mPlayer->GetComponent<GIrrlichtComponent>(mPlayer->GetComponentIdByIndex(GC_IRRNODE_IDX));
	
	ISceneNode* node =  GGameApp::getInstance()->getIrrlichtDevice()->getSceneManager()->getSceneNodeFromId(mTestNodeId);
	ISceneNode* elementsNode =  GGameApp::getInstance()->getIrrlichtDevice()->getSceneManager()->getSceneNodeFromId(mBatchedSpriteNodeId);
	ISceneNode* jointsNode =  GGameApp::getInstance()->getIrrlichtDevice()->getSceneManager()->getSceneNodeFromId(mJointsNodeId);
	SpriteNode* playerNode = (SpriteNode*)ccomp->mNode;

	//ISceneNode* backNode = (ISceneNode*)GGameApp::getInstance()->getIrrlichtDevice()->getSceneManager()->getSceneNodeFromId(mStarBackground->GetId());

	
	SColor playerColor = playerNode->mVertexColor;
		
	
	if (renderTarget1 )
	{	
		elementsNode->setVisible(true);
		playerNode->setVisible(false);
		jointsNode->setVisible(true);
		//backNode->setVisible(true);
		
		elementsNode->setMaterialType((video::E_MATERIAL_TYPE)MetaballShaderPre::gMetaballMaterialType);	
		elementsNode->getMaterial(0).setTexture(0, alphaTexture);
		elementsNode->getMaterial(0).setTexture(1, metaTexture);		
		
		
		for (irr::u32 i = 0; i < mElementList.size(); i++)
		{
			GObjectPtr element =  mElementList[i];
			GSpriteComponent* spcomp = element->GetComponent<GSpriteComponent>(element->GetComponentIdByIndex(GC_SPRITE_IDX));
			GTransformComponent* tcomp = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));

			spcomp->mColor2=  spcomp->mColor;
			//tcomp->mTrans.Set(tcomp->mTrans.p , 0.0f);


			spcomp->mColor =  irr::video::SColor(255,255,255,255);
		}
		
		
		jointsNode->setMaterialType((video::E_MATERIAL_TYPE)MetaballShaderPre::gMetaballMaterialType);	
		jointsNode->getMaterial(0).setTexture(0, alphaTexture);
		jointsNode->getMaterial(0).setTexture(1, metaTexture);		
		

		playerNode->setMaterialType((video::E_MATERIAL_TYPE)MetaballShaderPre::gMetaballMaterialType);	
		playerNode->getMaterial(0).setTexture(0, alphaTexture);
		playerNode->getMaterial(0).setTexture(1, metaTexture);		
		irr::video::SColor color(255,255,255,255);
		playerNode->SetVertexColor(color);

		device->getVideoDriver()->setRenderTarget(renderTarget2, true, true, video::SColor(0,0,0,0));

		mTargetCamera->render();

		jointsNode->render();	
		elementsNode->render();	
		playerNode->updateAbsolutePosition();
		
		//render elements using color texture normal shader 
		elementsNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);	
		elementsNode->getMaterial(0).setTexture(0, colorTexture);
		elementsNode->getMaterial(0).setTexture(1, NULL);		
	
		//render elements using color texture normal shader
		for (irr::u32 i = 0; i < mElementList.size(); i++)
		{
			GObjectPtr element =  mElementList[i];
			GSpriteComponent* spcomp = element->GetComponent<GSpriteComponent>(element->GetComponentIdByIndex(GC_SPRITE_IDX));			
			spcomp->mColor =  spcomp->mColor2;
		}

		jointsNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);	
		jointsNode->getMaterial(0).setTexture(0, colorTexture);
		jointsNode->getMaterial(0).setTexture(1, NULL);		
		

		playerNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);			
		playerNode->getMaterial(0).setTexture(0, colorTexture);
		playerNode->getMaterial(0).setTexture(1, NULL);		
		playerNode->SetVertexColor(playerColor);

		device->getVideoDriver()->setRenderTarget(renderTarget1, true, true, video::SColor(0,0,0,0));	
		
		mTargetCamera->render();		
		jointsNode->render();	
		elementsNode->render();	
		
		device->getVideoDriver()->setRenderTarget(0, true, true, 0);				
		
		//render elements using color texture + render target  and metaball shader
		node->setMaterialType((video::E_MATERIAL_TYPE)MetaballShader::gMetaballMaterialType);			
		node->getMaterial(0).setTexture(0, renderTarget1); //color texture
		node->getMaterial(0).setTexture(1, renderTarget2); //meta texture
		
		playerNode->setVisible(false);
		jointsNode->setVisible(false);
		elementsNode->setVisible(false);	
		
	}
	

	//node->setVisible(false);

	//node->render();
	device->getSceneManager()->setActiveCamera(mCamera);
	device->getSceneManager()->drawAll();				
}



void TGameStateDemo::CreateElementAtRandXY(int element)
{	
	int retries = 3;

	while(retries >0)
	{
	
		int randX = ((rand()%24)*5)-60;
		int randY = ((rand()%16)*5)-40;
		float x = ((float)randX)/10.0f;
		float y = ((float)randY)/10.0f;
		bool skip = false;

		//check for all elements
		for (irr::u32 i = 0; i < mElementList.size(); i++)
		{
			GObjectPtr element =  mElementList[i];
			GTransformComponent* tcomp = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		
			b2Vec2 dif = b2Vec2(x,y) - tcomp->mTrans.p;

			if(dif.Length()<0.5f)
			{
				skip=true;
				break;
			}
		}
			
		if(!skip)
		{	
			CreateElementAtXY(randX,randY,element);
			break;
		}
		retries--;
	}

}


void TGameStateDemo::CreateElementDistributed(int count)
{
	CreateElementDistributedInternal(count,mLevelValues.maxObjects);


	for(int i=0; i< mLevelValues.maxBad;i++)
	{
	
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* pcont = (TPlayerController*)ccomp->gameController;

		//calculate percent of completion
		int crashed = std::min(pcont->mCountA,mLevelValues.colorCount)+ 
					  std::min(pcont->mCountB,mLevelValues.colorCount)+ 
					  std::min(pcont->mCountC,mLevelValues.colorCount)+
					  std::min(pcont->mCountD,mLevelValues.colorCount)+
					  std::min(pcont->mCountE,mLevelValues.colorCount);
		int expected = mLevelValues.colorCount*mLevelValues.maxColors;

		if(expected>0.0f)
		{
			float percent = (float)crashed / (float)expected;

			if(percent > 0.9f)
			{
				mWorld->SetGravity(b2Vec2(0.0f, -3.4f));	 //unset gravity
			}
			else if(percent > 0.7f)
			{
				mWorld->SetGravity(b2Vec2(0.0f, -3.0f));	 //unset gravity
			}
			else if(percent > 0.4f)
			{
				mWorld->SetGravity(b2Vec2(0.0f, -2.5f));	 //unset gravity
			}

			//if player has done 60% then launch bad objects
			if(percent > 0.7f)
			{
				
				int x=  (rand()%6)*10 - 30;
				int y = 80;
				float fx = (float)x /10.0f;
				float fy = (float)y /10.0f;

				CreateElementAtXYInternal
									(4,
									  0, 
									  fx,
									  fy,
									false);
				
			}
	

		}
	}

	if(!(mGoalManager.GoalState() == GOAL_STATE_CRASHABLE))
	{
		if(mGoalManager.GoalState() == GOAL_STATE_NOT_FOUND)
		{
			int r= rand()%2;			
			CreateElementDistributedInternal(r, mElementList.size()+2);
			mGoalManager.startMonitoringScene(mLevelValues);
			
		}
		else if(mGoalManager.GoalState() == GOAL_STATE_NONE)
		{
			mGoalManager.startMonitoringScene(mLevelValues);
		}

	}

	/*
	if(!mGoalManager.hasGoal())
	{
		int retry= 2;

		while(!mHasObjective && retry>0)
		{
			int r= rand()%2;

			CreateElementDistributedInternal(r,15);
			DoLogicFindGoal();
			retry--;
		}
	}
	*/
}
 

void TGameStateDemo::CreateElementDistributedInternal(int count, int lmax)
{

	PositionInfo* positions[12];
	int  colors[12];
	int i=0;
	int duos = 3;

	int energies = 0;

	for (irr::u32 j = 0;j < mElementList.size(); j++)
	{
		GObjectPtr element =  mElementList[j];
		
		if(element->GetSubType()== T_OS_ELEMENT_ENERGY)
			energies++;
	}

	

	if(count==2 || count==3)
	{
		if(mLevelValues.maxLaunch>=3)
		{
			 int rw= (int)CRASH_AREA_HALF_WIDTH;
			 int rh= (int)CRASH_AREA_HALF_HEIGHT;
			 int x= (rand()%rw)*10 - 30;
			 int y= 80 +   ((rand()%rh)*2 + 20);
			 int angle = (rand()%9) * 45;	
		
			 if( (energies+ 3) > lmax)
				return;


			if(count==2)
			{
				CreateTri(x,y,6,angle, 0x0f,0x0f,0x0f);
			}
			else
			if(count==3)
			{
				CreateTri2(x,y,9,angle, 0x0f,0x0f,0x0f);
			}

			return;
		}
		else
		{
			count=count%2;
		}
	}
		
	if(mLevelValues.maxLaunch>=2)
	{
		if(count==1)
		{
			duos =2;
			count = 5;
		}
		else
		{
			duos =1;
			count = 3;
		}
	}
	else
	{
		if(count==1)
		{
			duos=0;
			count = 3;
		}
		else
		{
			duos=0;
			count = 2;
		}
	
	}

	if( (energies+ count) > lmax)
		return;



	int index[12];
	bool duoEnabled[6];
	bool enabled[12];
	
	for(i=0;i<12;i++)
	{
		index[i] = i;
		enabled[i]= false;
	}
	for(i=0;i<6;i++)
	{
		duoEnabled[i] = false;		
	}

	int required = count;
	

	/*required = 20  - energies;

	if(required<0)
		required = 0;


	required = std::min(12, required); /// - std::min(required,energies);
	*/

	std::random_shuffle(&index[0],&index[11]);

	for(i=0;i<required;i++)
		enabled[i]  = true;
	
	for(i=0;i<duos;i++)
		duoEnabled[i]  = true;
	

	//create elements
	for(i=0;i<12;i++)
		positions[i] = new PositionInfo();

	i=0;
	positions[i]->position.x = -4.0f;
	positions[i]->position.y =  9.0f;
	positions[i]->fromR = 27;
	positions[i]->toR = 27+9;

	i=1;
	positions[i]->position.x =  -4.0f;
	positions[i]->position.y =  8.8f;
	positions[i]->fromR = 18;
	positions[i]->toR = 18+18;

	i=2;
	positions[i]->position.x =  4.0f;
	positions[i]->position.y =  8.6f;
	positions[i]->fromR = 18;
	positions[i]->toR = 18+18;

	i=3;
	positions[i]->position.x =  4.0f;
	positions[i]->position.y =  8.4f;
	positions[i]->fromR = 18;
	positions[i]->toR = 18+9;




	i=4;
	positions[i]->position.x =  0.0f;
	positions[i]->position.y =  8.2f;
	positions[i]->fromR = 27;
	positions[i]->toR = 27+18;

	i=5;
	positions[i]->position.x =  0.0f;
	positions[i]->position.y =  8.0f;
	positions[i]->fromR = 0;
	positions[i]->toR = 36;

	i=6;
	positions[i]->position.x =  0.0f;
	positions[i]->position.y =  8.0f;
	positions[i]->fromR = 0;
	positions[i]->toR = 36;
	i=7;
	positions[i]->position.x =  0.0f;
	positions[i]->position.y =  8.2f;	
	positions[i]->fromR = 9;
	positions[i]->toR = 9+18;



	i=8;
	positions[i]->position.x = -4.0f;
	positions[i]->position.y =  8.4f;
	positions[i]->fromR = 0;
	positions[i]->toR = 9;

	i=9;
	positions[i]->position.x =  -4.0f;
	positions[i]->position.y =   8.6f;
	positions[i]->fromR = 0;
	positions[i]->toR = 18;

	i=10;
	positions[i]->position.x =  4.0f;
	positions[i]->position.y =  8.8f;
	positions[i]->fromR = 0;
	positions[i]->toR = 18;

	i=11;
	positions[i]->position.x =  4.0f;
	positions[i]->position.y =  9.0f;
	positions[i]->fromR = 9;
	positions[i]->toR = 9+9;

	for(i=0;i<12;i++)
	{
		positions[i]->position.y = positions[i]->position.y - 1.0f;
	}
	//randomize position
	for(i=0;i<12;i++)
	{
		bool valid = false;
		int retries =3;
		
		float x = positions[i]->position.x;
		float y = positions[i]->position.y;


		while((retries > 0) && !valid)
		{
			float angle  = (float) (positions[i]->fromR + std::rand()%(positions[i]->toR-positions[i]->fromR));
			angle = angle * 10.0f * 3.1416f / 180.0f; 
			float len = (float) (std::rand()%10);
			len = (len*1.0f) / 10.0f;

			x = positions[i]->position.x + cos(angle)*len;
			y = positions[i]->position.y + sin(angle)*len;
			valid = true;

			for(int j=0;j<12;j++)	
			{
				if(i!=j)
				{
					b2Vec2 dif = b2Vec2(x,y) -  positions[j]->position;

					if(dif.Length()<2.0f)
					{
						valid = false;
						break;
					}
				}
			}

			for (irr::u32 j = 0;j < mElementList.size(); j++)
			{
				GObjectPtr element =  mElementList[j];
				GTransformComponent* tcomp = element->GetComponent<GTransformComponent>(element->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		
				b2Vec2 dif = b2Vec2(x,y) - tcomp->mTrans.p;

				if(dif.Length()<0.5f)
				{
					valid=false;
					break;
				}
			}

			retries--;
		}

		positions[i]->position.x =x;
		positions[i]->position.y =y;
		

	}
	

	
	for(i=0;i<12;i++)
	{
		positions[i]->color =   std::rand()%mLevelValues.maxColors;
	}
	
	for(int j=0;j<12;j++)
	{
		i = index[j];

		if(enabled[j])
		{
			int j2= j/2;

			if((j%2==0) && duoEnabled[j2])
			{
					int angle = (rand()%18)*10;			
					int i2 = index[j2*2+1];
					enabled[j2*2+1] = false;
					 
					int color1 = positions[i]->color;
					int color2 = color1;
					int maxcolr = std::rand()%mLevelValues.maxColors-1;

					int rc2= 0;
					
					if(maxcolr>1)
						rc2 = rand()%maxcolr;

					if(color1==0)
					{
							switch(rc2)
							{
							case 0: color2 = 1;break;
							case 1: color2 = 2;break;
							case 2: color2 = 3;break;
							case 3: color2 = 4;break;
							}
					
					}
					else if(color1==1)
					{
							switch(rc2)
							{
							case 0: color2 = 0;break;
							case 1: color2 = 2;break;
							case 2: color2 = 3;break;
							case 3: color2 = 4;break;
							}
					
					}
					else if(color1==2)
					{
							switch(rc2)
							{
							case 0: color2 = 0;break;
							case 1: color2 = 1;break;
							case 2: color2 = 3;break;
							case 3: color2 = 4;break;
							}
					}
					else if(color1==3)
					{
							switch(rc2)
							{
							case 0: color2 = 0;break;
							case 1: color2 = 1;break;
							case 2: color2 = 2;break;
							case 3: color2 = 4;break;
							}
					
					}
					else if(color1==4)
					{
							switch(rc2)
							{
							case 0: color2 = 0;break;
							case 1: color2 = 1;break;
							case 2: color2 = 2;break;
							case 3: color2 = 3;break;
							}
					}

					CreateDuo(
						positions[i]->position.x*10.0f,
						positions[i]->position.y*10.0f ,
						DUO_WIDTH,
						angle,
						color1, 
						color2);


			}
			else
			{
				CreateElementAtXYInternal
						(0x00, 
						positions[i]->color, 
						positions[i]->position.x,
						positions[i]->position.y, 
						false);
			}
		}
	}

	for(i=0;i<12;i++)
		delete positions[i];


}

GObjectPtr TGameStateDemo::CreateBottomShield()
{
	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_SQUARESHIELD,0);
	
	GBox2dComponent::FixtureDefinitionsList fixtures;	
	b2Transform trans;
	trans.Set(b2Vec2(0.0f,-1.0f),0.0f);

	//create the phyisics definition
	b2BodyDef groundBodyDef;
	groundBodyDef.type =  b2_staticBody;
	groundBodyDef.angle = trans.q.GetAngle();
    groundBodyDef.position = trans.p;
	
	b2EdgeShape groundEdge1;
    b2FixtureDef boxShapeDef1;
	b2EdgeShape groundEdge2;
    b2FixtureDef boxShapeDef2;
    boxShapeDef1.shape = &groundEdge1;
	boxShapeDef2.shape = &groundEdge2;

	groundEdge1.Set(b2Vec2(-CRASH_AREA_HALF_WIDTH,-CRASH_AREA_HALF_HEIGHT), b2Vec2(CRASH_AREA_HALF_WIDTH, -CRASH_AREA_HALF_HEIGHT));    
    groundEdge2.Set(b2Vec2(-CRASH_AREA_HALF_WIDTH,CRASH_AREA_HALF_HEIGHT), b2Vec2(CRASH_AREA_HALF_WIDTH,CRASH_AREA_HALF_HEIGHT));
    

	fixtures.push_back(boxShapeDef1);
	fixtures.push_back(boxShapeDef2);

	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);
	

	GBox2dComponent* bcomp = new GBox2dComponent();
	bcomp->SetOwner(object);
	bcomp->VInit(mWorld,groundBodyDef,fixtures);
	object->AddComponent(bcomp);
	
	object->PostInit(); //post initialize all components 

	return object;
}

GObjectPtr TGameStateDemo::CreateSquareShield()
{	
	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_SQUARESHIELD,0);
	
	GBox2dComponent::FixtureDefinitionsList fixtures;	
	b2Transform trans;
	trans.Set(b2Vec2(0.0f,-1.0f),0.0f);

	//create the phyisics definition
	b2BodyDef groundBodyDef;
	groundBodyDef.type =  b2_staticBody;
	groundBodyDef.angle = trans.q.GetAngle();
    groundBodyDef.position = trans.p;
	
	b2EdgeShape groundEdge1;
    b2FixtureDef boxShapeDef1;
	b2EdgeShape groundEdge2;
    b2FixtureDef boxShapeDef2;
	b2EdgeShape groundEdge3;
    b2FixtureDef boxShapeDef3;
	b2EdgeShape groundEdge4;
    b2FixtureDef boxShapeDef4;

    boxShapeDef1.shape = &groundEdge1;
	boxShapeDef2.shape = &groundEdge2;
	boxShapeDef3.shape = &groundEdge3;
	boxShapeDef4.shape = &groundEdge4;

    groundEdge1.Set(b2Vec2(-CRASH_AREA_HALF_WIDTH,-CRASH_AREA_HALF_HEIGHT*2.0f), b2Vec2(-CRASH_AREA_HALF_WIDTH,CRASH_AREA_HALF_HEIGHT));    

    groundEdge2.Set(b2Vec2(CRASH_AREA_HALF_WIDTH,CRASH_AREA_HALF_HEIGHT), b2Vec2(CRASH_AREA_HALF_WIDTH,-CRASH_AREA_HALF_HEIGHT*2.0f));    

	//groundEdge3.Set(b2Vec2(-CRASH_AREA_HALF_WIDTH,-CRASH_AREA_HALF_HEIGHT*2.0f), b2Vec2(CRASH_AREA_HALF_WIDTH, -CRASH_AREA_HALF_HEIGHT*2.0f));    

	groundEdge4.Set(b2Vec2(-CRASH_AREA_HALF_WIDTH,CRASH_AREA_HALF_HEIGHT*3.0f), b2Vec2(CRASH_AREA_HALF_WIDTH,CRASH_AREA_HALF_HEIGHT*3.0f));
    
	fixtures.push_back(boxShapeDef1);
	fixtures.push_back(boxShapeDef2);
	//fixtures.push_back(boxShapeDef3);
	fixtures.push_back(boxShapeDef4);

	
	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);
	

	GBox2dComponent* bcomp = new GBox2dComponent();
	bcomp->SetOwner(object);
	bcomp->VInit(mWorld,groundBodyDef,fixtures);
	object->AddComponent(bcomp);
	
	object->PostInit(); //post initialize all components 

	return object;



}



/*
int TGameStateDemo::RefreshCrasheableMoles()
{
	int inAreaCount = 0;
	GObjectPtr objectsInArea[18];
	GObjectPtr chain3[3];
	
	bool trio = false;	
	bool duo = false;
	int differentCount = 0;


	//clear the moles
	mMoles[0].count=0;
	mMoles[0].colors[0]=-1;
	mMoles[0].colors[1]=-1;
	mMoles[0].colors[2]=-1;
	mMoles[0].atoms[0]=NULL;
	mMoles[0].atoms[1]=NULL;
	mMoles[0].atoms[2]=NULL;
	mMoles[1].count=0;
	mMoles[1].colors[0]=-1;
	mMoles[1].colors[1]=-1;
	mMoles[1].colors[2]=-1;
	mMoles[1].atoms[0]=NULL;
	mMoles[1].atoms[1]=NULL;
	mMoles[1].atoms[2]=NULL;
	mMoles[2].count=0;
	mMoles[2].colors[0]=-1;
	mMoles[2].colors[1]=-1;
	mMoles[2].colors[2]=-1;
	mMoles[2].atoms[0]=NULL;
	mMoles[2].atoms[1]=NULL;
	mMoles[2].atoms[2]=NULL;

	//get all the objects in the collide region
	inAreaCount=0;
	for (irr::u32 i = 0; i < mElementList.size(); i++)
	{
		GObjectPtr element =  mElementList[i];
		GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TElementController* econt = (TElementController*)ccomp->gameController;					
		if(econt->getInCrashForce() )
		{
			econt->mCounted=false;

			if(inAreaCount<=18)
			{
				objectsInArea[inAreaCount] = element;							
				inAreaCount++;
			}
		}
	}


	//no estar to grab the diferent moles using the objects in area



	//check if there is a 3 chain
	
	for(int i=0;i<inAreaCount;i++)
	{
		GControllerComponent* ccomp = objectsInArea[i]->GetComponent<GControllerComponent>(objectsInArea[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		GBox2dComponent* box2d = objectsInArea[i]->GetComponent<GBox2dComponent>(objectsInArea[i]->GetComponentIdByIndex(GC_BOX2D_IDX));
		TElementController* econt = (TElementController*)ccomp->gameController;		
		trio = false;	
		duo = false;	
		if(!econt->mCounted)
		{
			econt->mCounted =true;

			//get the box2d component
			b2JointEdge* edge = box2d->mBody->GetJointList();

			chain3[0]=objectsInArea[i];
			chain3[1]=NULL;
			chain3[2]=NULL;

			if(edge!=NULL && edge->other!=NULL) //this is at leat a duo
			{	
				if(edge->other->GetUserData() != NULL)
				{

					chain3[1]=(GObjectPtr)edge->other->GetUserData();
					GControllerComponent* duocomp = chain3[1]->GetComponent<GControllerComponent>(chain3[1]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					GBox2dComponent* duobox2d = chain3[1]->GetComponent<GBox2dComponent>(chain3[1]->GetComponentIdByIndex(GC_BOX2D_IDX));
					TElementController* duocont = (TElementController*)duocomp->gameController;		
				
					duocont->mCounted = true;
					duo = true;
					edge = edge->next;

					//this has 2 edges , then it is a trio
					if(edge && edge->other!=NULL)
					{
						if(edge->other->GetUserData()!=NULL)
						{	chain3[2]=(GObjectPtr)edge->other->GetUserData();
							trio=true;
							duo = false;
						}
					}
					else
					{
						GControllerComponent* ccomp2 = chain3[1]->GetComponent<GControllerComponent>(chain3[1]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						GBox2dComponent* box2d2 = chain3[1]->GetComponent<GBox2dComponent>(chain3[1]->GetComponentIdByIndex(GC_BOX2D_IDX));
						TElementController* econt2 = (TElementController*)ccomp2->gameController;		
						b2JointEdge* edge2 = box2d2->mBody->GetJointList();
						//check if other body has edge then it is a trio
						if(edge2 && edge2->other!=NULL)
						{
							if(edge2->other->GetUserData() == chain3[0])
								edge2 = edge2->next;

							if(edge2 && edge2->other!=NULL)
							{

								if(edge2->other->GetUserData()!=NULL)
								{
									chain3[2]=(GObjectPtr)edge2->other->GetUserData();
									duo = false;
									trio=true;
								}
							}
						}
					}

					if(trio)
					{

						GControllerComponent* tricomp = chain3[2]->GetComponent<GControllerComponent>(chain3[2]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						TElementController* tricont = (TElementController*)tricomp->gameController;		
						tricont->mCounted = true;
					}
				}

			}


			if(differentCount<=2)
			{
				if(duo)
				{
					mMoles[differentCount].count=2;
					mMoles[differentCount].colors[0]= ((TElementController*)CanCollideQuery::GetElementController(chain3[0]))->mColorId;;
					mMoles[differentCount].colors[1]= ((TElementController*)CanCollideQuery::GetElementController(chain3[1]))->mColorId;;					
					mMoles[differentCount].atoms[0]=chain3[0];
					mMoles[differentCount].atoms[1]=chain3[1];					

				}
				else if(trio)
				{
					mMoles[differentCount].count=3;
					mMoles[differentCount].colors[0]= ((TElementController*)CanCollideQuery::GetElementController(chain3[0]))->mColorId;
					mMoles[differentCount].colors[1]= ((TElementController*)CanCollideQuery::GetElementController(chain3[1]))->mColorId;
					mMoles[differentCount].colors[2]= ((TElementController*)CanCollideQuery::GetElementController(chain3[2]))->mColorId;
					mMoles[differentCount].atoms[0]=chain3[0];
					mMoles[differentCount].atoms[1]=chain3[1];
					mMoles[differentCount].atoms[2]=chain3[2];
				}
				else
				{
					mMoles[differentCount].count=1;
					mMoles[differentCount].colors[0]= ((TElementController*)CanCollideQuery::GetElementController(chain3[0]))->mColorId;;
					mMoles[differentCount].atoms[0]=chain3[0];
					

				}
			}

			differentCount++;
			
		}
	}

	

	return differentCount;


}

*/
int TGameStateDemo::CalculateLevel()
{
	int level =0;

	
	
	if(mPlayer)
	{
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* pcont = (TPlayerController*)ccomp->gameController;

		int countA = std::min(pcont->mCountA,mLevelValues.colorCount);
		int countB = std::min(pcont->mCountB,mLevelValues.colorCount); 
		int countC = std::min(pcont->mCountC,mLevelValues.colorCount);
		int countD = std::min(pcont->mCountD,mLevelValues.colorCount);
		int countE = std::min(pcont->mCountE,mLevelValues.colorCount);

		int crashed = countA+ 
					  countB+ 
					  countC+
					  countD+
					  countE;
		int expected = mLevelValues.maxColors *mLevelValues.colorCount;



		level= pcont->mLevel;

		if(crashed>= expected)
		{
			level++;
			mDoNextLevelAnimation=true;
			mNextLevelTimer=0;
			mWorld->SetGravity(b2Vec2(0.0f, -2.1f));	 //unset gravity

			AudioItem audioUp;
			audioUp.mChunkId = AudioManager::gChunkIndexes[AudioItem::CHUNK_LEVELUP];
			audioUp.mFlags = AudioItem::PLAYONCE;
			audioUp.mLoop=0;				
			audioUp.mId = GGameApp::getInstance()->getAudioManager()->playSound(audioUp);

			//clear bad elements
			int hasBad = 0;
			/*
			for (irr::u32 i = 0; i < mElementList.size(); i++)
			{
				if (mElementList[i]->GetSubType() == T_OS_ELEMENT_BAD1)
				{
					GControllerComponent* ccomp = mElementList[i]->GetComponent<GControllerComponent>(mElementList[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));					
					TElementController* econt = (TElementController*)ccomp->gameController;					
					econt->setCrashState(CRASH_STATE_CRASHED);			
					hasBad++;
				}
			}

			if(hasBad>0)
				ProcessCrashedObjects();
			*/
			//ClearElements();
			mGoalManager.startMonitoringScene(mLevelValues);
	
			//increase player energy
			pcont->mEnergy = 1.0f;

			pcont->mCountA=0;
			pcont->mCountB=0;
			pcont->mCountC=0;
			pcont->mCountD=0;
			pcont->mCountE=0;

			pcont->setLevel(level);

			pcont->clearMoles();
			pcont->setCrashState(CRASH_STATE_NONE);
			pcont->setMoleState(MOLE_STATE_NONE);


		}

		

	}

	return level;
}


void TGameStateDemo::SetLevelValues(
		int maxJoin,		
		int maxObjects,
		int maxLaunch,
		int maxBad,
		int colorCount,
		int moleSize,
		int maxColors
		)
{
	mLevelValues.maxJoin = maxJoin;
	mLevelValues.launchSpeed = 0;
	mLevelValues.maxObjects = maxObjects;
	mLevelValues.maxLaunch = maxLaunch;
	mLevelValues.maxBad = maxBad;
	mLevelValues.colorCount = colorCount;
	mLevelValues.moleSize = moleSize;
	mLevelValues.maxColors = maxColors;

	mGoalManager.startMonitoringScene(mLevelValues);
	

	
	if(mPlayer)
	{
		
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* pcont = (TPlayerController*)ccomp->gameController;




		pcont->mMaxJoin = maxJoin;
		pcont->mColorCount = colorCount;
	}
}




void TGameStateDemo::VOnGetFocus(int param)
{
	GGameState::VOnGetFocus(param);

	GGameApp* app = GGameApp::getInstance();

	if(mPlayer)
	{
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* pcont = (TPlayerController*)ccomp->gameController;		
		pcont->mLevelChanged=true;
		pcont->mScoreChanged=true;
	}

	#if defined(_IRR_ANDROID_PLATEFORM_)
	//load the menu GUI
	if (app->getRocketGUI()->LoadDocument("game.rml"))
	{	
		app->getRocketGUI()->SetDocumentVisible("game_window",true);		
	}
	#else	
	//load the menu GUI
	if (app->getRocketGUI()->LoadDocument("IrrlichtSdcard/game.rml"))
	{	
		app->getRocketGUI()->SetDocumentVisible("game_window",true);		
	}
	#endif

	GAME_ONFOCUS_PARAMS focusValue = (GAME_ONFOCUS_PARAMS)param;
   
	if(focusValue == GAME_ONFOCUS_PARAMS_TUTORIAL)
	{		
		SetGameMode(1);
		ResetGame(1);
		if (mGameMode == 1)
		{
			mTutorialVisible=true;

		}



	}
	else if(focusValue == GAME_ONFOCUS_PARAMS_LEVEL1)
	{
		SetGameMode(2);
		ResetGame(1);
		mScript->run_chunk("level=1");

	}
	else if(focusValue == GAME_ONFOCUS_PARAMS_LEVEL2)
	{
		SetGameMode(3);
		ResetGame(6);
		mScript->run_chunk("level=6");
	}
	else if(focusValue == GAME_ONFOCUS_PARAMS_LEVEL3)
	{
		SetGameMode(4);
		ResetGame(10);		
		mScript->run_chunk("level=10");
	}
	else if (GAME_ONFOCUS_PARAMS_RESUME)
	{
		if(mWaitingForTap)
		{
			mWaiting=false;
			mWaitingForTap= false;
			mLaunchTime = 0;
			mWaitTime=0;
		}

		mTutorialVisible=false;
	}

	//set visibility for game mode
	if(mGameMode == 1)
	{
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeA",false);
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeB",false);
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeC",false);

	}
	else if(mGameMode == 2)
	{		
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeA",true);
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeB",false);
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeC",false);
	}
	else if(mGameMode == 3)
	{
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeA",false);
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeB",true);
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeC",false);
	}
	else if(mGameMode == 4)
	{
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeA",false);
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeB",false);
		app->getRocketGUI()->SetElementVisibility("game_window","tagModeC",true);
	}

	if(mTutorialVisible)
	{
	
		ShowTutorialMenu(mTutorialVisible);
	}


	if(mPlayer)
	{
		//check if vibration is on
		bool vibrate = false;
		vibrate = (app->getPreferencesManager()->getPreference((int)GAME_PREFERENCES_VIBRATE,1) == 1);
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* pcont = (TPlayerController*)ccomp->gameController;
		pcont->setCanVibrate(vibrate);
	
	}

	mPaused=false;

	

}


void TGameStateDemo::ShowTutorialMenu(bool show)
{
	GGameApp* app = GGameApp::getInstance();	
	
	if(!mTutorialLoaded && show)
	{
		mTutorialLoaded=true;

		#if defined(_IRR_ANDROID_PLATEFORM_)
		//load the menu GUI
		if (app->getRocketGUI()->LoadDocument("tutorial.rml"))
		{	
			app->getRocketGUI()->SetDocumentVisible("tutorial_window",true);		
		}
		#else	
		//load the menu GUI
		if (app->getRocketGUI()->LoadDocument("IrrlichtSdcard/tutorial.rml"))
		{	
			app->getRocketGUI()->SetDocumentVisible("tutorial_window",true);		
		}
		#endif

		mTutorialVisible= show;

	}
	else
	{
		app->getRocketGUI()->SetDocumentVisible("tutorial_window",show);	
		mTutorialVisible= show;
	}


}

void TGameStateDemo::VOnLostFocus()
{
	GGameState::VOnLostFocus();

	GGameApp* app = GGameApp::getInstance();	
	app->getRocketGUI()->CloseDocument("game_window");		

	app->getRocketGUI()->CloseDocument("tutorial_window");		
	mTutorialLoaded=false;

	mPaused=true;
}



void TGameStateDemo::SetupTextures()
{
	GGameApp* app = GGameApp::getInstance();
#if defined(_IRR_ANDROID_PLATEFORM_)
	colorTexture = app->getIrrlichtDevice()->getVideoDriver()->getTexture("Figures.tga");  
	metaTexture = app->getIrrlichtDevice()->getVideoDriver()->getTexture("FiguresMeta.tga");  
	alphaTexture = app->getIrrlichtDevice()->getVideoDriver()->getTexture("FiguresAlpha.tga");  
#else
	colorTexture = app->getIrrlichtDevice()->getVideoDriver()->getTexture("IrrlichtSdcard/Figures.tga");  
	metaTexture = app->getIrrlichtDevice()->getVideoDriver()->getTexture("IrrlichtSdcard/FiguresMeta.tga");  
	alphaTexture = app->getIrrlichtDevice()->getVideoDriver()->getTexture("IrrlichtSdcard/FiguresAlpha.tga");  
#endif


}


void TGameStateDemo::SetupRenderTargets(bool reinit)
{
	GGameApp* app = GGameApp::getInstance();
	renderTarget1 = NULL;
	renderTarget2 = NULL;
	core::vector3df p3dout;

	if(!reinit)
	{

		mTargetCamera = app->getIrrlichtDevice()->getSceneManager()->addCameraSceneNode(
							0, 
							irr::core::vector3df(0.0f, 
												-1.0f, 
												-2.0f),
							irr::core::vector3df(0.0f, 
												-1.0f, 
												 0.0f),
							 -1,
							false);

					//target1Camera->setUpVector(irr::core::vector3df(0, -1, 0));
					irr::core::matrix4 mat;
					mat.buildProjectionMatrixOrthoLH(
						CRASH_AREA_HALF_WIDTH*2.0f, 
						CRASH_AREA_HALF_HEIGHT*2.0f,
						-0.1f,10.0f);
					mTargetCamera->setProjectionMatrix(mat,true);

		p3dout.X=CRASH_AREA_HALF_WIDTH*2.0f;
		p3dout.Y=CRASH_AREA_HALF_HEIGHT*2.0f;

		//set size to match screen size using current camera transform
		app->getIrrlichtDevice()->getSceneManager()->setActiveCamera(mCamera);
	
		//calculat RT size in pixels	
		core::matrix4 view = app->getIrrlichtDevice()->getVideoDriver()->getTransform(video::ETS_VIEW);
		core::matrix4 proj  = app->getIrrlichtDevice()->getVideoDriver()->getTransform(video::ETS_PROJECTION);		
		f32 transformedPos[4] = { p3dout.X, p3dout.Y, 0.0f, 1.0f };	
		view.multiplyWith1x4Matrix(transformedPos);
		proj.multiplyWith1x4Matrix(transformedPos);	
		p3dout.X= transformedPos[0]/transformedPos[2];
		p3dout.Y= transformedPos[1]/transformedPos[2];
		p3dout.Z= transformedPos[2];
		p3dout.X = p3dout.X  * ((float)app->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Width)   / 2.0f;
		p3dout.Y = p3dout.Y  * ((float)app->getIrrlichtDevice()->getVideoDriver()->getScreenSize().Height)  / 2.0f;

		//now get the max half square and set it as the render target size in pixels
		int msb = 0;
		mRTSize = (int)p3dout.X;
		while (mRTSize >>= 1) {
			msb++;
		}
		msb = msb-1;

		mRTSize = 2;
		for(int i=0;i<msb;i++)
			mRTSize = mRTSize*2;

#ifndef _IRR_ANDROID_PLATEFORM_
		mRTSize=512;		
#else
		mRTSize=256;		
#endif

	}


	//calculate the render target size
	if (app->getIrrlichtDevice()->getVideoDriver()->queryFeature(video::EVDF_RENDER_TO_TARGET))
	{
		renderTarget1 = app->getIrrlichtDevice()->getVideoDriver()->addRenderTargetTexture(core::dimension2d<u32>(mRTSize ,mRTSize), "RTT1", ECF_R5G6B5);			
	}
	b2_linearSlop;

	if (app->getIrrlichtDevice()->getVideoDriver()->queryFeature(video::EVDF_RENDER_TO_TARGET))
	{
		renderTarget2 = app->getIrrlichtDevice()->getVideoDriver()->addRenderTargetTexture(core::dimension2d<u32>(mRTSize,mRTSize), "RTT2", ECF_R5G6B5);			
	}

}

void TGameStateDemo::SetGameMode(int mode)
{
	GBox2dComponent* bcomp = NULL;

	
	mWaiting=false;
	mWaitingForClear=false;
	mWaitingForJoin=false;
	mWaitingForCrash=false;
	mWaitingForTap= false;
	mLaunchTime = 0;
	mWaitTime=0;

	if(mode == 1) //tutorial
	{
	


		mTutorialState = TUTORIAL_STATE_START;		
		if(mBottomShield)
		{
		
			bcomp = mBottomShield->GetComponent<GBox2dComponent>(mBottomShield->GetComponentIdByIndex(GC_BOX2D_IDX));

			if(bcomp)
			{
				bcomp->mBody->SetActive(true);
			}

			mWorld->SetGravity(b2Vec2(0.0f, 0.0f));	 //unset gravity
		}

	}
	else if(mode >= 2) //normal
	{		
		mTutorialState = TUTORIAL_STATE_NONE;
		if(mBottomShield)
		{
		
			bcomp = mBottomShield->GetComponent<GBox2dComponent>(mBottomShield->GetComponentIdByIndex(GC_BOX2D_IDX));

			if(bcomp)
			{
				bcomp->mBody->SetActive(false);
			}
		}
		mWorld->SetGravity(b2Vec2(0.0f, -2.1f));	 //unset gravity

	}

	mGameMode = mode;

}


void TGameStateDemo::SetupSceneNodes(bool reinit)
{
	GGameApp* app = GGameApp::getInstance();


	SetupElementsSpriteNode(reinit);
	SetupBitsSpriteNode(reinit);
	SetupStaticShieldSpriteNode(reinit);
	SetupDynamicShieldNode(reinit);
	SetupJointsSpriteNode(reinit);


	if(!reinit)
	{
		//create the gameobjects
		mPlayer = CreatePlayerObject();	
		mEnergyBar = CreateBarNode(mPlayer);	
		mRadiationBar = CreateRadiationBarNode(mPlayer);
		mSquareShield = CreateSquareShield();
		mBottomShield = CreateBottomShield();


		mSpringGrid = CreateSpringGrid();
		SetupStaticShieldNodes();


		//create a sprite node to render the metaballs
		mTestNodeId = app->getObjectFactory()->GetNextObjectId();
		SpriteNode* testNode= new SpriteNode(app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
										 app->getIrrlichtDevice()->getSceneManager(),
										 mTestNodeId);	
		testNode->LoadFromTexture(renderTarget1,mRTSize,mRTSize,CRASH_AREA_HALF_WIDTH*2.0f,CRASH_AREA_HALF_HEIGHT*2.0f);
		testNode->setPosition(vector3df(0.0f,-1.0f,-0.01f));
		testNode->setTransparent(true);
		testNode->setMaterialFlag(video::EMF_LIGHTING, false);
		testNode->setMaterialType((video::E_MATERIAL_TYPE)MetaballShader::gMetaballMaterialType);	
		testNode->getMaterial(0).ZWriteEnable = false;
		testNode->getMaterial(0).ZBuffer = false;
		testNode->getMaterial(0).setTexture(0, renderTarget1);
		testNode->getMaterial(0).setTexture(1, renderTarget2);
	


	}


	if(reinit)
	{
		GIrrlichtComponent* ircomp = NULL;
		Base2dNode* node = NULL;


		if(mPlayer)
		{
			ircomp = mPlayer->GetComponent<GIrrlichtComponent>(mPlayer->GetComponentIdByIndex(GC_IRRNODE_IDX));
			node = (Base2dNode*)ircomp->mNode;					
			node->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());
		}

		if(mEnergyBar)
		{
			ircomp = mEnergyBar->GetComponent<GIrrlichtComponent>(mEnergyBar->GetComponentIdByIndex(GC_IRRNODE_IDX));
			node = (Base2dNode*)ircomp->mNode;					
			node->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());		
		}
		if(mRadiationBar)
		{
			ircomp = mRadiationBar->GetComponent<GIrrlichtComponent>(mRadiationBar->GetComponentIdByIndex(GC_IRRNODE_IDX));
			node = (Base2dNode*)ircomp->mNode;					
			node->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());		
		}

		if(mSpringGrid)
		{
			ircomp = mSpringGrid->GetComponent<GIrrlichtComponent>(mSpringGrid->GetComponentIdByIndex(GC_IRRNODE_IDX));
			node = (Base2dNode*)ircomp->mNode;					
			node->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());				
		}
	
	}

	
	SetupStarBackground(reinit);
	SetupFireworkPool(reinit);
	SetupCreatorsPool(reinit);
	SetupColorBars(reinit);		
	
	

}



void TGameStateDemo::SetupJointsSpriteNode(bool reinit)
{
	GGameApp* app = GGameApp::getInstance();
	JointsNode* jointsNode = NULL;

	
	if(!reinit)
	{
		mJointsNodeId = app->getObjectFactory()->GetNextObjectId();
		jointsNode = new JointsNode
										(app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
										 app->getIrrlichtDevice()->getSceneManager(),
										 mJointsNodeId,
										 128);
		#if defined(_IRR_ANDROID_PLATEFORM_)
			jointsNode->Load("Figures.tga",128,128);
		#else
			jointsNode->Load("IrrlichtSdcard/Figures.tga",128,128);
		#endif

		jointsNode->setJointList(NULL,0);
	}
	else
	{
		jointsNode = (JointsNode*)app->getIrrlichtDevice()->getSceneManager()->getSceneNodeFromId(mJointsNodeId);	
		jointsNode->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());

	}

	jointsNode->getMaterial(0).setTexture(0, colorTexture);
	jointsNode->getMaterial(0).setTexture(1, metaTexture);	
	

}

void TGameStateDemo::SetupDynamicShieldNode(bool reinit)
{

	GGameApp* app = GGameApp::getInstance();
	ShieldNode* shieldNode = NULL;

	if(!reinit)
	{
		//initialize the shield node
		mShieldNodeId = app->getObjectFactory()->GetNextObjectId();
		shieldNode = new ShieldNode
										(app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
										 app->getIrrlichtDevice()->getSceneManager(),
										 mShieldNodeId,
										 20);
		#if defined(_IRR_ANDROID_PLATEFORM_)
			shieldNode->Load("laser.tga",128,32);
		#else
			shieldNode->Load("IrrlichtSdcard/laser.tga",128,32);
		#endif
			shieldNode->setGameObjects(&mShieldList);

	}
	else
	{
		shieldNode = (ShieldNode*)app->getIrrlichtDevice()->getSceneManager()->getSceneNodeFromId(mShieldNodeId);			
		shieldNode->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());
	}

}

void TGameStateDemo::SetupStaticShieldSpriteNode(bool reinit)
{
	GGameApp* app = GGameApp::getInstance();
	BatchedSpriteNode* batchedNode = NULL;
	
	if(!reinit)
	{

		mBatchedSpriteNodeStaticShieldId = app->getObjectFactory()->GetNextObjectId();
		batchedNode = new BatchedSpriteNode
										(app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
										 app->getIrrlichtDevice()->getSceneManager(),
										 mBatchedSpriteNodeStaticShieldId,
										 4);
		#if defined(_IRR_ANDROID_PLATEFORM_)
		batchedNode->Load("Figures.tga",128,128);
		#else
		batchedNode->Load("IrrlichtSdcard/Figures.tga",128,128);
		#endif
	}
	else
	{
		batchedNode = (BatchedSpriteNode*)app->getIrrlichtDevice()->getSceneManager()->getSceneNodeFromId(mBatchedSpriteNodeStaticShieldId);			
		batchedNode->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());

	}
	batchedNode->setGameObjects(&mStaticShieldList);


}

void TGameStateDemo::SetupBitsSpriteNode(bool reinit)
{

//initialize the batched sprite node for bits
	GGameApp* app = GGameApp::getInstance();
	BatchedSpriteNode* batchedNode = NULL;
	
	if(!reinit)
	{
		mBatchedSpriteNodeIdBits = app->getObjectFactory()->GetNextObjectId();

		batchedNode = new BatchedSpriteNode
										(app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
										 app->getIrrlichtDevice()->getSceneManager(),
										 mBatchedSpriteNodeIdBits,
										 50);

		#if defined(_IRR_ANDROID_PLATEFORM_)
		batchedNode->Load("Figures.tga",128,128);
		#else
		batchedNode->Load("IrrlichtSdcard/Figures.tga",128,128);
		#endif
	}
	else
	{
		batchedNode = (BatchedSpriteNode*)app->getIrrlichtDevice()->getSceneManager()->getSceneNodeFromId(mBatchedSpriteNodeIdBits);			
		batchedNode->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());

	}

	batchedNode->setGameObjects(&mBitList);

}

void TGameStateDemo::SetupElementsSpriteNode(bool reinit)
{
	GGameApp* app = GGameApp::getInstance();
	BatchedSpriteNode* batchedNode = NULL;

	if(!reinit)
	{
	
		//initialize the batched sprite node for elements
		mBatchedSpriteNodeId = app->getObjectFactory()->GetNextObjectId();
		batchedNode = new BatchedSpriteNode
										(app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
										 app->getIrrlichtDevice()->getSceneManager(),
										 mBatchedSpriteNodeId,
										 500);

		#if defined(_IRR_ANDROID_PLATEFORM_)
		batchedNode->Load("Figures.tga",128,128);
		#else
			batchedNode->Load("IrrlichtSdcard/figures.tga",128,128);
		#endif
	}
	else
	{
		batchedNode = (BatchedSpriteNode*)app->getIrrlichtDevice()->getSceneManager()->getSceneNodeFromId(mBatchedSpriteNodeId);			
		batchedNode->reloadTexture0(app->getIrrlichtDevice()->getVideoDriver());

	}

	batchedNode->getMaterial(0).setTexture(0, colorTexture);
	batchedNode->getMaterial(0).setTexture(1, metaTexture);	
	batchedNode->setGameObjects(&mElementList);
	
}

//re initialize render objects
void TGameStateDemo::VReInitRenderContext()
{
	

	SetupTextures();
	SetupRenderTargets(true);
	SetupSceneNodes(true);


}


int TGameStateDemo::IsRunningTutorial()
{
	int value = 0;
	if(mGameMode==1)
		value = mTutorialState;

	return value;


}
void TGameStateDemo::OnTutorialFinished()
{
	GGameApp* app = GGameApp::getInstance();
	
	mTutorialState = TUTORIAL_STATE_NONE;
	ShowTutorialMenu(false);

	app->mGameManager->VSetNextGameState(GAME_STATE_MAINMENU,0);

}


void TGameStateDemo::SetTutorialText(char* text, int nextButton)
{
	GGameApp* app = GGameApp::getInstance();
	bool showTutorial = nextButton==1;

	ShowTutorialMenu(showTutorial);

	app->getRocketGUI()->SetElementInnerText("tutorial_window","tutorial_div",text); 


}


int TGameStateDemo::GetMoleCount()
{
	int moleCount  = 0;
		
	if (mGoalManager.GoalState() == GOAL_STATE_NOT_FOUND
		|| mGoalManager.GoalState() == GOAL_STATE_NONE)
	{
		moleCount = mGoalManager.mMoleCount;
	}

	return moleCount;

}

void TGameStateDemo::AnimatePalete()
{
	irr::video::SColor color0;
	irr::video::SColor color1;
	irr::video::SColor color2;
	mColorFactor = mColorFactor + mColorSpeed;
	if(mColorFactor > 1.0f)
	{
		mColorFactor=0.0f;
		mColorIndex= mColorIndex + 1;		
		mColorIndex= mColorIndex%4;
	}	
	color0 = mColorPalete[mColorIndex];
	color1 = mColorPalete[(mColorIndex+1)%4];
	color2 = mColorPalete[(mColorIndex+2)%4];
	mColorFrom= color0.getInterpolated(color1,(1.0f-mColorFactor));
	mColorTo=   color1.getInterpolated(color2,(1.0f-mColorFactor));

	//update the wall colors

	for(int i=0; i<mStaticShieldList.size();i++)
	{
		GObjectPtr gameObject = mStaticShieldList[i];
		GSpriteComponent* spcomp = gameObject->GetComponent<GSpriteComponent>(gameObject->GetComponentIdByIndex(GC_SPRITE_IDX));
		spcomp->mColor = mColorFrom;
		spcomp->mColor2 = mColorTo;		
	}

	

}
