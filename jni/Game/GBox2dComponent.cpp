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
#include <irrlicht.h>
#include "GameDefines.h"
#include "GObject.h"
#include "GTransformComponent.h"
#include "GBox2dComponent.h"


const char* GBox2dComponent::mName = "Box2d";



//The Gbox2dcomponent depends on the GTransformComponent 
//it is very imporant to set the initial transformation when creating the body
//box2d is more efficient if the position and angle for the body are already defined
//before creating the fixtures since fixture definition coordinates are relative to the body transformation.

//Creating fixtures with the body at world origin and then translating the body to its final positon
//will result in a severe performance penalty and inadecuate physics simulation
GBox2dComponent::GBox2dComponent()
{
	gcomponentId = GComponent::GetIdFromName(GBox2dComponent::mName);
	mWorld = NULL;
	mBody = NULL;
}
bool GBox2dComponent::VInit(b2World* world, b2BodyDef& bodyDef, FixtureDefinitionsList& fixtureDefinitionList)
{
	mWorld = world;
	mBodyDef = bodyDef;
	mFixtureDefs = fixtureDefinitionList;

	

	return true;
}
bool GBox2dComponent::VInit(Json::Value& jcomponent)
{
	
	mWorld = GGameApp::getInstance()->getCurrentBox2dWorld();



	if(	mGameObject->IsPrototype() || 
	  (!mGameObject->IsPrototype() && !mGameObject->IsCloned()))
	{
		Json::Value jresult(-1);
		Json::Value jfixtures = jcomponent.get("aFixtures", jresult);

		mBodyDef.type = GetBodyTypeFromStr(jcomponent["iBodyType"].asString().c_str());
			
		if(jfixtures.isArray())
		{
			int size = jfixtures.size();

			for (int i =0; i < size; i++ )
			{
				Json::Value jfixture =jfixtures[i];

				b2FixtureDef fd = CreateFixtureDefinition(jfixture);
				mFixtureDefs.push_back(fd);
			}
		}	

		
		
	}		
							
	return true;
}


void GBox2dComponent::VPostInit()
{
	if(!mGameObject->IsPrototype() && mWorld)
	{
		GComponentId transformId = mGameObject->GetComponentIdByIndex(GC_TRANSFORM_IDX);
		GTransformComponent* transform = mGameObject->GetComponent<GTransformComponent>(transformId);

		float x = transform->mTrans.p.x;
		float y = transform->mTrans.p.y;
			
		mBodyDef.position.Set(x,y);
		 	
		if(mGameObject->IsCloned() && mGameObject->GetPrototype()!=NULL)
		{   //if this is a cloned object, then we must retrieve the fixtures definitions from the prototype
			
			GObjectPtr proto = mGameObject->GetPrototype();

			GBox2dComponent* pbcomp = proto->GetComponent<GBox2dComponent>(proto->GetComponentIdByIndex(GC_BOX2D_IDX));		

			if(pbcomp)
			{
				mBody = mWorld->CreateBody(&mBodyDef);
		
				if(mBody)
				{
					mBody->SetUserData((void*)mGameObject);

					for (int i =0; i < pbcomp->mFixtureDefs.size(); i++ )
					{
						mBody->CreateFixture(&pbcomp->mFixtureDefs[i]);
					}
				}
			}
		}
		else
		{
			mBody = mWorld->CreateBody(&mBodyDef);
		
			if(mBody)
			{
				mBody->SetUserData((void*)mGameObject);

				for (int i =0; i < mFixtureDefs.size(); i++ )
				{
					mBody->CreateFixture(&mFixtureDefs[i]);
				}
			}
		
		}
	}

}

GBox2dComponent& GBox2dComponent::operator= (const GBox2dComponent& other )
{
	GComponent::operator= (other); //call base assignment operator
	GComponentId transformId = other.mGameObject->GetComponentIdByIndex(GC_TRANSFORM_IDX);
	GTransformComponent* transform = other.mGameObject->GetComponent<GTransformComponent>(transformId);
	mWorld = GGameApp::getInstance()->getCurrentBox2dWorld();	
	mBodyDef = other.mBodyDef;

	
	return *this;
}


