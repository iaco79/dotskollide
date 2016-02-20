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
#include "TonatiuhDefines.h"
#include <sstream>      // std::stringstream
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


int myrandom (int i) { return std::rand()%i;}


int TGoalManager::mColorCombinations2[20] =   { 0,1,
												0,2,
												1,2,
												0,3,
												1,3,
												2,3,
												0,4,
												1,4,
												2,4,
												3,4
												};

int TGoalManager::mColorCombinations3[30] = {	0,1,2,
												0,1,3,
												0,2,3,
												1,2,3,
												0,1,4,
												0,2,4,
												0,3,4,
												1,2,4,
												1,3,4,
												2,3,4,
												};
int TGoalManager::mColorCombinations4[20] = { 
												0,1,2,3,
												0,1,2,4,
												0,1,3,4,
												0,2,3,4,
												1,2,3,4};

											    ;

int TGoalManager::mGoalCombinations[18][4] = 
									{	
									//1 color
									{2,0,0,0},
									{3,0,0,0},

									//2 colors
									{2,2,0,0},
									{3,2,0,0},
									{2,3,0,0},
									{3,3,0,0},
									{2,2,2,0},

									//3 colors
									{2,2,2,0},
									{3,2,2,0},
									{2,3,2,0},
									{2,2,3,0},
									{3,3,2,0},
									{3,2,3,0},
									{2,3,3,0},
									{3,3,3,0},

									//4 colors
									{2,2,2,2},
									{3,2,2,2},
									{2,3,2,2}};
									

GController* CanCollideQuery::GetElementController(GObjectPtr element)
{
	GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
	TElementController* econt = (TElementController*)ccomp->gameController;			
	return econt;
}

//CanCollideQuery class definition
CanCollideQuery::CanCollideQuery()
{
	mPlayer=NULL;
	mWorld=NULL;
	m_count=0;
	m_countEnergy=0;
	m_countTriangle=0;
	m_countBox=0;
	m_countCross=0;
	m_countEnergy1=0;
	m_countEnergy2=0;
	m_countEnergy3=0; 
	m_countEnergy4=0; 
	m_countEnergy5=0;

	mMaxJoin=3;
	mMaxSwap=0;
	mMinCrash=4;
	mColorCount=5;
}
CanCollideQuery::CanCollideQuery(GObjectPtr player,b2World* world, irr::u32* objectIdList,irr::u32 objectIdListSize)
{
	mObjectIdList=objectIdList;
	mObjectIdListSize=objectIdListSize;


	mPlayer=player;
	mWorld=world;
	m_count=0;
	m_countEnergy=0;
	m_countTriangle=0;
	m_countBox=0;
	m_countCross=0;
	m_countEnergy1=0;
	m_countEnergy2=0;
	m_countEnergy3=0; 
	m_countEnergy4=0; 
	m_countEnergy5=0;

	mMaxJoin=3;
	mMaxSwap=0;
	mMinCrash=4;
	mColorCount=5;

};

