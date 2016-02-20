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
#include <cstdio>
#include "TonatiuhDefines.h"
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
#include "irrnode/SpriteNode.h"
#include "irrnode/BatchedSpriteNode.h"
#include "controller/TElementController.h"
#include "controller/TSynthetizerController.h"
#include "TGoalManager.h"
#include "Controller/TPlayerController.h"
#include "Controller/TShieldController.h"
#include "component/TSpringGridComponent.h"
#include "springrid/SpringGrid.h"
#include "spline/spline.h"
#include "scenenode/SpringNode.h"
#include "TGameStateDemo.h"


struct ChainType
{
	int masterFigure;
	int circle;
	int triangle;
	int square;
	bool valid;

	ChainType():masterFigure(0),circle(0),triangle(0),square(0),valid(false)
	{
		;
	}

	bool validateChain(int& nCircle, int& nTriangle, int& nSquare)
	{
		if(nSquare>0)
		{
			masterFigure= (int)T_OS_ELEMENT_POINTS;

			if(((nCircle + nTriangle + nSquare) == nSquare) && nSquare >=4)
			{
				square = nSquare;
				valid = true;
			}
			else
			{
				if(nTriangle>0) 
				{
					valid=false;
				}
				else
				if(nCircle>1)
				{
					square = std::min(4,nSquare);
					circle = std::min(4-square,nCircle);
				
					if( 4 == (square+triangle + circle))
					{
						valid=true;
					}
				}
			}
		}
		else
		if(nTriangle>0)
		{
			masterFigure= (int)T_OS_ELEMENT_SHIELD;


			if(((nCircle + nTriangle + nSquare) == nTriangle) && (nTriangle >= 3))
			{
				triangle = nTriangle;
				valid = true;
			}
			else
			{
				if(nSquare>0) 
				{
					valid=false;
				}
				else
				
				if(nCircle>1)

				{
					triangle = std::min(3,nTriangle);
					circle = std::min(3-triangle,nCircle);
				
					//triangle = nTriangle;
					//circle = nCircle;
				

					if( 3 == (square+triangle + circle))
					{
						valid=true;
					}
					

				}
			}
		}
		else
		if(nCircle>=2)
		{
			masterFigure= (int)T_OS_ELEMENT_ENERGY;

			if((nCircle + nTriangle + nSquare) == nCircle)
			{
				circle = nCircle;
				valid = true;
			}
		}


		if(valid)
		{
			nSquare -= square;
			nCircle -= circle;
			nTriangle -= triangle;
		}

		return valid;
	};

};


//ApplyForceQuery class definition
TGameStateDemo::ApplyForceQuery::ApplyForceQuery(GObjectPtr player,b2World* world, float factor)
{
		mPlayer = player;
		mWorld = world;
		mCount = 0;
		mCursorRadius = 0.0f;
		mForceRadius = 0.0f;
		mForceCenter.SetZero();
		mFactor=factor;

};

void TGameStateDemo::ApplyForceQuery::Execute()
{
	mCount=0;
	mForceRadius = 0.0f;
	mForceCenter.SetZero();

	if(mPlayer)
	{
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* playerController = (TPlayerController*)ccomp->gameController;
		GTransformComponent* tcomp = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		

		//if (playerController->getApplyForce())
		{
			b2AABB aabbf;
			b2Vec2 df;
	
			mForceRadius = playerController->getAttractionForceRadius();
			mCursorRadius = playerController->getCursorRadius();
			
			mForceCenter = tcomp->mTrans.p;

			df.Set(mForceRadius,mForceRadius);


			aabbf.lowerBound = mForceCenter - df;
			aabbf.upperBound = mForceCenter + df;

			mWorld->QueryAABB(this, aabbf);

		}
	}	
}

