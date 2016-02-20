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
#include "GNodeFactory.h"
#include "GObject.h"
#include "GTransformComponent.h"
#include "GIrrlichtComponent.h"

const char* GIrrlichtComponent::mName = "IrrlichtNode";

GIrrlichtComponent::GIrrlichtComponent()
{
	gcomponentId = GComponent::GetIdFromName(GIrrlichtComponent::mName);
	mNode= NULL;
}


bool GIrrlichtComponent::VInit(irr::scene::ISceneNode* node) 
{
	mNode = node;

	return true;
}

void GIrrlichtComponent::VUpdate(int deltaMs) 
{ 
	if(mNode && mGameObject)
	{
		GComponentId transformId = mGameObject->GetComponentIdByIndex(GC_TRANSFORM_IDX);
		GTransformComponent* transform = mGameObject->GetComponent<GTransformComponent>(transformId);

		if(transform)
		{
			
			mNode->setPosition(irr::core::vector3df(transform->mTrans.p.x,transform->mTrans.p.y,transform->mZ));
			mNode->setRotation(irr::core::vector3df(0.0f,0.0f, transform->mTrans.q.GetAngle()* 180.0f/ 3.1416f));

		}
	}
}

//The scene Node has to be the very last component to be intialized
//since it depends on the rest of the components
//to get information about how to render the object
bool GIrrlichtComponent::VInit(Json::Value& jcomponent)
{
	GNodeFactory* nodeFactory = GGameApp::getInstance()->getNodeFactory(); //get from context

	if(mGameObject->IsPrototype())
		mjcomponent = jcomponent; //if this is a prototype, then backup the json definition

	if(!mGameObject->IsPrototype() && !mGameObject->IsCloned())
	{
		nodeFactory->CreateSceneNode(jcomponent);
	}

	return true;
}

GIrrlichtComponent& GIrrlichtComponent::operator= (const GIrrlichtComponent& other )
{
	GComponent::operator= (other); //call base assignment operator
	GNodeFactory* nodeFactory = GGameApp::getInstance()->getNodeFactory();//get from context

	nodeFactory->CreateSceneNode(other.mjcomponent); //, mGameObject);

	return *this;
}
