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
// Based on nehe.gamedev.net spring physics tutorial.

#include <stdlib.h>     
#include "springrid/SpringGrid.h"

using namespace springrid;

SpringGrid::SpringGrid(										//a long long constructor with 11 parameters starts here
	int numOfMasses,								//1. the number of masses
	float m,										//2. weight of each mass
	float springConstant,							//3. how stiff the springs are
	float springLength,								//4. the length that a spring does not exert any force
	float springFrictionConstant,					//5. inner friction constant of spring
	Vector3D gravitation,							//6. gravitational acceleration
	float airFrictionConstant,						//7. air friction constant
	float groundRepulsionConstant,					//8. ground repulsion constant
	float groundFrictionConstant,					//9. ground friction constant
	float groundAbsorptionConstant,					//10. ground absorption constant
	float groundHeight,								//11. height of the ground (y position)
	int   gridWidfth
	) : Simulation(numOfMasses, m)					//The super class creates masses with weights m of each
{
	 

	applyExtraForce=false;
	applyForceTest=false;
	this->allSpringConstant= springConstant;
	this->allSpringLength = springLength;
	this->gravitation = gravitation;
	this->gridWidth = gridWidfth;
	this->gridHeight = numOfMasses/gridWidfth;
	this->allFrictionConstant = springFrictionConstant;

	this->groundFrictionConstant = groundFrictionConstant;
	this->groundRepulsionConstant = groundRepulsionConstant;
	this->groundAbsorptionConstant = groundAbsorptionConstant;
	this->groundHeight = groundHeight;
	this->applyForceTest = false;

	for (int i=0; i<10;i++)
	{
		mPinchForces[i].mForcePosition = Vector3D(0.0f,0.0f, 0.0f);
		mPinchForces[i].mForceFactor = 0.0f;
		mPinchForces[i].mForceEnable = false;
		mPinchForces[i].mIndex = i;
		mPinchForces[i].type  = 0;
		mPinchForces[i].mRadius = 1.0f;

	}

	for (int a = 0; a < numOfMasses; ++a)			//To set the initial positions of masses loop with for(;;)
	{
		masses[a]->pos.x = -((gridWidth-1)*springLength/2.0) + (a%gridWidth) * springLength;		//Set x position of masses[a] with springLength distance to its neighbor
		masses[a]->pos.y =-1.0f +  -((gridHeight-1)*springLength/2.0) + (a/gridWidth) * springLength;			//Set y position as 0 so that it stand horizontal with respect to the ground
		masses[a]->pos.z = 0.06f;						//Set z position as 0 so that it looks simple
			
		masses[a]->oripos = masses[a]->pos;

	}

	//IACO calculate springs needed
	springCount = (gridWidfth-1)*(gridHeight-1)*4
						- (gridWidfth-2)*(gridHeight-1) 
						- (gridWidfth-1)*(gridHeight-2) ;

	springs = new Spring*[springCount];			//create [numOfMasses - 1] pointers for springs
													//([numOfMasses - 1] springs are necessary for numOfMasses)
	int sc=0;
		
	for(int j=0;j<gridHeight-1;j++)
	{
		for(int i=0;i<gridWidfth-1;i++)
		{
				
			springs[sc] = new Spring(masses[j*gridWidfth+i], 
										masses[j*gridWidfth+i+1], springConstant, springLength, springFrictionConstant);

			if(j==0)
			{					
				masses[j*gridWidfth+i]->isFixed=true;
				masses[j*gridWidfth+i+1]->isFixed=true;
			}

			sc++;

			springs[sc] = new Spring(masses[j*gridWidfth+i], 
										masses[(j+1)*gridWidfth+i], springConstant, springLength, springFrictionConstant);
				
			if(i==0)
			{	
				masses[j*gridWidfth+i]->isFixed=true;					
				masses[(j+1)*gridWidfth+i]->isFixed=true;					
			}

			sc++;

			if(i==gridWidfth-2)
			{
				springs[sc] = new Spring(masses[j*gridWidfth+i+1], 
											masses[(j+1)*gridWidfth+i+1], springConstant, springLength, springFrictionConstant);
					
				masses[j*gridWidfth+i+1]->isFixed=true;
				masses[(j+1)*gridWidfth+i+1]->isFixed=true;

				sc++;
			}

			if(j==gridHeight-2)
			{
				springs[sc] = new Spring(masses[(j+1)*gridWidfth+i], 
											masses[(j+1)*gridWidfth+i+1], springConstant, springLength, springFrictionConstant);

				masses[(j+1)*gridWidfth+i]->isFixed=true;
				masses[(j+1)*gridWidfth+i+1]->isFixed=true;

				sc++;
			}
				
		}
	}
	
}


	
Mass* SpringGrid::getMass(int i,int j)
{
	int a = gridWidth*j+i;

	return Simulation::getMass(a);

}


