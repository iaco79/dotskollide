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
#ifndef GBOX2DCOMPONENT_CLASS
#define GBOX2DCOMPONENT_CLASS

#include "GameDefines.h"
#include "GComponent.h"

/*
*/
class GBox2dComponent : public GComponent
{

private: 
	b2BodyType GetBodyTypeFromStr(const char* bodyType);
	b2FixtureDef CreateFixtureDefinition(Json::Value& jfixture);
	
public:
	virtual ~GBox2dComponent(void);

	GBox2dComponent();

	typedef std::vector<b2FixtureDef> FixtureDefinitionsList;

	b2Body* mBody; 
	b2World* mWorld;
	b2BodyDef mBodyDef;

	FixtureDefinitionsList mFixtureDefs; //contains the fixture definitions (if this is a prototype)

	virtual GBox2dComponent& operator= (const GBox2dComponent& other);

	static const char* mName;
	virtual const char* VGetName() const { return mName; }

	virtual bool VInit(Json::Value& jcomponent) ;
	virtual void VPostInit(void) ;
	bool VInit(b2World* world, b2BodyDef& bodyDef, FixtureDefinitionsList& fixtureDefinitionList);
	virtual void VUpdate(int deltaMs) ;
 
};

#endif