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
#ifndef TSHIELDCONTROLLER_CLASS
#define TSHIELDCONTROLLER_CLASS

#include "GameDefines.h"
#include "GController.h"


/*
 The controller component is an abstract  component 
 that handles the behaviour and state of a particular gameobject
*/
class TShieldController : public GController
{
public:
	
	SHIELD_STATE mState;
	int mPower;
	int mFixers;

	typedef b2Vec2 (*PositionFunction)(irr::u32 time);

	class ShieldVertex
	{
		private:
		
			irr::u32 mTime;
			irr::u32 mMaxTime;
			PositionFunction mFT;
			
		public:
			b2Vec2 mPosition;
			ShieldVertex();
			void Init(irr::u32 initialTime, irr::u32 maxTime, PositionFunction ft);
			void SetTime(irr::u32 time);
			void Update(irr::u32 delta);
	};


private:
	int mElementType;	
	
	irr::u32 mTimer;


public:

	TShieldController();
	TShieldController& operator= (const TShieldController& other );
	virtual void VUpdate(int deltaMs);

	virtual bool VInit(const Json::Value& jcomponent) ;
	void VInit(irr::u32 shieldLenghtTime, irr::u32 shieldPeriodTime, PositionFunction ft);
	void SetTime (irr::u32 time) ;
	

	ShieldVertex mShieldVertexes[2];

	//f(t) shield path function, time is in ms
	static b2Vec2 ShieldPathFunction (irr::u32 time);



	static const char* mName;
};

#endif