float SpringGrid::getSpringLength()
{
	return allSpringLength;
}


int SpringGrid::getMassCountW()
{
	return gridWidth;
}

int SpringGrid::getMassCountH()
{
	return gridHeight;
}

int SpringGrid::getMassCount()
{
	return numOfMasses;
}

float SpringGrid::getTotalWidth()
{
	return (gridWidth-1)*allSpringLength;
}
float SpringGrid::getTotalHeight()
{
	return (gridHeight -1)*allSpringLength;
}

void SpringGrid::release()										//release() is overriden because we have springs to delete
{
	Simulation::release();							//Have the super class release itself

	for (int a = 0; a < springCount; ++a)		//to delete all springs, start a loop
	{
		delete(springs[a]);
		springs[a] = NULL;
	}
		
	delete(springs);
	springs = NULL;
}


void SpringGrid::solve()										//solve() is overriden because we have forces to be applied
{
	for (int a = 0; a < springCount; ++a)		//apply force of all springs
	{
		springs[a]->solve();						//Spring with index "a" should apply its force
	}
}

void SpringGrid::applyPinchForce(PinchForce force)
{
	
		float spc=2.5;
		float forceF = 4.0;
		for (int a = 0; a < numOfMasses; a++)	
		{

			Vector3D dif = force.mForcePosition  - Vector3D(masses[a]->pos.x,masses[a]->pos.y,0);


			masses[a]->inForce=false;
			
			float distance = abs(dif.length());

			if(distance<=(allSpringLength*spc))
			{

				masses[a]->inForce=true;
			
				float unitDistance = distance/(allSpringLength*spc);

					
				float factor = 0;
					
				if(distance>0.4)
					factor = (masses[a]->m * 100.0)/  (unitDistance*unitDistance);
					
 
				Vector3D finalForce=   dif.unit();
			
				{
					finalForce = finalForce*  factor * 0.25f* force.mForceFactor;

					finalForce.z = 0;					
				}

				masses[a]->applyForce(finalForce);
				
					
			}
	}
	/*
	for (int a = 0; a < numOfMasses; a++)	
	{
		Vector3D dist2 = force.mForcePosition - Vector3D(masses[a]->pos.x,masses[a]->pos.y,0);
		float distsq =  dist2.length()*dist2.length();

		if (dist2.length() < force.mRadius)
        {
			Vector3D newf = force.mForcePosition - Vector3D(masses[a]->pos.x,masses[a]->pos.y,0) ;
			
			newf*=  (10.0f * force.mForceFactor / (100.0f + distsq));
			masses[a]->applyForce(newf);	
		//	masses[a]->increaseDamping(0.95f);

        }
    }
	*/
	
}

int SpringGrid::SetPinchForce(PinchForce force)
{
	int index=-1;
	for (int i=0; i<10;i++)
	{
		if ( !mPinchForces[i].mForceEnable)
		{
			mPinchForces[i] = force;
			mPinchForces[i].mForceEnable =true;
			mPinchForces[i].mIndex = i;
			index = i;
			break;
		}	

	}
	return index;
}


void SpringGrid::UnsetPinchForce(int index)
{
	if(index >=0)
		mPinchForces[index].mForceEnable =false;

}


