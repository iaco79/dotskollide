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
#include <IFileSystem.h>
#include <IReadFile.h>
#include "GObjectFactory.h"
#include "GObject.h"
#include "GComponent.h"
#include "GController.h"
#include "GSpriteComponent.h"
#include "GTransformComponent.h"
#include "GBox2dComponent.h"
#include "GIrrlichtComponent.h"
#include "GControllerComponent.h"

//GComponentFactory GObjectFactory::mComponentFactory;
GObjectFactory* GObjectFactory::mInstance = NULL;


void GObjectFactory::Initialize(GObjectFactory* objectFactory)
{
	mInstance= objectFactory;
}

GObjectFactory::GObjectFactory(irr::io::IFileSystem* fs) : mFS(fs)
{
	lastObjectId = INVALID_ACTOR_ID;

    mComponentFactory.Register<GSpriteComponent>(GC_SPRITE_IDX,GComponent::GetIdFromName(GSpriteComponent::mName));
    mComponentFactory.Register<GTransformComponent>(GC_TRANSFORM_IDX,GComponent::GetIdFromName(GTransformComponent::mName));
    mComponentFactory.Register<GBox2dComponent>(GC_BOX2D_IDX,GComponent::GetIdFromName(GBox2dComponent::mName));
    mComponentFactory.Register<GIrrlichtComponent>(GC_IRRNODE_IDX,GComponent::GetIdFromName(GIrrlichtComponent::mName));
	mComponentFactory.Register<GControllerComponent>(GC_CONTROLLER_IDX,GComponent::GetIdFromName(GControllerComponent::mName));
 
	
};


std::string GObjectFactory::ReadStringFile(const char* resource)
{
	irr::io::IReadFile* fr = mFS->createAndOpenFile(resource);
	
   if (!fr)
      return std::string("");

   long size = fr->getSize();

   std::string text;
   char *buffer = new char[size+1];
   buffer[size] = 0;

   if ( fr->read(buffer, size) == (unsigned long)size )
      text = buffer;
   delete[] buffer;
	fr->drop();

   return text;

}




GObjectPtr GObjectFactory::CreateObject(const char* resource, GObjectId objectId, bool prototype)
{
	Json::Value jroot;
	Json::Reader jreader;

	std::string text = ReadStringFile(resource);
	bool parsingSuccessful = jreader.parse(text, jroot);
	if (!parsingSuccessful)
    {
        //GCC_ERROR("Failed to create actor from resource: " + std::string(actorResource));
        return NULL;
    }

    // create the actor instance
	GObjectId nextObjectId = objectId;

	if (objectId == 0)
	{
		nextObjectId = GetNextObjectId();
	}
    GObjectPtr gobject = new GObject(nextObjectId);
	gobject->SetIsPrototype(prototype); //stay that this is a prototype
			

    if (!gobject->Init(jroot))
    {
        //GCC_ERROR("Failed to initialize actor: " + std::string(actorResource));
        return NULL;
    }

	Json::Value jresult(-1);

	Json::Value jcomponents = jroot.get("aComponents", jresult);

	if(jcomponents.isInt() && jcomponents.asInt()==-1)
	{
		//no components
		return NULL;

	}

	if(jcomponents.isArray())
	{
		int size = jcomponents.size();

		for ( int i =0; i < size; i++ )
         {
            Json::Value jcomponent =jcomponents[i];

			if(jcomponent.isObject())
			{
				Json::Value::Members  jmembers = jcomponent.getMemberNames();
				bool iscomponent = false;

				 for ( Json::Value::Members::iterator it = jmembers.begin();   it != jmembers.end();  ++it )
				 {
					const std::string &name = *it;

					if(name.compare("sComponentId") == 0)
					{
						iscomponent = true;
						break;
					}
				 }

				 if(iscomponent)
				 {
					GComponentPtr gcomponent = VCreateComponent(jcomponent,gobject);

					if (gcomponent)
					{
						
						gobject->AddComponent(gcomponent);
					//	gcomponent->SetOwner(gobject);
					}
					else
					{
						// If an error occurs, we kill the actor and bail.  We could keep going, but the actor is will only be 
						// partially complete so it's not worth it.  Note that the pActor instance will be destroyed because it
						// will fall out of scope with nothing else pointing to it.
						return NULL;
					}
				 
				 }
				 else
				 {
					return NULL;
				 }

			}

         }

	}
	else
	{
		return NULL;
	}

	
	if(!prototype)
		gobject->PostInit(); //  non-prototype/non-cloned objects are initialized automatically
	
    return gobject;
}