void CanCollideQuery::Execute()
{
	m_count=0;
	m_countEnergy=0;

	m_countEnergy1=0;
	m_countEnergy2=0;
	m_countEnergy3=0; 
	m_countEnergy4=0;
	m_countEnergy5=0;
	 
	m_countTriangle=0;
	m_countBox=0;
	m_countCross=0;

	if(mPlayer)
	{
		GControllerComponent* ccomp = mPlayer->GetComponent<GControllerComponent>(mPlayer->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TPlayerController* playerController = (TPlayerController*)ccomp->gameController;
		GTransformComponent* tcomp = mPlayer->GetComponent<GTransformComponent>(mPlayer->GetComponentIdByIndex(GC_TRANSFORM_IDX));

		m_circle.m_radius = playerController->getValidCollideRadius();		
		m_circle.m_p = tcomp->mTrans.p;		
		m_transform.SetIdentity();
	
		b2AABB aabb;
		m_circle.ComputeAABB(&aabb, m_transform, 0);
		mWorld->QueryAABB(this, aabb);

	}
}

std::string TGoalManager::intToString(int i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();

    return s;
}

bool CanCollideQuery::ReportFixture(b2Fixture* fixture)
{
		if (m_count >= mObjectIdListSize)
		{
			return false;
		}

		

		GObjectPtr chain3[3];
		b2Body* body = fixture->GetBody();
		b2Shape* shape = fixture->GetShape();
		/*chain3[0]=NULL;
		chain3[1]=NULL;
		chain3[2]=NULL;		
		int mc=1;
		*/


		bool overlap = b2TestOverlap(shape, 0, &m_circle, 0, body->GetTransform(), m_transform);
		//bool overlap	 = false;

		if (overlap)
		{

		    if (body->GetUserData() != NULL)
		    {
				GObjectPtr ud = (GObjectPtr)body->GetUserData();									

				if(ud->GetType() == T_OT_ELEMENT)
				{
					
						

					//add this to the elements size
					int elementID = ud->GetId();
					bool alreadyInList =false;

					for(int i=0;i<m_count;i++)
					{
						if(mObjectIdList[i] == elementID)
						{
							alreadyInList=true;
							break;
						}
					}

					if(!alreadyInList)
					{
						//irr::os::Printer::log("Element has contact.");
						
						mObjectIdList[m_count] = elementID;			
					}


					//get the element controller
					/*

					GControllerComponent* ccomp = ud->GetComponent<GControllerComponent>(ud->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					TElementController* econt = (TElementController*)ccomp->gameController;
					GBox2dComponent* box2d = ud->GetComponent<GBox2dComponent>(ud->GetComponentIdByIndex(GC_BOX2D_IDX));
	


					
					if(!econt->getHasContact())
					{
						
						irr::os::Printer::log("Element has contact.");
						
						

						
						chain3[0]=ud;

						
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
									if(edge2->other->GetUserData() == ud)
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

					*/
				}
		    }
		}
		m_count++;

		return true;

}



bool CanCollideQuery::IsOpening(GObjectPtr* elements, int count, GObjectPtr* currentTestObject)
{
	
	bool result = false;
	GObjectPtr chain3[3];
	bool fullTrio = false;

	if(count==3)
	{
		//check if there is a 3 chain	
		chain3[0]=NULL;
		chain3[1]=NULL;
		chain3[2]=NULL;
		for(int i=0;i<count;i++)
		{
			GControllerComponent* ccomp = elements[i]->GetComponent<GControllerComponent>(elements[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			GBox2dComponent* box2d = elements[i]->GetComponent<GBox2dComponent>(elements[i]->GetComponentIdByIndex(GC_BOX2D_IDX));
			TElementController* econt = (TElementController*)ccomp->gameController;		
			//get the box2d component
			b2JointEdge* edge = box2d->mBody->GetJointList();

			chain3[0]=NULL;
			chain3[1]=NULL;
			chain3[2]=NULL;

			if(edge!=NULL) //this is at leat a duo
			{	
				chain3[0]=elements[i];
				chain3[1]=(GObjectPtr)edge->other->GetUserData();
				edge = edge->next;

				//this has 2 edges , then it is a trio
				if(edge)
				{
					chain3[2]=(GObjectPtr)edge->other->GetUserData();
					
					//check if other has 2 edges as well
					GControllerComponent* ccomp2 = chain3[1]->GetComponent<GControllerComponent>(chain3[1]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					GBox2dComponent* box2d2 = chain3[1]->GetComponent<GBox2dComponent>(chain3[1]->GetComponentIdByIndex(GC_BOX2D_IDX));
					TElementController* econt2 = (TElementController*)ccomp2->gameController;		
					b2JointEdge* edge2 = box2d2->mBody->GetJointList();
					//check if other body has edge then it is a trio
					if(edge2)
					{
						edge2 = edge2->next;

						//it is a full trio
						if(edge2)
						{
							fullTrio=true;
						}
					}


				}
				
			}

			if(fullTrio)
				break;
		}


		if(fullTrio)
		{
		   
			int indexWithContact = -1;
			int contactCount = 0;

			for(int i=0;i<3;i++)
			{
				GControllerComponent* gco = chain3[i]->GetComponent<GControllerComponent>(chain3[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController*   eco = (TElementController*)gco->gameController;					

				if(eco->getHasPlayerContact())
				{
					contactCount++;
					indexWithContact = i;
				}
			}

			if(contactCount == 1)
			{
				GControllerComponent* gc = chain3[indexWithContact]->GetComponent<GControllerComponent>(chain3[indexWithContact]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController*   ec = (TElementController*)gc->gameController;

				//now check how long we have been tracking this object
				if((*currentTestObject) == NULL)
				{
					(*currentTestObject) =  chain3[indexWithContact];
					ec->setContactTimer(0);
					ec->setHasContactTimer(true);
				}
				else
				{
					GControllerComponent* testgc = (*currentTestObject)->GetComponent<GControllerComponent>((*currentTestObject)->GetComponentIdByIndex(GC_CONTROLLER_IDX));
					TElementController*   testec = (TElementController*)testgc->gameController;

					if((*currentTestObject) !=NULL && (*currentTestObject) !=  chain3[indexWithContact])
					{
						testec->setContactTimer(0);
						testec->setHasContactTimer(false);

						(*currentTestObject) =  chain3[indexWithContact];
						ec->setContactTimer(0);
						ec->setHasContactTimer(true);
					}
					else
					{
						if(testec->getContactTimer()>TRIPLE_CONTACT_TIMER)
						{
							result = true;
						}
					}
				}
			}
		}

	}
	return result;
}


bool CanCollideQuery::IsClosing(GObjectPtr* elements, int count)
{
	bool result = false;
	GObjectPtr chain3[3];
	bool trio = false;

	if(count==3)
	{
		//check if there is a 3 chain	
		chain3[0]=NULL;
		chain3[1]=NULL;
		chain3[2]=NULL;
		int corner1Index=0;
		int corner2Index=0;
		for(int i=0;i<count;i++)
		{
			GControllerComponent* ccomp = elements[i]->GetComponent<GControllerComponent>(elements[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			GBox2dComponent* box2d = elements[i]->GetComponent<GBox2dComponent>(elements[i]->GetComponentIdByIndex(GC_BOX2D_IDX));
			TElementController* econt = (TElementController*)ccomp->gameController;		
			//get the box2d component
			b2JointEdge* edge = box2d->mBody->GetJointList();

			chain3[0]=NULL;
			chain3[1]=NULL;
			chain3[2]=NULL;

			if(edge!=NULL) //this is at leat a duo
			{	
				chain3[0]=elements[i];
				chain3[1]=(GObjectPtr)edge->other->GetUserData();
				edge = edge->next;

				//this has 2 edges , then it is a trio
				if(edge)
				{
					corner1Index=1;
					corner2Index=2;

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

							corner1Index=0;
							corner2Index=2;

							trio=true;
						}
					}
				}
			}

			if(trio)
				break;
		}


		if(trio)
		{
		  //check if corner1 and corner2 are in contact with player

			GControllerComponent* corner1= chain3[corner1Index]->GetComponent<GControllerComponent>(chain3[corner1Index]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			GControllerComponent* corner2= chain3[corner2Index]->GetComponent<GControllerComponent>(chain3[corner2Index]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			

			TElementController* elemCorner1 = (TElementController*)corner1->gameController;		
			
			TElementController* elemCorner2 = (TElementController*)corner2->gameController;		
			
			if(elemCorner1->getHasPlayerContact() &&  elemCorner2->getHasPlayerContact())
			{
				result= true;

			}

		}

	}
	return result;


}

bool CanCollideQuery::IsCrashing(Mole* moles, int moleCount, int& mole1crashed, int& mole2crashed, int& mole3crashed)
{

	
	bool result = false;
	int colorCount1[5]; //color count in mole 1
	int colorCount2[5]; //color count in mole 2
	int colorCount3[5]; //color count in mole 3

	int moleOrder[3];
	bool isCrash[5]; //if the color iscrashing
	bool isPartialCrash[5]; //if the color iscrashing
	int difColor[5];

	for(int i=0;i<5;i++)
	{
		colorCount1[i]=0;
		colorCount2[i]=0;
		colorCount3[i]=0;
		
		isCrash[i] = false;	
		isPartialCrash[i]=false;
	}
	moleOrder[0]=-1;
	moleOrder[1]=-1;
	moleOrder[2]=-1;
	mole1crashed=0;
	mole2crashed=0;
	mole3crashed=0;

	//count the colors in each mole
	for (irr::u32 i = 0; i < moleCount; i++)
	{

		

		for (irr::u32 j = 0; j < moles[i].count; j++)
		{
			if(moles[i].count==3)
				mole3crashed++;
			else
			if(moles[i].count==2)
				mole2crashed++;
			else
				mole1crashed++;


			GObjectPtr element =  moles[i].atoms[j];

			GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TElementController* econt = (TElementController*)ccomp->gameController;
					
			int color = econt->mColorId;

			//if a bad element is present then stop searching
			if(econt->getElementType() == T_OS_ELEMENT_BAD1)
				return false;

				

			if(color>=0 && color<=4)
			{
				switch(i)
				{
				case 0:colorCount1[color] =colorCount1[color]+1;
					break;
				case 1:colorCount2[color] =colorCount2[color]+1;
					break;
				case 2:colorCount3[color] =colorCount3[color]+1;
					break;
				}
			}
		}
	}


	
	//first check if all colors are equal
	//and color is in at least 2 moles
	result = false;
	int difColors =0;

	for(int i=0;i<5;i++)
	{
		int totalColor =  colorCount1[i] +colorCount2[i]  + colorCount3[i];
		int diferentMoles = (colorCount1[i]>0?1:0)+
							(colorCount2[i]>0?1:0)+
							(colorCount3[i]>0?1:0);
		if(totalColor>0)
			difColors++;

		if(totalColor >0 && diferentMoles >=2) 
		{	
			isCrash[i] = (totalColor >= mMinCrash);			
		}	

	}
	//now check the iscrash = true in all moles
	int checkSum = 	     (isCrash[0]?1:0)  
					    +(isCrash[1]?1:0)
						+(isCrash[2]?1:0)
						+(isCrash[3]?1:0)
						+(isCrash[4]?1:0);

	result = (checkSum == difColors) && (checkSum>0);	
	

	//now validate that total count of colors is at leat min colors
	//and that each color is present in all the moles	
	if(!result)
	{
		
		for(int i=0;i<5;i++)
		{
			isCrash[i] = false;	
			isPartialCrash[i]=false;
		}
		bool atLeastOne = false;

		for(int i=0;i<5;i++)
		{		

			int totalColor =  colorCount1[i] +colorCount2[i]  + colorCount3[i];
	
			if(moleCount == 2 )
			{
				if( (   (colorCount1[i] > 0 ) && (colorCount2[i] > 0 ) 
					||  (colorCount1[i] > 0 ) && (colorCount3[i] > 0 ) 
					||  (colorCount2[i] > 0 ) && (colorCount3[i] > 0 ) 
					)
					&& totalColor >= mMinCrash
					)
				{
					isCrash[i] = true;	
					atLeastOne=true;
					
				}		
			}
			else if(moleCount == 3 )
			{
				if( (   (colorCount1[i] > 0 ) && (colorCount2[i] > 0) && (colorCount3[i] > 0 ) 
					)
					&& totalColor >= mMinCrash
					)
				{
					isCrash[i] = true;	
					atLeastOne=true;
					
				}		
			}

		}

		if(atLeastOne)
		{

			result =true;

			for(int i=0;i<5;i++)
			{
				int totalColor =  colorCount1[i] +colorCount2[i]  + colorCount3[i];
				result =result &&  ((totalColor > 0)?isCrash[i]:true);	
			}
		}
	}



	//if we still cannot crash then verifiy if it is a partial crash
	if(!result)
	{

		mole1crashed=0;
		mole2crashed=0;
		mole3crashed=0;
		
		for(int i=0;i<5;i++)
		{
		
			isCrash[i] = false;	
			isPartialCrash[i]=false;
		}

		//sort moles by # of diferent colors
		for(int i=0;i<moleCount;i++)
		{					
			int max=0;

			

			difColor[0]=0;
			difColor[1]=0;
			difColor[2]=0;
			difColor[3]=0;
			difColor[4]=0;

			for(int j=0;j<moleCount;j++)
			{
				bool isMax= false;
				for(int k=0;k<i;k++)
				{
					if(j == moleOrder[k])
					{
						isMax = true;
						break;
					}
				}
				
				for(int k=0;k<moles[j].count;k++)
				{
					if(moles[j].colors[k] >=0 && moles[j].colors[k] <=4)
						difColor[moles[j].colors[k]]=1;				
				}
				
				int difColors = difColor[0]+
								difColor[1]+
								difColor[2]+
								difColor[3]+
								difColor[4];

				if(difColors > max && !isMax)
				{
					moleOrder[i] = j;
					max = difColors;
				}				
			}

			

		}

		if(moleCount == 3)
		{
			Mole* molX = &moles[moleOrder[0]];			
			Mole* mol1 = &moles[moleOrder[1]];			
			Mole* mol2 = &moles[moleOrder[2]];			

			int k=0;
			k++;

			bool colors1X=true;
			bool colors2X=true;
			//now try to substract mole1 and 2 from moleX
			
			mole1crashed=(mol1->count==1?1:0)  + (mol2->count==1?1:0);
			mole2crashed=(mol1->count==2?1:0)  + (mol2->count==2?1:0);
			mole3crashed=(mol1->count==3?1:0)  + (mol2->count==3?1:0);

			//validate that all colors in mole 1 are in X
			for(int i=0;i<mol1->count;i++)
			{
				bool isInMolX = false;

				for(int j=0;j<molX->count;j++)
				{
					int color= molX->colors[j];
					if(mol1->colors[i]==color)
					{
						if(color>=0 && color<=4)
						{
							isInMolX=true;
							isPartialCrash[color]=true;
							break;
						}
						
					}					
				}				
				colors1X = colors1X && isInMolX;			
			}

			//validate that all colors in mole 2 are in X
			for(int i=0;i<mol2->count;i++)
			{
				bool isInMolX = false;

				for(int j=0;j<molX->count;j++)
				{
					int color= molX->colors[j];
					if(mol2->colors[i]==color)
					{
						if(color>=0 && color<=4)
						{

							isInMolX=true;
							isPartialCrash[color]=true;
							break;
						}

						
					}					
				}				
				colors2X = colors2X && isInMolX;			
			}

			
			colors1X = colors1X && (mol1->count>0);
			colors2X = colors2X && (mol2->count>0);
			
			

			if(colors1X && colors2X)
			{
				//now check the ispartial crash colors
				bool allComply=false;

				for(int i=0; i<5;i++)
				{	
					if(isPartialCrash[i])
					{
						int totalColor =  colorCount1[i] +colorCount2[i]  + colorCount3[i];

						if(totalColor>= mMinCrash)
						{
							allComply=true;
							isCrash[i] = true;	
						}
						//else
							///allComply=false;
					}
				}


				result = allComply;

			}			


			

		}
		else if(moleCount == 2)
		{
			Mole* molX = &moles[moleOrder[0]];			
			Mole* mol1 = &moles[moleOrder[1]];			
			int k=0;
			k++;


			mole1crashed=(mol1->count==1?1:0);
			mole2crashed=(mol1->count==2?1:0);
			mole3crashed=(mol1->count==3?1:0);


			//now try to substract mole1 from moleX
			bool colors1X=true;
			bool colors2X=true;
			//now try to substract mole1 and 2 from moleX
			
			//validate that all colors in mole 1 are in X
			for(int i=0;i<mol1->count;i++)
			{
				bool isInMolX = false;

				for(int j=0;j<molX->count;j++)
				{
					int color= molX->colors[j];
					if(mol1->colors[i]==color)
					{
						if(color>=0 && color<=4)
						{
							isInMolX=true;
							isPartialCrash[color]=true;
							break;
						}
						
					}				
				}				
				colors1X = colors1X && isInMolX;			
			}

			
			colors1X = colors1X && (mol1->count>0);
			

			
			if(colors1X )
			{
				//now check the ispartial crash colors
				bool allComply=false;

				for(int i=0; i<5;i++)
				{	
					if(isPartialCrash[i])
					{
						int totalColor =  colorCount1[i] +colorCount2[i]  + colorCount3[i];

						if(totalColor>= mMinCrash)
						{
							allComply=true;
							isCrash[i] = true;	
						}
						
					}
				}

				result = allComply;

			}		

		}





	}


	//and finally lets select the elements with a crashing color
	if(result)
	{
		for (irr::u32 i = 0; i < moleCount; i++)
		{
			for (irr::u32 j = 0; j < moles[i].count; j++)
			{

				GObjectPtr element =  moles[i].atoms[j];

				GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
				TElementController* econt = (TElementController*)ccomp->gameController;
					
				int color = econt->mColorId;

				if(color>=0 && color<=4)
				{
					if(isCrash[color])
					{
						econt->setIsTemporaryCrash(true);
					}
				}
			}
		}
	}

	return result;
}
int CanCollideQuery::IsJoining(Mole* moles, int moleCount)
{
	
	bool result = false;
	int joinType = 0;

	if(mMaxJoin==2)
	{
		if(moleCount==2)
		{
			result = (moles[0].count == 1) && (moles[1].count == 1);

			if(result)
				joinType=1;

		}	
	}
	else
	if(mMaxJoin==3)
	{
	
		if(moleCount==2)
		{
			result = (moles[0].count == 1) && (moles[1].count == 1)
				||   (moles[0].count == 2) && (moles[1].count == 1)
				||	 (moles[0].count == 1) && (moles[1].count == 2);

			if(result)
				joinType=1;


		}	
		else if(moleCount==3)
		{	
			result = (moles[0].count == 1) && (moles[1].count == 1) && (moles[2].count == 1);

			if(result)
				joinType=2;

		}
	}

	return joinType;

}

int  CanCollideQuery::IsSwaping(Mole* moles, int count)
{
	int colorCount[5];
	int sortedColor[5];
	int result = 0;
	

	colorCount[0]=0;
	colorCount[1]=0;
	colorCount[2]=0;
	colorCount[3]=0;
	colorCount[4]=0;
	sortedColor[0]=-1;
	sortedColor[1]=-1;
	sortedColor[2]=-1;
	sortedColor[3]=-1;
	sortedColor[4]=-1;
	

	if(count ==2 )
	{

		//case 2-1
		if(	(	(moles[0].count == 2 && moles[1].count==1)
			||  (moles[0].count == 1 && moles[1].count==2))
				&& mMaxSwap >= 2				
						
			)
		{
			//make sure that at least one color in mole 1 is in mole 2 
			bool sharedColor = false;
			Mole* mole2 = NULL;			
			Mole* mole1 = NULL;			
			if(moles[0].count == 2 && moles[1].count==1)
			{
			 mole2 = &moles[0];
			 mole1 = &moles[1];
			}
			else
			{
			 mole2 = &moles[1];
			 mole1 = &moles[0];
			}

			//validate that that color is in other mole
			for(int j=0;j<moles[0].count;j++)
			{
				int color = moles[0].colors[j];
				for(int k=0;k<moles[1].count;k++)
				{
					if(color == moles[1].colors[k])
					{
						sharedColor = true;
						break;
					}
				
				}

				if(sharedColor)
					break;
			}

			if(sharedColor)
			{
				result=1;

				//count total colors 
				for(int i=0;i<count;i++)
				{
					for(int j=0;j<moles[i].count;j++)
					{
						int color = moles[i].colors[j];

						if(color>=0 && color<=4)
							colorCount[color]= colorCount[color] + 1;

						//reset colors
						moles[i].colors[j]=-1; 

					}
				}
				//sort colors by qty
				for(int i=0;i<5;i++)
				{
					int max=0;
					for(int j=0;j<5;j++)
					{
						bool skip = false;
						for(int k=0;k<i;k++)
						{
							if(sortedColor[k]==j)
								skip=true;
						}
						
						if(!skip && (colorCount[j]  > max))
						{
							max =colorCount[j];
							sortedColor[i]=j;
						}				
					}
				}

				for(int i=0; i<5;i++)
				{
					if (sortedColor[i]>=0)
					{
						int color = sortedColor[i];
						int qty= colorCount[color];
						int freeSlotsMole2=0;
						int freeSlotsMole1=0;
						while(qty>0)
						{
							//get free slots in mole2
							freeSlotsMole2 = (mole2->colors[0]==-1?1:0)
										+(mole2->colors[1]==-1?1:0);
										
							//get free slots in mole1
							freeSlotsMole1 = (mole1->colors[0]==-1?1:0);
										

							//put the color in the free slots
							if(qty<= freeSlotsMole2)
							{
								for(int j=0;j<mole2->count;j++)
								{
									if(mole2->colors[j] == -1  && qty>0)
									{
										mole2->colors[j]=color;
										qty--;
									}								
								}
							}
							else if(color<= freeSlotsMole1)
							{
								for(int j=0;j<mole1->count;j++)
								{
									if(mole1->colors[j] == -1 && qty>0)
									{
										mole1->colors[j]=color;
										qty--;
									}								
								}														
							}
							//try to fill as much as we can
							else
							{
								for(int j=0;j<mole2->count;j++)
								{
									if(mole2->colors[j] == -1  && qty>0)
									{
										mole2->colors[j]=color;
										qty--;
									}								
								}
								for(int j=0;j<mole1->count;j++)
								{
									if(mole1->colors[j] == -1 && qty>0)
									{
										mole1->colors[j]=color;
										qty--;
									}								
								}		
							}
						}
					}
				}

			}
		}
		//case 3-1
		else if((	(moles[0].count == 3 && moles[1].count==1) 
				||  (moles[0].count == 1 && moles[1].count==3))
				&& mMaxSwap >= 3				
				
				)
		{



			//make sure that at least one color in mole 1 is in mole 2 
			bool sharedColor = false;
			Mole* mole3 = NULL;			
			Mole* mole1 = NULL;			
			if(moles[0].count == 3 && moles[1].count==1)
			{
			 mole3 = &moles[0];
			 mole1 = &moles[1];
			}
			else
			{
			 mole3 = &moles[1];
			 mole1 = &moles[0];
			}

			//validate that that color is in other mole
			for(int j=0;j<moles[0].count;j++)
			{
				int color = moles[0].colors[j];
				for(int k=0;k<moles[1].count;k++)
				{
					if(color == moles[1].colors[k])
					{
						sharedColor = true;
						break;
					}
				
				}

				if(sharedColor)
					break;
			}

			if(sharedColor)
			{
				result=1;

				//count total colors 
				for(int i=0;i<count;i++)
				{
					for(int j=0;j<moles[i].count;j++)
					{
						int color = moles[i].colors[j];

						if(color>=0 && color<=4)
							colorCount[color]= colorCount[color] + 1;

						//reset colors
						moles[i].colors[j]=-1; 

					}
				}
				//sort colors by qty
				for(int i=0;i<5;i++)
				{
					int max=0;
					for(int j=0;j<5;j++)
					{
						bool skip = false;
						for(int k=0;k<i;k++)
						{
							if(sortedColor[k]==j)
								skip=true;
						}
						
						if(!skip && (colorCount[j]  > max))
						{
							max =colorCount[j];
							sortedColor[i]=j;
						}				
					}
				}

				for(int i=0; i<5;i++)
				{
					if (sortedColor[i]>=0)
					{
						int color = sortedColor[i];
						int qty= colorCount[color];
						int freeSlotsMole3=0;
						int freeSlotsMole1=0;
						while(qty>0)
						{
							//get free slots in mole2
							freeSlotsMole3 = (mole3->colors[0]==-1?1:0)
											+(mole3->colors[1]==-1?1:0)
											+(mole3->colors[2]==-1?1:0);
										
							//get free slots in mole1
							freeSlotsMole1 = (mole1->colors[0]==-1?1:0);
										

							//put the color in the free slots
							if(qty<= freeSlotsMole3)
							{
								for(int j=0;j<mole3->count;j++)
								{
									if(mole3->colors[j] == -1  && qty>0)
									{
										mole3->colors[j]=color;
										qty--;
									}								
								}
							}
							else if(color<= freeSlotsMole1)
							{
								for(int j=0;j<mole1->count;j++)
								{
									if(mole1->colors[j] == -1 && qty>0)
									{
										mole1->colors[j]=color;
										qty--;
									}								
								}														
							}
							//try to fill as much as we can
							else
							{
								for(int j=0;j<mole3->count;j++)
								{
									if(mole3->colors[j] == -1  && qty>0)
									{
										mole3->colors[j]=color;
										qty--;
									}								
								}
								for(int j=0;j<mole1->count;j++)
								{
									if(mole1->colors[j] == -1 && qty>0)
									{
										mole1->colors[j]=color;
										qty--;
									}								
								}		
							}
						}
					}
				}
			}
		}
		//case 3-2
		else if(   ((moles[0].count == 3 && moles[1].count==2)
				||  (moles[0].count == 2 && moles[1].count==3))
				&& mMaxSwap >= 3)
		{

			//make sure that at least one color in mole 1 is in mole 2 
			bool sharedColor = false;
			Mole* mole3 = NULL;			
			Mole* mole2 = NULL;			
			if(moles[0].count == 3 && moles[1].count==2)
			{
			 mole3 = &moles[0];
			 mole2 = &moles[1];
			}
			else
			{
			 mole3 = &moles[1];
			 mole2 = &moles[0];
			}

			//validate that that color is in other mole
			for(int j=0;j<moles[0].count;j++)
			{
				int color = moles[0].colors[j];
				for(int k=0;k<moles[1].count;k++)
				{
					if(color == moles[1].colors[k])
					{
						sharedColor = true;
						break;
					}
				
				}

				if(sharedColor)
					break;
			}

			if(sharedColor)
			{
				result=1;

				//count total colors 
				for(int i=0;i<count;i++)
				{
					for(int j=0;j<moles[i].count;j++)
					{
						int color = moles[i].colors[j];

						if(color>=0 && color<=4)
							colorCount[color]= colorCount[color] + 1;

						//reset colors
						moles[i].colors[j]=-1; 

					}
				}
				//sort colors by qty
				for(int i=0;i<5;i++)
				{
					int max=0;
					for(int j=0;j<5;j++)
					{
						bool skip = false;
						for(int k=0;k<i;k++)
						{
							if(sortedColor[k]==j)
								skip=true;
						}
						
						if(!skip && (colorCount[j]  > max))
						{
							max =colorCount[j];
							sortedColor[i]=j;
						}				
					}
				}

				for(int i=0; i<5;i++)
				{
					if (sortedColor[i]>=0)
					{
						int color = sortedColor[i];
						int qty= colorCount[color];
						int freeSlotsMole3=0;
						int freeSlotsMole2=0;
						while(qty>0)
						{
							//get free slots in mole2
							freeSlotsMole3 = (mole3->colors[0]==-1?1:0)
											+(mole3->colors[1]==-1?1:0)
											+(mole3->colors[2]==-1?1:0);
										
							//get free slots in mole1
							freeSlotsMole2 = (mole2->colors[0]==-1?1:0)
											+(mole2->colors[1]==-1?1:0);
										

							//put the color in the free slots
							if(qty<= freeSlotsMole3)
							{
								for(int j=0;j<mole3->count;j++)
								{
									if(mole3->colors[j] == -1  && qty>0)
									{
										mole3->colors[j]=color;
										qty--;
									}								
								}
							}
							else if(color<= freeSlotsMole2)
							{
								for(int j=0;j<mole2->count;j++)
								{
									if(mole2->colors[j] == -1 && qty>0)
									{
										mole2->colors[j]=color;
										qty--;
									}								
								}														
							}
							//try to fill as much as we can
							else
							{
								for(int j=0;j<mole3->count;j++)
								{
									if(mole3->colors[j] == -1  && qty>0)
									{
										mole3->colors[j]=color;
										qty--;
									}								
								}
								for(int j=0;j<mole2->count;j++)
								{
									if(mole2->colors[j] == -1 && qty>0)
									{
										mole2->colors[j]=color;
										qty--;
									}								
								}		
							}
						}
					}
				}
			}
		}
		//case 2-2
		else if(	(moles[0].count == 2 && moles[1].count==2 )
					&& mMaxSwap >= 2				)
		{
			//make sure that at least one color in mole 1 is in mole 2 
			bool sharedColor = false;
			Mole* mole2a = NULL;			
			Mole* mole2b = NULL;			
			
			mole2a = &moles[0];
			mole2b = &moles[1];
			

			//validate that that color is in other mole
			for(int j=0;j<moles[0].count;j++)
			{
				int color = moles[0].colors[j];
				for(int k=0;k<moles[1].count;k++)
				{
					if(color == moles[1].colors[k])
					{
						sharedColor = true;
						break;
					}
				
				}

				if(sharedColor)
					break;
			}

			if(sharedColor)
			{
				result=1;

				//count total colors 
				for(int i=0;i<count;i++)
				{
					for(int j=0;j<moles[i].count;j++)
					{
						int color = moles[i].colors[j];

						if(color>=0 && color<=4)
							colorCount[color]= colorCount[color] + 1;

						//reset colors
						moles[i].colors[j]=-1; 

					}
				}
				//sort colors by qty
				for(int i=0;i<5;i++)
				{
					int max=0;
					for(int j=0;j<5;j++)
					{
						bool skip = false;
						for(int k=0;k<i;k++)
						{
							if(sortedColor[k]==j)
								skip=true;
						}
						
						if(!skip && (colorCount[j]  > max))
						{
							max =colorCount[j];
							sortedColor[i]=j;
						}				
					}
				}

				for(int i=0; i<5;i++)
				{
					if (sortedColor[i]>=0)
					{
						int color = sortedColor[i];
						int qty= colorCount[color];
						int freeSlotsMole2a=0;
						int freeSlotsMole2b=0;
						while(qty>0)
						{
							//get free slots in mole2
							freeSlotsMole2a = (mole2a->colors[0]==-1?1:0)
											 +(mole2a->colors[1]==-1?1:0);
										
							//get free slots in mole1
							freeSlotsMole2b = (mole2b->colors[0]==-1?1:0)
											 +(mole2b->colors[1]==-1?1:0);
										

							//put the color in the free slots
							if(qty<= freeSlotsMole2a)
							{
								for(int j=0;j<mole2a->count;j++)
								{
									if(mole2a->colors[j] == -1  && qty>0)
									{
										mole2a->colors[j]=color;
										qty--;
									}								
								}
							}
							else if(color<= freeSlotsMole2b)
							{
								for(int j=0;j<mole2b->count;j++)
								{
									if(mole2b->colors[j] == -1 && qty>0)
									{
										mole2b->colors[j]=color;
										qty--;
									}								
								}														
							}
							//try to fill as much as we can
							else
							{
								for(int j=0;j<mole2a->count;j++)
								{
									if(mole2a->colors[j] == -1  && qty>0)
									{
										mole2a->colors[j]=color;
										qty--;
									}								
								}
								for(int j=0;j<mole2b->count;j++)
								{
									if(mole2b->colors[j] == -1 && qty>0)
									{
										mole2b->colors[j]=color;
										qty--;
									}								
								}		
							}
						}
					}
				}
			}
		}
		//case 3-3s
		else if(   ((moles[0].count == 3 && moles[1].count==3)
				||  (moles[0].count == 3 && moles[1].count==3))
				&& mMaxSwap >= 3				
				)
		{

			//make sure that at least one color in mole 1 is in mole 2 
			bool sharedColor = false;
			Mole* mole3 = NULL;			
			Mole* mole3b = NULL;			
			mole3 =  &moles[0];
			mole3b = &moles[1];
			

			//validate that that color is in other mole
			for(int j=0;j<moles[0].count;j++)
			{
				int color = moles[0].colors[j];
				for(int k=0;k<moles[1].count;k++)
				{
					if(color == moles[1].colors[k])
					{
						sharedColor = true;
						break;
					}
				
				}

				if(sharedColor)
					break;
			}

			if(sharedColor)
			{
				result=1;

				//count total colors 
				for(int i=0;i<count;i++)
				{
					for(int j=0;j<moles[i].count;j++)
					{
						int color = moles[i].colors[j];

						if(color>=0 && color<=4)
							colorCount[color]= colorCount[color] + 1;

						//reset colors
						moles[i].colors[j]=-1; 

					}
				}
				//sort colors by qty
				for(int i=0;i<5;i++)
				{
					int max=0;
					for(int j=0;j<5;j++)
					{
						bool skip = false;
						for(int k=0;k<i;k++)
						{
							if(sortedColor[k]==j)
								skip=true;
						}
						
						if(!skip && (colorCount[j]  > max))
						{
							max =colorCount[j];
							sortedColor[i]=j;
						}				
					}
				}

				for(int i=0; i<5;i++)
				{
					if (sortedColor[i]>=0)
					{
						int color = sortedColor[i];
						int qty= colorCount[color];
						int freeSlotsMole3=0;
						int freeSlotsMole3b=0;
						while(qty>0)
						{
							//get free slots in mole2
							freeSlotsMole3 = (mole3->colors[0]==-1?1:0)
											+(mole3->colors[1]==-1?1:0)
											+(mole3->colors[2]==-1?1:0);
										
							//get free slots in mole1
							freeSlotsMole3b = (mole3b->colors[0]==-1?1:0)
											 +(mole3b->colors[1]==-1?1:0)
											 +(mole3b->colors[2]==-1?1:0);
										

							//put the color in the free slots
							if(qty<= freeSlotsMole3)
							{
								for(int j=0;j<mole3->count;j++)
								{
									if(mole3->colors[j] == -1  && qty>0)
									{
										mole3->colors[j]=color;
										qty--;
									}								
								}
							}
							else if(color<= freeSlotsMole3b)
							{
								for(int j=0;j<mole3b->count;j++)
								{
									if(mole3b->colors[j] == -1 && qty>0)
									{
										mole3b->colors[j]=color;
										qty--;
									}								
								}														
							}
							//try to fill as much as we can
							else
							{
								for(int j=0;j<mole3->count;j++)
								{
									if(mole3->colors[j] == -1  && qty>0)
									{
										mole3->colors[j]=color;
										qty--;
									}								
								}
								for(int j=0;j<mole3b->count;j++)
								{
									if(mole3b->colors[j] == -1 && qty>0)
									{
										mole3b->colors[j]=color;
										qty--;
									}								
								}		
							}
						}
					}
				}
			}
		}

	}

	return result;
}



bool TGoalManager::validateCrashIsObjective(int moleCount, int &points)
{
	bool result = false;
	int diferentColors=0;
	int totalCount =0;
	int crashColors[5];
	crashColors[0] = -1;
	crashColors[1] = -1;
	crashColors[2] = -1;
	crashColors[3] = -1;
	crashColors[4] = -1;
	mSceneIsCrashable = false;

	//get all the objects with crash state = true;


	//count total colors 
	for (irr::u32 i = 0; i < mElementList->size(); i++)
	{
		GObjectPtr element =  (*mElementList)[i];
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
				totalCount++;
			}

		}
	}

	diferentColors = (crashColors[0]>0?1:0)+
						 (crashColors[1]>0?1:0)+
						 (crashColors[2]>0?1:0)+
						 (crashColors[3]>0?1:0)+
						 (crashColors[4]>0?1:0);

	int multiplier = std::max((moleCount-1),0) * diferentColors;
	points = 100 * multiplier * totalCount;
	/*
	if(hasGoal())
	{
		CrashGoal& currentGoal = CurrentGoal();
	

		int bonusA = (crashColors[0]>= currentGoal.colors[0])?crashColors[0]:0;
		int bonusB = (crashColors[1]>= currentGoal.colors[1])?crashColors[1]:0;
		int bonusC = (crashColors[2]>= currentGoal.colors[2])?crashColors[2]:0;
		int bonusD = (crashColors[3]>= currentGoal.colors[3])?crashColors[3]:0;
		int bonusE = (crashColors[4]>= currentGoal.colors[4])?crashColors[4]:0;
	
		
		points+= bonusA * 100 * multiplier;
		points+= bonusB * 100 * multiplier;
		points+= bonusC * 100 * multiplier;
		points+= bonusD * 100 * multiplier;
		points+= bonusE * 100 * multiplier;


		//now make sure that all colors all equal
		result =   (crashColors[0]!=-1?crashColors[0]:0)>= currentGoal.colors[0]
				&& (crashColors[1]!=-1?crashColors[1]:0)>= currentGoal.colors[1]
				&& (crashColors[2]!=-1?crashColors[2]:0)>= currentGoal.colors[2]
				&& (crashColors[3]!=-1?crashColors[3]:0)>= currentGoal.colors[3]
				&& (crashColors[4]!=-1?crashColors[4]:0)>= currentGoal.colors[4];

	}
	else
		*/
	{

		result = false;

	}
	return result;

}


void CrashGoal::reset()
{
	colors[0]=0;
	colors[1]=0;
	colors[2]=0; 
	colors[3]=0;
	colors[4]=0; 
	diferentColors =0;
	totalColors=0;
	hasTrio=false;
	hasDuo=false;
	moleCount =0;
	
}

bool CrashGoal::isEqual(CrashGoal* other)
{
	
	if(other!=NULL)
	{
		return 
		colors[0]==other->colors[0] &&
		colors[1]==other->colors[1] &&
		colors[2]==other->colors[2] &&
		colors[3]==other->colors[3] &&
		colors[4]==other->colors[4];
		
	} 
	return false;

}

void CrashGoal::set(CrashGoal* other)
{
	if(other!=NULL)
	{
		colors[0]=other->colors[0];
		colors[1]=other->colors[1];
		colors[2]=other->colors[2];
		colors[3]=other->colors[3];
		colors[4]=other->colors[4];
		diferentColors =other->diferentColors;
		totalColors=other->totalColors;
		moleCount = other->moleCount;
		hasTrio=other->hasTrio;
		hasDuo=other->hasDuo;


	}
}


void SafeMole::setFrom(Mole* other)
{
	if(other!=NULL)
	{
		count= other->count;

		for(int i=0;i<count;i++)
		{
			colors[i] = other->colors[i];
			objectIds[i] = other->atoms[i]->GetId();
		}
	}

}
void SafeMole::populateMole(Mole* other, irr::core::array<GObjectPtr>& elements)
{
	if(other!=NULL)
	{
		Mole mole;
		mole.clear();
		other->clear();
		
		for(int k=0;k<count;k++)
		{
			int id = objectIds[k];
			int color = colors[k];
			if(id>0)
			{
				GObjectPtr atom = NULL;

				for(int i=0;i<elements.size();i++)
				{
					if(elements[i]->GetId() == id)
					{

						if(mole.count < 3)
						{	
							mole.atoms[mole.count] = elements[i];
							mole.colors[mole.count] =  color;							
						}
						mole.count++;
						break;			
					}
				}
			}
		}

		if(mole.count>0  && mole.count<=3)
		{
			other->set(&mole);
		}
	}
}
void SafeMole::clear()
{
	for(int i=0;i<3;i++)
	{
		colors[i] = 0;
		objectIds[i] = 0;	  	
	
	
	}

	count=0;
}


void Mole::clear()
{
	
	for(int i=0;i<3;i++)
	{
		colors[i] = 0;
		atoms[i] = NULL;	  	
	}
	count=0;


}



void Mole::set(Mole* other)
{
	if(other!=NULL)
	{
		count= other->count;

		for(int i=0;i<count;i++)
		{
			colors[i] = other->colors[i];
			atoms[i] = other->atoms[i];	  	
		}
	}

}






int TGoalManager::GetSceneMoles(Mole* mTestMoles, int maxMoles )
{
	int inAreaCount = 0;
	//GObjectPtr objectsInArea[30];
	GObjectPtr chain3[3];
	int moleCount =0 ;

	bool trio = false;	
	bool duo = false;
		//clear the moles
	for(int i=0;i<maxMoles;i++)
	{
		mTestMoles[i].count=0;
		mTestMoles[i].colors[0]=-1;
		mTestMoles[i].colors[1]=-1;
		mTestMoles[i].colors[2]=-1;
		mTestMoles[i].atoms[0]=NULL;
		mTestMoles[i].atoms[1]=NULL;
		mTestMoles[i].atoms[2]=NULL;	
	}
	
	for (irr::u32 i = 0; i < mElementList->size(); i++)
	{
		GObjectPtr element =  (*mElementList)[i];
		GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		TElementController* econt = (TElementController*)ccomp->gameController;					
		econt->mCounted=false;
		
		//exclude crashed objects
		if(econt->getCrashState() == CRASH_STATE_CRASHED )
		{
			econt->setIsTemporaryCrash(false);
			econt->mCounted=true;	
		}
	}


	//no estar to grab the diferent moles using the objects in area



	//check if there is a 3 chain
	
	for (irr::u32 i = 0; (i < mElementList->size())  && moleCount<maxMoles; i++)
	{
		GControllerComponent* ccomp = (*mElementList)[i]->GetComponent<GControllerComponent>((*mElementList)[i]->GetComponentIdByIndex(GC_CONTROLLER_IDX));
		GBox2dComponent* box2d = (*mElementList)[i]->GetComponent<GBox2dComponent>((*mElementList)[i]->GetComponentIdByIndex(GC_BOX2D_IDX));
		TElementController* econt = (TElementController*)ccomp->gameController;		
		trio = false;	
		duo = false;	
		if(!econt->mCounted)
		{
			econt->mCounted =true;

			//get the box2d component
			b2JointEdge* edge = box2d->mBody->GetJointList();

			chain3[0]=(*mElementList)[i];
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


			if(moleCount<maxMoles)
			{
				if(duo)
				{
					mTestMoles[moleCount].count=2;
					mTestMoles[moleCount].colors[0]= ((TElementController*)CanCollideQuery::GetElementController(chain3[0]))->mColorId;;
					mTestMoles[moleCount].colors[1]= ((TElementController*)CanCollideQuery::GetElementController(chain3[1]))->mColorId;;					
					mTestMoles[moleCount].atoms[0]=chain3[0];
					mTestMoles[moleCount].atoms[1]=chain3[1];					

				}
				else if(trio)
				{
					mTestMoles[moleCount].count=3;
					mTestMoles[moleCount].colors[0]= ((TElementController*)CanCollideQuery::GetElementController(chain3[0]))->mColorId;
					mTestMoles[moleCount].colors[1]= ((TElementController*)CanCollideQuery::GetElementController(chain3[1]))->mColorId;
					mTestMoles[moleCount].colors[2]= ((TElementController*)CanCollideQuery::GetElementController(chain3[2]))->mColorId;
					mTestMoles[moleCount].atoms[0]=chain3[0];
					mTestMoles[moleCount].atoms[1]=chain3[1];
					mTestMoles[moleCount].atoms[2]=chain3[2];
				}
				else
				{
					mTestMoles[moleCount].count=1;
					mTestMoles[moleCount].colors[0]= ((TElementController*)CanCollideQuery::GetElementController(chain3[0]))->mColorId;;
					mTestMoles[moleCount].atoms[0]=chain3[0];
					

				}
			}

			moleCount++;
			
		}
	}



	return moleCount;

}


void TGoalManager::clearMolesFlags(Mole* moles, int moleCount)
{
	for (irr::u32 i = 0; i < moleCount; i++)
	{
		for (irr::u32 j = 0; j < moles[i].count; j++)
		{

			GObjectPtr element =  moles[i].atoms[j];
			GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TElementController* econt = (TElementController*)ccomp->gameController;					
			econt->mCounted=false;
			econt->setIsTemporaryCrash(false);
		}
			
	}
}

void CrashGoal::setFromMoles(Mole* testMoles, int moleCount)
{
	hasTrio=false;
	hasDuo=false;
	//count total colors 
	for(int i=0;i<moleCount;i++)
	{
		for(int j=0;j<testMoles[i].count;j++)
		{
			int color = testMoles[i].colors[j];
			GObjectPtr element =  testMoles[i].atoms[j];
			GControllerComponent* ccomp = element->GetComponent<GControllerComponent>(element->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TElementController* econt = (TElementController*)ccomp->gameController;
				
			if(testMoles[i].count==3)
				hasTrio=true;

			if(testMoles[i].count==2)
				hasDuo=true;


			bool isCrash = econt->getIsTemporaryCrash();
			econt->setIsTemporaryCrash(false);
			if( (color>=0 && color<=4) && isCrash)
			{
				colors[color]= colors[color] + 1;						
			}					
		}
	}		

}

void TGoalManager::doMoleJoin(Mole* mole3, Mole* moleX, Mole* moleY)
{

	for(int i=0;i<moleX->count;i++)
	{
		if(mole3->count<3)
		{
			mole3->atoms[mole3->count] = moleX->atoms[i];
			mole3->colors[mole3->count] = moleX->colors[i];
			mole3->count++;					
		}
	}
	for(int i=0;i<moleY->count;i++)
	{
		if(mole3->count<3)
		{
			mole3->atoms[mole3->count] = moleY->atoms[i];
			mole3->colors[mole3->count] = moleY->colors[i];
			mole3->count++;
		}
	}
}

bool TGoalManager::testMolesAreGoal(Mole* moles,int moleCount, CrashGoal* crashGoal)
{
	bool result = false;
	Mole testMoles[3];
	Mole* moleX=NULL;
	Mole* moleY=NULL;
	Mole* moleZ=NULL;
	Mole* moleW=NULL;
	int goalMoles;
	bool hasTrio=false;
	bool hasDuo=false;
	int mc1=0;
	int mc2=0;
	int mc3=0;

	//reset goals
	crashGoal->colors[0] = 0;
	crashGoal->colors[1] = 0;
	crashGoal->colors[2] = 0;
	crashGoal->colors[3] = 0;
	crashGoal->colors[4] = 0;
	
	if(moleCount>=3)
	{
		//take 3 moles
		moleX = &moles[0];
		moleY = &moles[1];
		moleZ = &moles[2];
		
		//do they crash		
		testMoles[0].set(moleX);			
		testMoles[1].set(moleY);
		testMoles[2].set(moleZ);

		//this is a valid crash then set this as an objective
		clearMolesFlags(testMoles,3);						
		if(mCollideQuery.IsCrashing(testMoles,3,mc1,mc2,mc3))
		{	
			goalMoles=3;
			

			crashGoal->setFromMoles(testMoles, 3);
			result = true;		
		}
	}

	if(moleCount>=4 && !result)
	{
	
		//take 4 moles
		moleX = &moles[0];
		moleY = &moles[1];
		moleZ = &moles[2];
		moleW = &moles[3];
		
		//try crash XYW
		if(!result)
		{
			testMoles[0].set(moleX);			
			testMoles[1].set(moleY);
			testMoles[2].set(moleW);

			//this is a valid crash then set this as an objective
			clearMolesFlags(testMoles,3);						
			if(mCollideQuery.IsCrashing(testMoles,3,mc1,mc2,mc3))
			{	
				goalMoles=3;
				crashGoal->setFromMoles(testMoles, 3);
				result = true;		
			}

		}

		//try crash XYZ
		if(!result)
		{
			
			testMoles[0].set(moleX);			
			testMoles[1].set(moleY);
			testMoles[2].set(moleW);

			//this is a valid crash then set this as an objective
			clearMolesFlags(testMoles,3);						
			if(mCollideQuery.IsCrashing(testMoles,3,mc1,mc2,mc3))
			{	
				goalMoles=3;
				crashGoal->setFromMoles(testMoles, 3);
				result = true;		
			}
		}


		//try crash YZW
		if(!result)
		{
			testMoles[0].set(moleY);			
			testMoles[1].set(moleZ);
			testMoles[2].set(moleW);

			//this is a valid crash then set this as an objective
			clearMolesFlags(testMoles,3);						
			if(mCollideQuery.IsCrashing(testMoles,3,mc1,mc2,mc3))
			{	
				goalMoles=3;
				crashGoal->setFromMoles(testMoles, 3);
				result = true;		
			}
		}


		if(!result)
		{
			//try crash XY
			testMoles[0].set(moleX);			
			testMoles[1].set(moleY);
			clearMolesFlags(testMoles,2);				
			if(mCollideQuery.IsCrashing(testMoles,2,mc1,mc2,mc3))
			{
				goalMoles=2;
				crashGoal->setFromMoles(testMoles, 2);
				result  = true;							
			}
			else //try join XY
			{
				testMoles[0].set(moleX);			
				testMoles[1].set(moleY);
				clearMolesFlags(testMoles,2);
				if(mCollideQuery.IsJoining(testMoles,2))
				{
					//join XY
					Mole moleJ;
					moleJ.count=0;
					doMoleJoin(&moleJ, moleX,moleY);
						
					//try crash J W Z
					testMoles[0].set(&moleJ);			
					testMoles[1].set(moleW);
					testMoles[2].set(moleZ);
					clearMolesFlags(testMoles,3);
					if(mCollideQuery.IsCrashing(testMoles,3,mc1,mc2,mc3))
					{
						goalMoles=3;
						crashGoal->setFromMoles(testMoles, 3);
						result  = true;							
					}
					else
					{ //try to crash just J W
					
						testMoles[0].set(&moleJ);			
						testMoles[1].set(moleW);						
						clearMolesFlags(testMoles,2);
						if(mCollideQuery.IsCrashing(testMoles,2,mc1,mc2,mc3))
						{
							goalMoles=2;
							crashGoal->setFromMoles(testMoles, 2);
							result  = true;							
						}
					
					
					}
				}
			}
		}
		
		if(!result)
		{
			//try crash XZ
			testMoles[0].set(moleX);			
			testMoles[1].set(moleZ);
			clearMolesFlags(testMoles,2);				
			if(mCollideQuery.IsCrashing(testMoles,2,mc1,mc2,mc3))
			{	goalMoles=2;
				crashGoal->setFromMoles(testMoles, 2);
				result  = true;							
			}
			else //try join XZ
			{
				testMoles[0].set(moleX);			
				testMoles[1].set(moleZ);
				clearMolesFlags(testMoles,2);
				if(mCollideQuery.IsJoining(testMoles,2))
				{
					//join XZ
					Mole moleJ;
					moleJ.count=0;
					doMoleJoin(&moleJ, moleX,moleZ);
						
					//try crash J W Y
					testMoles[0].set(&moleJ);			
					testMoles[1].set(moleW);
					testMoles[2].set(moleY);
					clearMolesFlags(testMoles,3);
					if(mCollideQuery.IsCrashing(testMoles,3,mc1,mc2,mc3))
					{	goalMoles=3;
						crashGoal->setFromMoles(testMoles, 3);
						result  = true;							
					}
					else
					{ //try to crash just J W
					
						testMoles[0].set(&moleJ);			
						testMoles[1].set(moleW);						
						clearMolesFlags(testMoles,2);
						if(mCollideQuery.IsCrashing(testMoles,2,mc1,mc2,mc3))
						{
							goalMoles=2;
							crashGoal->setFromMoles(testMoles, 2);
							result  = true;							
						}
					
					
					}
				}
			}
		}
		
		if(!result)
		{
			//try crash YZ
			testMoles[0].set(moleY);			
			testMoles[1].set(moleZ);
			clearMolesFlags(testMoles,2);				
			if(mCollideQuery.IsCrashing(testMoles,2,mc1,mc2,mc3))
			{	
				goalMoles=2;
				crashGoal->setFromMoles(testMoles, 2);
				result  = true;							
			}
			else //try join YZ
			{
				testMoles[0].set(moleY);			
				testMoles[1].set(moleZ);
				clearMolesFlags(testMoles,2);
				if(mCollideQuery.IsJoining(testMoles,2))
				{
					//join YZ
					Mole moleJ;
					moleJ.count=0;
					doMoleJoin(&moleJ, moleY,moleZ);
						
					//try crash J W X
					testMoles[0].set(&moleJ);			
					testMoles[1].set(moleW);
					testMoles[2].set(moleX);
					clearMolesFlags(testMoles,3);
					if(mCollideQuery.IsCrashing(testMoles,3,mc1,mc2,mc3))
					{	
						goalMoles=3;
						crashGoal->setFromMoles(testMoles, 3);
						result  = true;							
					}
					else
					{ //try to crash just J W
					
						testMoles[0].set(&moleJ);			
						testMoles[1].set(moleW);						
						clearMolesFlags(testMoles,2);
						if(mCollideQuery.IsCrashing(testMoles,2,mc1,mc2,mc3))
						{
							goalMoles=2;
							crashGoal->setFromMoles(testMoles, 2);
							result  = true;							
						}
					
					
					}
				}
			}
		}

	}
	else if(moleCount>=3 && !result)
	{
		//take 3 moles
		moleX = &moles[0];
		moleY = &moles[1];
		moleZ = &moles[2];
		
		
		//try to crash XY
		if(!result)
		{
			testMoles[0].set(moleX);			
			testMoles[1].set(moleY);
			clearMolesFlags(testMoles,2);
							
			if(mCollideQuery.IsCrashing(testMoles,2,mc1,mc2,mc3))
			{				
				goalMoles=2;
				crashGoal->setFromMoles(testMoles, 2);
				result  = true;							
			}
		}

		//try to crash XZ
		if(!result)
		{
			testMoles[0].set(moleX);			
			testMoles[1].set(moleZ);
			clearMolesFlags(testMoles,2);
				
			if(mCollideQuery.IsCrashing(testMoles,2,mc1,mc2,mc3))
			{
				goalMoles=2;				
				crashGoal->setFromMoles(testMoles, 2);
				result  = true;							
			}
		}

		//try to crash YZ
		if(!result)
		{
			testMoles[0].set(moleY);			
			testMoles[1].set(moleZ);
			clearMolesFlags(testMoles,2);
							
			if(mCollideQuery.IsCrashing(testMoles,2,mc1,mc2,mc3))
			{
				goalMoles=2;
				crashGoal->setFromMoles(testMoles, 2);
				result  = true;							
			}
		}

	}	
	else if(moleCount>=2 && !result)
	{
		//take 2 moles
		moleX = &moles[0];
		moleY = &moles[1];
		
		//do they crash		
		testMoles[0].set(moleX);			
		testMoles[1].set(moleY);

		//this is a valid crash then set this as an objective
		clearMolesFlags(testMoles,2);
		if(mCollideQuery.IsCrashing(testMoles,2,mc1,mc2,mc3))
		{
			goalMoles=2;
			crashGoal->setFromMoles(testMoles, 2);
			result  = true;							
		}
	}
  

	if(result)
	{
		int mincrash = mCollideQuery.getMinCrash();
		bool complyA  = (crashGoal->colors[0] >0)? crashGoal->colors[0]>=mincrash:false;
		bool complyB  = (crashGoal->colors[1] >0)? crashGoal->colors[1]>=mincrash:false;
		bool complyC  = (crashGoal->colors[2] >0)? crashGoal->colors[2]>=mincrash:false;
		bool complyD  = (crashGoal->colors[3] >0)? crashGoal->colors[3]>=mincrash:false;
		bool complyE  = (crashGoal->colors[4] >0)? crashGoal->colors[4]>=mincrash:false;

		if( (complyA  && crashGoal->colors[0]==1) ||
			(complyB  && crashGoal->colors[1]==1) ||
			(complyC  && crashGoal->colors[2]==1) ||
			(complyD  && crashGoal->colors[3]==1) ||
			(complyE  && crashGoal->colors[4]==1) 
			)
		{
			int l=0;
			l++;
		}


		result = complyA || complyB || complyC || complyD || complyE;

		if(result)
		{

			//now lets validate it complies with level goal requirements
			int minColors = 100;

			minColors =  std::min(crashGoal->colors[0]>0?crashGoal->colors[0]:100,minColors);
			minColors =  std::min(crashGoal->colors[1]>0?crashGoal->colors[1]:100,minColors);
			minColors =  std::min(crashGoal->colors[2]>0?crashGoal->colors[2]:100,minColors);
			minColors =  std::min(crashGoal->colors[3]>0?crashGoal->colors[3]:100,minColors);
			minColors =  std::min(crashGoal->colors[4]>0?crashGoal->colors[4]:100,minColors);

			crashGoal->moleCount = goalMoles;
			crashGoal->totalColors =  crashGoal->colors[0]
									+ crashGoal->colors[1]
									+ crashGoal->colors[2]
									+ crashGoal->colors[3]
									+ crashGoal->colors[4];

			crashGoal->diferentColors =   (crashGoal->colors[0]>0?1:0)
										+ (crashGoal->colors[1]>0?1:0)
										+ (crashGoal->colors[2]>0?1:0)
										+ (crashGoal->colors[3]>0?1:0)
										+ (crashGoal->colors[4]>0?1:0);
			/*
			if(	!(  crashGoal->diferentColors>= mLevelValues.goalDifColors 
				 && minColors>= mLevelValues.goalColorCount))
				result = false;
				*/
		}
	}

	return result;


}


void TGoalManager::startMonitoringScene(LevelValues levelValues)
{
	//Refresh the list of moles
	mLevelValues = levelValues;

	//get the moles in scene
	mGoalState = GOAL_STATE_SEARCHING;	
	mHasGoal= false;
	mTmpGoalCount=0;
	
	mgI=0;
	mgJ=0;
	mgK=0;
	mgL=0;
	mCycleCount=0;

	
	mCollideQuery.setMaxJoin(levelValues.maxJoin);
	mCollideQuery.setMinCrash(2);
	mCollideQuery.setMaxSwap(0);
	
	

	mtmpCrashGoals[5];
	for(int i=0;i<5;i++)
	{
		mtmpCrashGoals[i].reset();
		
	}
		
	//get up to 20 random scene moles
	for(int i=0;i<20;i++)
		mShuffle[i]=i;		

	mMoleCount = GetSceneMoles(mMoles,20);	
	std::random_shuffle(&mShuffle[0],&mShuffle[mMoleCount],myrandom);

	if(mMoleCount <=1)
		mGoalState = GOAL_STATE_NONE;


}

void TGoalManager::Initialize(irr::core::array<GObjectPtr>* elements)
{
	mElementList = elements;
}

TGoalManager::TGoalManager()
{
		mHasGoal= false;
		mGoalState = GOAL_STATE_NONE;
		mMoleCount=0;
		mTmpGoalCount=0;
		mElementList=NULL;
}


bool TGoalManager::DoLogicFindGoal2(int gi,int gj)
{
	Mole testMoles[2];		
	CrashGoal tmpgoal;
	tmpgoal.reset();
	bool result = false;

	testMoles[0].set(&mMoles[gi]);
	testMoles[1].set(&mMoles[gj]);
		
	if ( testMolesAreGoal(testMoles, 2, &tmpgoal))
	{
		//make sure this goal is not repeated
		bool isRepeated=false;
		for(int g=0;g<mTmpGoalCount;g++)
		{
			isRepeated= tmpgoal.isEqual(&mtmpCrashGoals[g]);
			if(isRepeated)
				break;
		}


		if(!isRepeated && mTmpGoalCount <5)
		{	
			mtmpCrashGoals[mTmpGoalCount].set(&tmpgoal);
			mTmpGoalCount++;
			result=true;
		}
	}	

	return result;

}

bool TGoalManager::DoLogicFindGoal3(int gi,int gj,int gk)
{
	bool result = false;

	Mole testMoles[3];		
	CrashGoal tmpgoal;
	tmpgoal.reset();
	
	testMoles[0].set(&mMoles[gi]);
	testMoles[1].set(&mMoles[gj]);
	testMoles[2].set(&mMoles[gk]);
	
	
	if ( testMolesAreGoal(testMoles, 3, &tmpgoal))
	{
		//make sure this goal is not repeated
		bool isRepeated=false;
		for(int g=0;g<mTmpGoalCount;g++)
		{
			isRepeated= tmpgoal.isEqual(&mtmpCrashGoals[g]);
			if(isRepeated)
				break;
		}


		if(!isRepeated && mTmpGoalCount <5)
		{	
			mtmpCrashGoals[mTmpGoalCount].set(&tmpgoal);
			mTmpGoalCount++;
			result=true;
		}
	}	

	return result;

}

void CrashGoal::ensureColorsAreEqual()
{
	//select the max color
	int maxc=0;

	for(int i=0;i<5;i++)
	{
		if(colors[i]>0 )
		{
			maxc= std::max(maxc, colors[i]);			
		}
	}

	for(int i=0;i<5;i++)
	{
		if(colors[i]>0 )
		{
			colors[i] = maxc;
		}
	}


}
void CrashGoal::removeOneRandomColor()
{
	//select a random color
	
	int r = rand()%diferentColors;
	int c=0;
	
	for(int i=0;i<5;i++)
	{
		if(colors[i]>0 )
		{
			if(i==r)
			{
				colors[i]=0;
				diferentColors = -1;				
				break;
			}
			c++;			
		}
	}


}


bool TGoalManager::DoLogicFindGoal4(int gi,int gj,int gl,int gk)
{	
	Mole testMoles[4];		
	CrashGoal tmpgoal;
	tmpgoal.reset();
	bool result = false;

	testMoles[0].set(&mMoles[gi]);
	testMoles[1].set(&mMoles[gj]);
	testMoles[2].set(&mMoles[gk]);
	testMoles[3].set(&mMoles[gl]);
		
	if (testMolesAreGoal(testMoles, 4, &tmpgoal))
	{
		//make sure this goal is not repeated
		bool isRepeated=false;
		for(int g=0;g<mTmpGoalCount;g++)
		{
			isRepeated= tmpgoal.isEqual(&mtmpCrashGoals[g]);
			if(isRepeated)
				break;
		}


		if(!isRepeated && mTmpGoalCount <5)
		{	
			mtmpCrashGoals[mTmpGoalCount].set(&tmpgoal);
			mTmpGoalCount++;
			result=true;
		}
	}	

	return result;

	
}


void TGoalManager::update(int elapsedMS, int max)
{

	if(mGoalState==GOAL_STATE_SEARCHING)
	{
		bool stopSearch=false;

		for(int v=0;v<max;v++)
		{

			if(mMoleCount>=4)
			{
				if(mgI<mMoleCount && mgJ <mMoleCount && mgK<mMoleCount && mgL<mMoleCount)
				{
					bool skip =  mgI==mgJ 
							|| mgI==mgK 
							|| mgI==mgL 
							|| mgJ==mgL 
							|| mgJ==mgK
							|| mgK==mgL 							
							|| mTmpGoalCount>=5;
							
					if(!skip)
						DoLogicFindGoal4(mgI, mgJ, mgK, mgL);
				};
				
				mgL++;
				mCycleCount++;
			
				if(mgL>=mMoleCount)
				{
					mgL=0;
					mgK++;		
				}
						
				if(mgK>=mMoleCount)
				{
					mgK=0;
					mgJ++;
				}
			
				if(mgJ>=mMoleCount)
				{	mgJ=0;
					mgI++;
				}


				if(mgI>=mMoleCount || mTmpGoalCount>=mMoleCount)
				{
					stopSearch=true;
				}

			}
			else if(mMoleCount>=3)
			{	
				if(mgI<mMoleCount && mgJ <mMoleCount && mgK<mMoleCount)
				{
					bool skip =  (mgI==mgJ || mgI==mgK || mgJ==mgK || mTmpGoalCount>=5 );					
					if(!skip)
						DoLogicFindGoal3(mgI, mgJ, mgK);
				};
			
				mgK++;
				mCycleCount++;
				if(mgK>=mMoleCount)
				{
					mgK=0;
					mgJ++;
				}
			
				if(mgJ>=mMoleCount)
				{	mgJ=0;
					mgI++;
				}


				if(mgI>=mMoleCount || (mTmpGoalCount>=5))
				{
					stopSearch=true;
				}
			}
			else if(mMoleCount>=2)
			{	
				if(mgI<mMoleCount && mgJ <mMoleCount)
				{
					bool skip =  (mgJ==mgI || mTmpGoalCount>=5);

					if(!skip)
						DoLogicFindGoal2(mgI, mgJ);
				};
			
				mgJ++;	
				mCycleCount++;
				if(mgJ>=mMoleCount)
				{	mgJ=0;
					mgI++;
				}


				if(mgI>=mMoleCount || (mTmpGoalCount>=5))
				{
					stopSearch=true;
					break; //exit for
				}
			}
		}


		if(stopSearch)
		{
			mGoalState = GOAL_STATE_NONE;
			
			if(mTmpGoalCount>0)
			{
				/*
				int complexity=  mLevelValues.goalColorCount;

				int goalOrder[10];

				for(int i=0;i<10;i++)
				{
					goalOrder[i]=-1;	
				}

				//lets make the goals look more challenging and fair
				for(int i=0;i<mTmpGoalCount;i++)
				{					
					
					if(mLevelValues.maxJoin>=3)
					{
						if(	    mtmpCrashGoals[i].moleCount>=2
							&&  mtmpCrashGoals[i].diferentColors==2)
						{						
							mtmpCrashGoals[i].ensureColorsAreEqual();
						}							
					}
					else if(mLevelValues.maxJoin==2)
					{
							if(	    mtmpCrashGoals[i].moleCount>=2
							&&  mtmpCrashGoals[i].diferentColors==2)
						{						
							mtmpCrashGoals[i].ensureColorsAreEqual();
						}					
					
					}

					//this is to complex
					if(complexity<=3)
					{
						if(	    mtmpCrashGoals[i].moleCount==3  
							&&  mtmpCrashGoals[i].diferentColors>3)
						{					
							//this is too complex. remove one color
							mtmpCrashGoals[i].removeOneRandomColor();
						}
					}



				}


				//sort by different colors and then by # of moles
				for(int i=0;i<mTmpGoalCount;i++)
				{					
					int maxDif=0;
					int maxGoalCount=0;

					for(int j=0;j<mTmpGoalCount;j++)
					{
						bool isMax= false;
						for(int k=0;k<i;k++)
						{
							if(j == goalOrder[k])
							{
								isMax = true;
								break;
							}
						}
						
						//check if maxColors 
						if(!isMax)
						{
						
							if(mtmpCrashGoals[j].moleCount > maxGoalCount)
							{
								goalOrder[i] = j;
								maxGoalCount = mtmpCrashGoals[j].moleCount;
								maxDif = mtmpCrashGoals[j].diferentColors;								
							}
							else if(mtmpCrashGoals[j].diferentColors > maxDif)
							{
								goalOrder[i] = j;
								//maxGoalCount = mtmpCrashGoals[j].moleCount;
								maxDif = mtmpCrashGoals[j].diferentColors;								
							}
						}
					}
					
				}
			
				int high=0;
				int low=mTmpGoalCount-1;
				int medLow  = (mTmpGoalCount-(mTmpGoalCount/3))-1;
				int med= (mTmpGoalCount/2)-1;
				int oo = low;

				if(low<0)
					low=0;
				if(medLow<0)
					medLow=0;
				if(med<0)
					med=0;


				if(mLevelValues.goalColorCount==2)
					oo = medLow;
				else if(mLevelValues.goalColorCount==3)
					oo = med;
				else if(mLevelValues.goalColorCount==4)
					oo = high;
				else
					oo = low;


				*/
			//	mCurrentGoal.set(&mtmpCrashGoals[goalOrder[oo]]);
				mGoalState = GOAL_STATE_CRASHABLE;
			}
			else
			{
				mGoalState = GOAL_STATE_NOT_FOUND;
			}
		}
	}
}
	

int TGoalManager::fact(int n)
{

	int r = 1;

	switch(n)
	{
	case 0:
		r = 1;
		break;
	case 1:
		r = 1;
		break;
	case 2:
		r = 1*2;
		break;
	case 3:
		r = 1*2*3;
		break;
	case 4:
		r = 1*2*3*4;
		break;
	case 5:
		r = 1*2*3*4*5;
		break;
	}

	return r;
}
/*
int TGoalManager::calculateGoals()
{

	int from = mLevelValues.goalIdFrom;
	int to =  mLevelValues.goalIdTo;
	int cnt = (to - from)+1;
	int colors = mLevelValues.colorCount;

	int r= 0;
	
	int combin = 0;

	if(colors == 1)
		combin = 5;
	else if(colors == 2)
		combin = 10;
	else if(colors == 3)
		combin = 10;
	else if (colors == 4)
		combin = 5;
	
	mGoalCount = combin;

	if(colors == 1)
	{
		for(int i=0;i<combin;i++)
		{
			mCrashGoals[i].reset();

			int colorX = i;
			
			if(cnt>1)
				r = rand()%cnt;		

			
			//peek random goal
			int qtyX=mGoalCombinations[from+r][0];
			
			//fill colors
			mCrashGoals[i].colors[colorX] = qtyX;

			mCrashGoals[i].diferentColors = colors;
			mCrashGoals[i].totalColors = qtyX;
		}
	}
	else
	if(colors == 2)
	{
		for(int i=0;i<combin;i++)
		{
			mCrashGoals[i].reset();

			int colorX = TGoalManager::mColorCombinations2[i*2];
			int colorY = TGoalManager::mColorCombinations2[i*2+1];
			if(cnt>1)
				r = rand()%cnt;		

		
			//peek random goal
			int qtyX=mGoalCombinations[from+r][0];
			int qtyY=mGoalCombinations[from+r][1];
			
			//fill colors
			mCrashGoals[i].colors[colorX] = qtyX;
			mCrashGoals[i].colors[colorY] = qtyY;

			mCrashGoals[i].diferentColors = colors;
			mCrashGoals[i].totalColors = qtyX + qtyY;
		}
	}
	else
	if(colors == 3)
	{
		for(int i=0;i<combin;i++)
		{
			mCrashGoals[i].reset();

			int colorX = TGoalManager::mColorCombinations3[i*3];
			int colorY = TGoalManager::mColorCombinations3[i*3+1];
			int colorZ = TGoalManager::mColorCombinations3[i*3+2];

			if(cnt>1)
				r = rand()%cnt;		

			
			//peek random goal
			int qtyX=mGoalCombinations[from+r][0];
			int qtyY=mGoalCombinations[from+r][1];
			int qtyZ=mGoalCombinations[from+r][2];
			
			//fill colors
			mCrashGoals[i].colors[colorX] = qtyX;
			mCrashGoals[i].colors[colorY] = qtyY;
			mCrashGoals[i].colors[colorZ] = qtyZ;

			mCrashGoals[i].diferentColors = colors;
			mCrashGoals[i].totalColors = qtyX + qtyY + qtyZ;
		}
	}
	else
	if(colors == 4)
	{
		for(int i=0;i<combin;i++)
		{
			mCrashGoals[i].reset();
			int colorX = TGoalManager::mColorCombinations4[i*4];
			int colorY = TGoalManager::mColorCombinations4[i*4+1];
			int colorZ = TGoalManager::mColorCombinations4[i*4+2];
			int colorW = TGoalManager::mColorCombinations4[i*4+3];
			
			if(cnt>1)
				r = rand()%cnt;		

			
			//peek random goal
			int qtyX=mGoalCombinations[from+r][0];
			int qtyY=mGoalCombinations[from+r][1];
			int qtyZ=mGoalCombinations[from+r][2];			
			int qtyW=mGoalCombinations[from+r][3];			
			//fill colors
			mCrashGoals[i].colors[colorX] = qtyX;
			mCrashGoals[i].colors[colorY] = qtyY;
			mCrashGoals[i].colors[colorZ] = qtyZ;
			mCrashGoals[i].colors[colorW] = qtyW;

			mCrashGoals[i].diferentColors = colors;
			mCrashGoals[i].totalColors = qtyX + qtyY + qtyZ + qtyW;
		}
	}

	mGoalIndex = mGoalCount-1;
	return mGoalIndex;

}
*/