b2BodyType GBox2dComponent::GetBodyTypeFromStr(const char* bodyType)
{
	if(std::strcmp(bodyType,"Static")==0)
	{
		return b2_staticBody;
	}
	else
	if(std::strcmp(bodyType,"Kinematic")==0)
	{
		return b2_kinematicBody;
	}
	
	return b2_dynamicBody;

}
b2FixtureDef GBox2dComponent::CreateFixtureDefinition(Json::Value& jfixture)
{

	b2FixtureDef fd;
	std::string fixtureType = jfixture["sShape"].asString();

	fd.density = (float)jfixture["fDensity"].asDouble();
	fd.friction = (float)jfixture["fFriction"].asDouble();
	fd.restitution = (float)jfixture["fRestitution"].asDouble();
	
	if(fixtureType.compare("Polygon")==0)
	{
		b2PolygonShape* shape =  new b2PolygonShape();
		Json::Value jvertices= jfixture["aVertices"];
		int count = jvertices.size();

		if(count>=3 )
		{
			
			b2Vec2* vec = new b2Vec2[count];

			for(int i=0;i<count;i++)
			{
				Json::Value jv = jvertices[i];

				vec[i].Set((float)jv[0u].asDouble(),
						   (float)jv[1].asDouble());
			
			}

			shape->Set(vec,count);

			delete []vec;

		}
		
		fd.shape = shape;
	}
	else
	if(fixtureType.compare("Square")==0)
	{
		b2PolygonShape* shape =  new b2PolygonShape();
		shape->SetAsBox((float)jfixture["fWidth"].asDouble(),(float)jfixture["fHeight"].asDouble());
		fd.shape = shape;

	}
	else
	if(fixtureType.compare("Edge")==0)
	{
		b2EdgeShape* shape =  new b2EdgeShape();
		Json::Value jvertices= jfixture["aVertices"];

		if(jvertices.size()>=2)
		{
			Json::Value v1 = jvertices[0u];
			Json::Value v2 = jvertices[1];

			shape->Set( b2Vec2((float)v1[0u].asDouble(),(float)v1[1].asDouble()),
					    b2Vec2((float)v2[0u].asDouble(),(float)v2[1].asDouble())); 
			
		}
	
		fd.shape = shape;
	}
	else
	if(fixtureType.compare("Circle")==0)
	{

		b2CircleShape* shape =  new b2CircleShape();
		shape->m_radius= (float)jfixture["fRadius"].asDouble();
		
		fd.shape = shape;
		
	}
	
	return fd;
}

void GBox2dComponent::VUpdate(int deltaMs) 
{
	GComponentId compId = mGameObject->GetComponentIdByIndex(GC_TRANSFORM_IDX);
	GTransformComponent* transform = mGameObject->GetComponent<GTransformComponent>(compId);

	if(transform && mBody)
	{
		transform->mTrans = mBody->GetTransform();
	}
}

GBox2dComponent::~GBox2dComponent(void)
{

	if(	mGameObject->IsPrototype() || 
	  (!mGameObject->IsPrototype() && !mGameObject->IsCloned()))
	{
			 	
		for (int i =0; i < mFixtureDefs.size(); i++ )
		{
			
			switch(mFixtureDefs[i].shape->GetType())
			{
			case b2Shape::e_circle:
				{
				b2CircleShape* shape = (b2CircleShape*)mFixtureDefs[i].shape;
				delete shape;
				mFixtureDefs[i].shape = NULL;
				}
				break;
			case b2Shape::e_edge:
				{
				b2EdgeShape* shape = (b2EdgeShape*)mFixtureDefs[i].shape;				
				delete shape;
				mFixtureDefs[i].shape = NULL;
				}
				break;
			case b2Shape::e_polygon:
				{
				b2PolygonShape* shape = (b2PolygonShape*)mFixtureDefs[i].shape;				
				delete shape;
				mFixtureDefs[i].shape = NULL;
				}
				break;
			}
		}


		mFixtureDefs.clear();

	

	}

	if(!mGameObject->IsPrototype() && mBody)
	{

		mBody->SetUserData(NULL);

		mWorld->DestroyBody(mBody);
		mWorld = NULL;
	}

}