irr::core::stringc GObjectFactory::GetResourceByPrototype(GObjectPrototypeId id)
{
	irr::core::stringc resourcePath;

	switch((GAME_OBJECT_PROTOTYPES)id)
	{
		case GP_TEST:
			resourcePath = "IrrlichtSdcard/TestDefiniton.json";
		break;
	}

	return resourcePath;

}

void GObjectFactory::SetPrototype(GObjectPrototypeId prototypeId,GObjectPtr gobject)
{
	GObjectPrototypesIterator findIt = prototypes.find(prototypeId);    

	if (findIt == prototypes.end())
    {
		prototypes.insert(std::make_pair(prototypeId, gobject));
    }
	else
	{
		findIt->second = gobject;
	}
}

GObjectPtr GObjectFactory::CreateObjectFromPrototype(GObjectPrototypeId id, GObjectId objectId) //, bool prototype)
{
	bool createObject = false;

	//if(prototype)
	{
		GObjectPrototypesIterator findIt = prototypes.find(id);    
		
        if (findIt != prototypes.end())
        {
			GObjectPtr prototype  = findIt->second;
            return CloneObject(prototype,objectId);
        }
        else
        {
			createObject = true;
        }
	}
	
	if(createObject)
	{	
		//if(prototype)
		{
			GObjectPtr prototype = CreateObject(GetResourceByPrototype(id).c_str(),objectId, prototype);

			if(prototype)
			{
				prototypes.insert(std::make_pair(id, prototype));
				return  CloneObject(prototype,objectId);
			}
			
		}
	}

	return NULL;
		/*
		else
		{
			return CreateObject(GetResourceByPrototype(id).c_str(),objectId, false);			
			
		}
		*/
//	}
}




GComponentPtr GObjectFactory::VCreateComponent(Json::Value json, GObjectPtr gobject)
{
	const std::string &name = json["sComponentId"].asString();
	GComponentId componentId = GComponent::GetIdFromName(name);

	GComponentPtr gcomponent(mComponentFactory.Create(componentId));

    // initialize the component if we found one
    if (gcomponent)
    {
		gcomponent->gcomponentId = componentId; 
		gcomponent->SetOwner(gobject);
        if (!gcomponent->VInit(json))
        {
            //GCC_ERROR("Component failed to initialize: " + std::string(name));
            return NULL;
        }
    }
    else
    {
        //GCC_ERROR("Couldn't find ActorComponent named " + std::string(name));
        return NULL;
    }

    // pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a 
    // custom CreateComponent() function in a sub class.
    return gcomponent;

}

GObjectPtr GObjectFactory::CreateEmptyObject(int type, int subtype, bool prototype )
{
	GObjectId nextObjectId = GetNextObjectId();
	GObjectPtr gobject = new GObject(nextObjectId);
	gobject->mType = type;
	gobject->mSubType = subtype;


	gobject->SetIsPrototype(prototype);
	gobject->SetIsCloned(false);

	return gobject;	
}

GObjectPtr GObjectFactory::CloneObject(GObjectPtr other, const GObjectId gObjectId)
{
	// create the actor instance
	GObjectId nextObjectId = gObjectId;

	if (gObjectId == 0)
	{
		nextObjectId = GetNextObjectId();
	}

	GObjectPtr gobject = new GObject(nextObjectId);
	gobject->SetIsCloned(true);
	*gobject = *other; //assignment operator

	if(other->IsPrototype())
		gobject->mPrototype = other;

    for (GObject::GComponents::iterator it = other->mComponents.begin(); it != other->mComponents.end(); ++it)
    {
		GComponentPtr gcomponent = mComponentFactory.Copy(it->second->VGetId(),it->second);
		
		if (gcomponent)
		{
			gobject->AddComponent(gcomponent);
			gcomponent->SetOwner(gobject);
		}
	}



	return gobject;
}
