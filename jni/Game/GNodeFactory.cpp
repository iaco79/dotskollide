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
#include "GComponent.h"

//builds a sprite node
irr::scene::ISceneNode* GNodeFactory_CreateSpriteSceneNode(irr::scene::ISceneManager* smgr, 
														   
														  const Json::Value& json)
{
	return NULL;
}

//builds a box2d node
irr::scene::ISceneNode* GNodeFactory_CreateBox2dSceneNode(irr::scene::ISceneManager* smgr, 
														  
														  const Json::Value& json)
{
	return NULL;
}


GNodeFactory::GNodeFactory(irr::scene::ISceneManager* smgr): mSceneMgr(smgr)
{

	factoryMap.Register(GN_BOX2D_IDX,GComponent::GetIdFromName("Box2dNode"),GNodeFactory_CreateBox2dSceneNode);
    factoryMap.Register(GN_SPRITE_IDX,GComponent::GetIdFromName("SpriteNode"),GNodeFactory_CreateSpriteSceneNode);
    
}


irr::scene::ISceneNode* GNodeFactory::CreateSceneNode(const Json::Value& json)
{
	std::string nodeName = json["sNode"].asString();
	GNodeFactoryId id = GComponent::GetIdFromName(nodeName);

	return factoryMap.Create(id, mSceneMgr, json);

}

GNodeFactory::~GNodeFactory()
{

}

