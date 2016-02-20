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
#include "GameDefines.h"
#include "GObject.h"
#include "GComponent.h"
#include "GObjectFactory.h"

GObject::GObject(GObjectId id)
{
	mId = id;
    mType = 0;
	mSubType = 0;
	mIsPrototype = false;
	mIsCloned = false;
	mPrototype = NULL;
	
	mStdComponents[GC_TRANSFORM_IDX] =false;
	mStdComponents[GC_SPRITE_IDX] =false;
	mStdComponents[GC_BOX2D_IDX] =false;
	mStdComponents[GC_IRRNODE_IDX] =false;
	mStdComponents[GC_CONTROLLER_IDX] =false;


}

GObject::~GObject(void)
{

	
}
 
bool GObject::Init(Json::Value& json)
{   
	//m_type = pData->Attribute("type");
	//m_resource = pData->Attribute("resource");
    return true;
}

void GObject::PostInit(void)
{

    //init components in specific order
	for(int i=0;i<GC_SIZE;i++)
	{
		if (mStdComponents[i])
		{
			GComponents::iterator findIt = mComponents.find( GetComponentIdByIndex(i));
			if (findIt != mComponents.end())
			{
				GComponentPtr component = findIt->second;
				component->VPostInit();
				component->mFrameUpdated=true;
			}			
		}
	}


	//init the rest of the components
    for (GComponents::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
    {
		if(!it->second->mFrameUpdated)
			it->second->VPostInit();			
		
		it->second->mFrameUpdated= false;
    }

}



void GObject::Destroy(void)
{

	
	for (GComponents::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
    {
        delete it->second;
    }

    mComponents.clear();
}


void GObject::Update(int deltaMs)
{
	//update components in specific order
	for(int i=0;i<GC_SIZE;i++)
	{
		if (mStdComponents[i])
		{
			GComponents::iterator findIt = mComponents.find( GetComponentIdByIndex(i));
			if (findIt != mComponents.end())
			{
				GComponentPtr component = findIt->second;
				component->VUpdate(deltaMs);
				component->mFrameUpdated=true;
			}			
		}
	}


	//update the rest of the components
    for (GComponents::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
    {
		if(!it->second->mFrameUpdated)
			it->second->VUpdate(deltaMs);			
		
		it->second->mFrameUpdated= false;
    }
}

void GObject::AddComponent(GComponentPtr component)
{
	
    std::pair<GComponents::iterator, bool> success = mComponents.insert(std::make_pair(component->VGetId(), component));

	//set the availability of the added component
	for(int i=0;i<GC_SIZE;i++)
	{
		if (component->VGetId() == GetComponentIdByIndex(i))
		{
			mStdComponents[i]=true;
		}
	}

}


//this should be called from the ObjectFactory
GObject& GObject::operator= (const GObject& other )
{
	mType = other.mType;
	mSubType= other.mSubType;
	
	return *this;
}


GComponentId GObject::GetComponentIdByIndex(int idx)
{
	
	GComponentId id = GObjectFactory::getInstance()->mComponentFactory.mIndexesForIds[idx];
	return id;

}