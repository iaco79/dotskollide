
#ifndef SPRINGGRID_CLASS
#define SPRINGGRID_CLASS
#include "springrid/SpringGridMath.h"

namespace springrid 
{
	class Spring											//An object to represent a spring with inner friction binding two masses. The spring 
															//has a normal length (the length that the spring does not exert any force)
	{
	public:
		Mass* mass1;										//The first mass at one tip of the spring
		Mass* mass2;										//The second mass at the other tip of the spring

		float springConstant;								//A constant to represent the stiffness of the spring
		float springLength;									//The length that the spring does not exert any force
		float frictionConstant;								//A constant to be used for the inner friction of the spring
	

		Spring(Mass* mass1, Mass* mass2, 
			float springConstant, float springLength, float frictionConstant)		//Constructor
		{
			this->springConstant = springConstant;									//set the springConstant
			this->springLength = springLength;										//set the springLength
			this->frictionConstant = frictionConstant;								//set the frictionConstant

			mass1->container = (void*)this;
			mass2->container = (void*)this;

			this->mass1 = mass1;													//set mass1
			this->mass2 = mass2;													//set mass2
		}

		void solve()																	//solve() method: the method where forces can be applied
		{
			Vector3D springVector = mass1->pos - mass2->pos;							//vector between the two masses
		
			float r = springVector.length();											//distance between the two masses

			if (r <= springLength)
				return;

			Vector3D force;																//force initially has a zero value
		
			if (r != 0)																	//to avoid a division by zero check if r is zero
				force += (springVector / r) * (r - springLength) * (-springConstant);	//the spring force is added to the force

			force += -(mass1->vel - mass2->vel) * frictionConstant;						//the friction force is added to the force
																						//with this addition we obtain the net force of the spring
			mass1->applyForce(force);													//force is applied to mass1
			mass2->applyForce(-force);													//the opposite of force is applied to mass2
		}

	};

	
	class SpringGrid : public Simulation	//simulation of a grid of springs
	{
	public:
		Spring** springs;									//Springs binding the masses (there shall be [numOfMasses - 1] of them)

		Vector3D gravitation;								//gravitational acceleration (gravity will be applied to all masses)

		Vector3D ropeConnectionPos;							//A point in space that is used to set the position of the 
															//first mass in the system (mass with index 0)
	
		Vector3D ropeConnectionVel;							//a variable to move the ropeConnectionPos (by this, we can swing the rope)

		/*
		Vector3D mForcePosition;	
		float mForceFactor;
		bool mForceEnable;
			*/

		struct PinchForce
		{
			int mIndex;
			Vector3D mForcePosition;	
			float mForceFactor;
			bool mForceEnable;
			float mRadius;
			int type;

		};

	//	Vector3D forcePositionVel;											
	
	


		float groundRepulsionConstant;						//a constant to represent how much the ground shall repel the masses
	
		float groundFrictionConstant;						//a constant of friction applied to masses by the ground
															//(used for the sliding of rope on the ground)
	
		float allSpringLength;
		float groundAbsorptionConstant;						//a constant of absorption friction applied to masses by the ground
															//(used for vertical collisions of the rope with the ground)
	
		float groundHeight;									//a value to represent the y position value of the ground
															//(the ground is a planer surface facing +y direction)
		int   gridWidth;
		int   gridHeight;
		float allFrictionConstant;							//a constant of air friction applied to masses
		int   springCount;  //IACO add spring count
		bool applyForceTest;  //IACO add test force
		bool applyExtraForce;  //IACO add test force
		float allSpringConstant;
		PinchForce mPinchForces[10];
		

		
		SpringGrid(										//a long long constructor with 11 parameters starts here
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
		);
	
		Mass* getMass(int i,int j);
		float getSpringLength();
		int getMassCountW();
		int getMassCountH();
		int getMassCount();
		float getTotalWidth();
		float getTotalHeight();
		void release();										//release() is overriden because we have springs to delete
		void solve();										//solve() is overriden because we have forces to be applied
		void simulate(float dt);								//simulate(float dt) is overriden because we want to simulate 
		void setRopeConnectionVel(Vector3D ropeConnectionVel);	//the method to set ropeConnectionVel
		void applyPinchForce(PinchForce force);

		void applyExplosiveForce(PinchForce force);


		int SetPinchForce(PinchForce force);
		void  UnsetPinchForce(int index);


	
		 


	};
}

#endif