bool TGameStateDemo::ApplyForceQuery::ReportFixture(b2Fixture* fixture)
{
		if (mCount == 120)
		{
			return false;
		}
		mCount++;

		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			b2Vec2 distance = mForceCenter   - body->GetWorldCenter() ;

            if(distance.Length() > mCursorRadius &&  distance.Length() < mForceRadius)
            {
				float32 massFactor = 60.0f;
				
				if(body->GetUserData())
				{
					GObjectPtr ud = (GObjectPtr)body->GetUserData();									

					if(ud->GetType() == T_OT_ELEMENT)
					{
						GControllerComponent* ccomp = ud->GetComponent<GControllerComponent>(ud->GetComponentIdByIndex(GC_CONTROLLER_IDX));
						TElementController* econt = (TElementController*)ccomp->gameController;
		 			 

						float fdist = distance.Length();
						float fdistrad = 1.5f;

						
						/*if(fdist>1.0f && !econt->getInAttraction()
							&& ud->GetSubType() == T_OS_ELEMENT_BAD1)
						{
							massFactor = massFactor*1.0f; //IACO NORMAL
							if(econt->getElementType() == T_OS_ELEMENT_ENERGY2)
								massFactor = massFactor*0.5f; //IACO NORMAL 
							
							float32 force= massFactor*body->GetMass() / distance.LengthSquared();
							distance.Normalize();
							b2Vec2 vforce = distance;
							vforce*= force;
							
							body->SetAwake(true);				
							body->ApplyForce(vforce, body->GetWorldCenter());
						
						}
						else 
						*/	
						if( fdist>fdistrad && econt->getInAttraction()) //NORMAL MODE			
						{	
						
							b2Vec2 center1 = body->GetWorldCenter();
							float fdist = distance.Normalize();
							b2Vec2 velocity = body->GetLinearVelocity();
							float factor= 2.0f;
								
							if(econt->getElementType() == T_OS_ELEMENT_ENERGY2)
								factor = factor*0.3f; //IACO NORMAL 
							

							factor = factor = mFactor;

							b2Vec2 velocity1 = 10.0f*(factor*distance);
							b2Vec2 veldif = velocity1- velocity;
							velocity=  velocity + veldif;

							
							body->SetLinearVelocity(velocity);
							

						}
						else
						{

							b2Vec2 center1 = body->GetWorldCenter();
							float fdist = distance.Normalize();
							b2Vec2 velocity = body->GetLinearVelocity();
							float factor = .5;
							
							if( (econt->getAllowAttraction())
								///|| (ud->GetSubType() == T_OS_ELEMENT_BAD1 && fdist<=1.0f)
								///|| fdist<=0.7f 
								
								)
							{
								

								econt->setInAttraction(true);
							


								if(fdist>1.0)
								{
									factor = 10.0f/(fdist*fdist);
									factor= factor*.2f;
								}

								if(econt->getElementType() == T_OS_ELEMENT_ENERGY2)
									factor = factor*0.3f; //IACO NORMAL 
								
								factor = factor = mFactor;

								b2Vec2 velocity1 = 10.0f*(factor*distance);
								b2Vec2 veldif = velocity1- velocity;
								velocity=  velocity + veldif;

							
								body->SetLinearVelocity(velocity);
							}
						}
					}
				}
            }
		}

		// Continue the query.
		return true;
}



//on contact handler

void TGameStateDemo::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    void* userData1= contact->GetFixtureA()->GetBody()->GetUserData();
    void* userData2= contact->GetFixtureB()->GetBody()->GetUserData();

	if(userData1 && userData2)
    {	
		GObjectPtr o1 = (GObjectPtr)userData1;
        GObjectPtr o2 = (GObjectPtr)userData2;
		
		if(o1->GetType() == T_OT_SQUARESHIELD)
		{
			b2Vec2 position = contact->GetFixtureB()->GetBody()->GetPosition();
			b2Vec2 speed = contact->GetFixtureB()->GetBody()->GetLinearVelocity();


			if((position.x < -CRASH_AREA_HALF_WIDTH) && (speed.x>0.0f))
			{
				contact->SetEnabled(false);
			}

			if((position.x >  CRASH_AREA_HALF_WIDTH) && (speed.x<0.0f))
			{
				contact->SetEnabled(false);
			}

		}
		else if(o2->GetType() == T_OT_SQUARESHIELD)
		{
			b2Vec2 position = contact->GetFixtureA()->GetBody()->GetPosition();
			b2Vec2 speed = contact->GetFixtureA()->GetBody()->GetLinearVelocity();


			if((position.x < -CRASH_AREA_HALF_WIDTH) && (speed.x>0.0f))
			{
				contact->SetEnabled(false);
			}

			if((position.x >  CRASH_AREA_HALF_WIDTH) && (speed.x<0.0f))
			{
				contact->SetEnabled(false);
			}
		}

	}

}

