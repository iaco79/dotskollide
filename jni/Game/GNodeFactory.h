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
#ifndef GNODEFACTORY_CLASS
#define GNODEFACTORY_CLASS

#include "GameDefines.h"

template <class IdType, size_t MAX_CREATION_FUNCTIONS>
class GenericNodeFactory
{

	typedef irr::scene::ISceneNode* (*NodeCreationFunction)(irr::scene::ISceneManager* smgr, 
												
												const Json::Value& json);
	    
    std::map<IdType, NodeCreationFunction> m_creationFunctions;
	typedef typename std::map<IdType, NodeCreationFunction>::const_iterator NodeCreationIterator;


public:
	//this way we can access a factory really fast using a secuential well known index 
	IdType mIndexesForIds[MAX_CREATION_FUNCTIONS];

	bool Register(int index, IdType id, NodeCreationFunction fn)
    {
		NodeCreationIterator findIt = m_creationFunctions.find(id);
        if (findIt == m_creationFunctions.end())
        {
			mIndexesForIds [index]  = id;
            m_creationFunctions[id] = fn;  // insert() is giving me compiler errors
			
        }
		
		return true;
    }

    irr::scene::ISceneNode* Create(IdType id,irr::scene::ISceneManager* smgr,
						const Json::Value& json)
    {
        
		NodeCreationIterator findIt = m_creationFunctions.find(id);

        if (findIt != m_creationFunctions.end())
        {
            NodeCreationFunction pFunc = findIt->second;
            return pFunc(smgr,  json);
        }

        return NULL;
    }
};


class GNodeFactory 
{	
	typedef GenericNodeFactory<GNodeFactoryId, GN_MAX_SIZE> GenericFactoryMap;

	GenericFactoryMap factoryMap;

private:
	irr::scene::ISceneManager* mSceneMgr;


public:

	//create the scene node for this object
	irr::scene::ISceneNode* CreateSceneNode(const Json::Value& json);

	GNodeFactory(irr::scene::ISceneManager* smgr);
	~GNodeFactory();

};

#endif