void SpringGrid::applyExplosiveForce(PinchForce force)
{
    
	for (int a = 0; a < numOfMasses; a++)	
	{
		Vector3D dist2 = force.mForcePosition - Vector3D(masses[a]->pos.x,masses[a]->pos.y,masses[a]->pos.z);
		float distsq =  dist2.length()*dist2.length();
		
		if (dist2.length() < force.mRadius)
        {
			Vector3D newf = Vector3D(masses[a]->pos.x,masses[a]->pos.y,masses[a]->pos.z) - force.mForcePosition;
			newf = newf / (10000.0f + distsq);
			newf = newf * 100.0f * force.mForceFactor * 50.0f;
			masses[a]->applyForce(newf);	
			//masses[a]->increaseDamping(0.6f);

        }
    }
}


void SpringGrid::simulate(float dt)								//simulate(float dt) is overriden because we want to simulate 
													//the motion of the ropeConnectionPos
{		
	ropeConnectionPos += ropeConnectionVel * dt;	//iterate the positon of ropeConnectionPos
	//forcePosition     += forcePositionVel * dt;	

		
	/*if(mForceEnable) //player force
		applyPinchForce(Vector3D(mForcePosition.x, mForcePosition.y,0.0f), mForceFactor);
	*/
	//other forces
	for (int i=0; i<10;i++)
	{
		if ( mPinchForces[i].mForceEnable)
		{
			if(mPinchForces[i].type==0)
				applyPinchForce(mPinchForces[i]);
			else
				applyExplosiveForce(mPinchForces[i]);
		}

	}



	Simulation::simulate(dt);						//the super class shall simulate the masses


	ropeConnectionPos.z = 0.06f;

	float maxX= ropeConnectionPos.x + ((gridWidth-1)*allSpringLength/2.0);
	float minX= ropeConnectionPos.x - ((gridWidth-1)*allSpringLength/2.0);
	float maxY= -1.0f + ropeConnectionPos.y + ((gridHeight-1)* allSpringLength/2.0);
	float minY= -1.0f + ropeConnectionPos.y - ((gridHeight-1)* allSpringLength/2.0);

	float minZ= ropeConnectionPos.z;

	for (int a = 0; a < numOfMasses; ++a)			//To set the initial positions of masses loop with for(;;)
	{

		if(masses[a]->isFixed)
		{
			masses[a]->pos.x = minX+ (a%gridWidth) * allSpringLength;		//Set x position of masses[a] with springLength distance to its neighbor
			masses[a]->pos.y = minY+ (a/gridWidth) * allSpringLength;			//Set y position as 0 so that it stand horizontal with respect to the ground				
			masses[a]->pos.z = 0;

			
			masses[a]->pos.x  += ropeConnectionPos.x;
			masses[a]->pos.y  += ropeConnectionPos.y;
			masses[a]->pos.z  += ropeConnectionPos.z;
			masses[a]->vel = ropeConnectionVel;			
			
		}
		else
		{				
			if(masses[a]->pos.x >= maxX)
			{	
				masses[a]->pos.x = maxX;
				masses[a]->vel.x  = -masses[a]->vel.x;
			}
			if(masses[a]->pos.x <=minX)
			{
				masses[a]->pos.x = minX;
				masses[a]->vel.x  = -masses[a]->vel.x;
			}					
			if(masses[a]->pos.y >= maxY)
			{
				masses[a]->pos.y = maxY;
				masses[a]->vel.y  = -masses[a]->vel.y;
			}
			if(masses[a]->pos.y <=minY)
			{
				masses[a]->pos.y = minY;
				masses[a]->vel.y  = -masses[a]->vel.y;
			}
			if(masses[a]->pos.z >=minZ)
			{
				masses[a]->pos.z = minZ;
				masses[a]->vel.z  = -masses[a]->vel.z;
			}
		}	
	}
}

void SpringGrid::setRopeConnectionVel(Vector3D ropeConnectionVel)	//the method to set ropeConnectionVel
{
	this->ropeConnectionVel = ropeConnectionVel;
}