void TGameStateDemo::BeginContact(b2Contact* contact)
  {

    void* userData1= contact->GetFixtureA()->GetBody()->GetUserData();
    void* userData2= contact->GetFixtureB()->GetBody()->GetUserData();

	GObjectPtr chain3[3];
	chain3[0]=NULL;
	chain3[1]=NULL;
	chain3[2]=NULL;
	int mc=1;

    if(userData1 && userData2)
    {		


		GObjectPtr o1 = (GObjectPtr)userData1;
        GObjectPtr o2 = (GObjectPtr)userData2;
				

		

		if(o1->GetType() == T_OT_PLAYER  &&  o2->GetType()==T_OT_ELEMENT)
		{
			//irr::os::Printer::log("Contact element player");

			/*
			//get the element controller
			GControllerComponent* mainComp = o2->GetComponent<GControllerComponent>(o2->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TElementController* mainECont = (TElementController*)mainComp->gameController;
			
			if(!mainECont->getHasContact())
			{
				mainECont->setHasContact(true);
				mainECont->setHasPlayerContact(true);
				mainECont->setAllowAttraction(true);
				
				//get corresponding mole
				//get the element controller
					GControllerComponent* ccomp = o2->GetComponent<GControllerComponent>(o2->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					TElementController* econt = (TElementController*)ccomp->gameController;
					GBox2dComponent* box2d = o2->GetComponent<GBox2dComponent>(o2->GetComponentIdByIndex(GC_BOX2D_IDX));
	
					chain3[0]=o2;

					//econt->setInCrashForce(true);
					//check if has edges
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
								if(edge2->other->GetUserData() == o2)
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
						bool wasInCrashForce = econt2->getInCrashForce();
						econt2->setHasContact(true);

						mole.atoms[i]= chain3[i];
						mole.colors[i]=  econt2->mColorId;
						mole.count++;
					}
					


					//ad this to the player moles					
					GControllerComponent* pcomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					TPlayerController* playerController = (TPlayerController*)pcomp->gameController;		
					playerController->AddMole(&mole);

					
			}
			*/
		}
		else
		if(o1->GetType() == T_OT_ELEMENT  &&  o2->GetType()==T_OT_PLAYER)
		{
			//irr::os::Printer::log("Contact element player");

			/*
			GControllerComponent* mainComp = o1->GetComponent<GControllerComponent>(o1->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TElementController* mainCcont = (TElementController*)mainComp->gameController;
			
			if(mainCcont->getHasContact())
			{
				mainCcont->setHasContact(true);
				mainCcont->setHasPlayerContact(true);
				mainCcont->setAllowAttraction(true);
				
				//get corresponding mole
				//get the element controller
					GControllerComponent* ccomp = o1->GetComponent<GControllerComponent>(o1->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					TElementController* econt = (TElementController*)ccomp->gameController;
					GBox2dComponent* box2d = o1->GetComponent<GBox2dComponent>(o1->GetComponentIdByIndex(GC_BOX2D_IDX));
	
					chain3[0]=o1;

					//econt->setInCrashForce(true);
					//check if has edges
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
								if(edge2->other->GetUserData() == o1)
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
						bool wasInCrashForce = econt2->getInCrashForce();
						econt2->setHasContact(true);

						mole.atoms[i]= chain3[i];
						mole.colors[i]=  econt2->mColorId;
						mole.count++;
					}

					//ad this to the player moles					
					GControllerComponent* pcomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					TPlayerController* playerController = (TPlayerController*)pcomp->gameController;		
					playerController->AddMole(&mole);

			}
			*/
		}
		/*
		else
		if(o1->GetType() == T_OT_ELEMENT  &&  o2->GetType()==T_OT_ELEMENT)
		{
			GControllerComponent* ccomp1 = o1->GetComponent<GControllerComponent>(o1->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TElementController* econt1 = (TElementController*)ccomp1->gameController;
			
			GControllerComponent* ccomp2 = o2->GetComponent<GControllerComponent>(o2->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TElementController* econt2 = (TElementController*)ccomp2->gameController;
					
			if(econt1->getInCrashForce() && econt2->getInCrashForce())
			{
				econt1->setHasContact(true);
				econt2->setHasContact(true);
				
			}
		}
		else
		if(		   o1->GetType() == T_OT_ELEMENT  
				&& (o1->GetSubType() == T_OS_ELEMENT_ENERGY  || o1->GetSubType() == T_OS_ELEMENT_ENERGY2)
				&& o2->GetType()==T_OT_SHIELD)
		{
			//inactivate the shield
			GControllerComponent* ccomp2 = o2->GetComponent<GControllerComponent>(o2->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TShieldController* econt2 = (TShieldController*)ccomp2->gameController;
					
			if(econt2->mState  == SHIELD_STATE_ACTIVE)
				econt2->mState = SHIELD_STATE_DAMAGED;
		}
		else
		if(o1->GetType() == T_OT_SHIELD &&  o2->GetType()==T_OT_ELEMENT  
				&& (o2->GetSubType() == T_OS_ELEMENT_ENERGY || o2->GetSubType() == T_OS_ELEMENT_ENERGY2 ) )
		{
			//inactivate the shield
			GControllerComponent* ccomp = o1->GetComponent<GControllerComponent>(o1->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TShieldController* econt = (TShieldController*)ccomp->gameController;
					
			if(econt->mState  == SHIELD_STATE_ACTIVE)
				econt->mState = SHIELD_STATE_DAMAGED;
		}
		*/
    }
}





