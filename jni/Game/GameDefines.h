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
#ifndef GGAME_DEFINES
#define GGAME_DEFINES
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <irrlicht.h>
#include <Box2D.h>
#include "json/json.h"
#include <IFileSystem.h>
#include <IReadFile.h>
#include "GGameApp.h"

#define  GC_MAX_SIZE  10
#define  GN_MAX_SIZE  20
#define  GCT_MAX_SIZE 10
#define  PROT_MAX_SIZE 10


//forward declarations
class GObject;
class GComponent;
class GObjectFactory;

typedef unsigned int GObjectId;
typedef unsigned long GComponentId;
typedef unsigned int GObjectPrototypeId;
typedef unsigned int GObjecType;
typedef unsigned int GObjecSubType;
typedef unsigned int GNodeFactoryId;
typedef unsigned int GControllerId;

typedef GObject* GObjectPtr;
typedef GComponent* GComponentPtr;


const GObjectId INVALID_ACTOR_ID = 0;
const GComponentId INVALID_COMPONENT_ID = 0;

enum GAME_OBJECT_PROTOTYPES
{	
	GP_TEST=0,
	GP_SIZE	
};

//components priority (Update order)
enum GAME_COMPONENT_IDX
{
	GC_CONTROLLER_IDX=0, //controller can modify the state of any other component
	GC_BOX2D_IDX, //world updates box2d objects
	GC_TRANSFORM_IDX,//transform is updated using box2d component as input
	GC_SPRITE_IDX,	//the sprite node is sometimes updated by the controller/game logic
	GC_IRRNODE_IDX,	 //irr node needs the transform as input
	GC_SIZE
};

enum GAME_NODES_IDX
{
	GN_BOX2D_IDX=0,
	GN_SPRITE_IDX,
	GN_SIZE
};

/*enum GAME_CONTROLLERS_IDX
{
	GCT_SIZE=0
};
*/


//warning this is initialized in the GameObjectFactory constructor


template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction(void) { return new SubType; }

template <class BaseType, class SubType>
BaseType* GenericObjectCopyFunction(BaseType* other) 
			{ 
				SubType* copy = new SubType;
				SubType* otherDer = static_cast<SubType*>(other);
				*(copy) = *otherDer; //call the asignment operator 

				return copy; 
			}


template <class BaseClass, class IdType,size_t MAX_CREATION_FUNCTIONS>
class GenericObjectFactory
{
    typedef BaseClass* (*ObjectCreationFunction)(void);
    std::map<IdType, ObjectCreationFunction> m_creationFunctions;
	typedef typename std::map<IdType, ObjectCreationFunction>::iterator ObjectCreationIterator;

	typedef BaseClass* (*ObjectCopyFunction)(BaseClass* other);
    std::map<IdType, ObjectCopyFunction> m_copyFunctions;
	typedef typename std::map<IdType, ObjectCopyFunction>::iterator ObjectCopyIterator;

	


public:
	//this way we can access a factory really fast using a secuential well known index 
	IdType mIndexesForIds[MAX_CREATION_FUNCTIONS];


    template <class SubClass>
    bool Register(int index, IdType id)
    {
        
		ObjectCreationIterator findIt = m_creationFunctions.find(id);
        if (findIt == m_creationFunctions.end())
        {
			mIndexesForIds [index]  = id;
			
            m_creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;  // insert() is giving me compiler errors
//			return true;
        }
		ObjectCopyIterator findIt2 = m_copyFunctions.find(id);
        if (findIt2 == m_copyFunctions.end())
        {
            m_copyFunctions[id] = &GenericObjectCopyFunction<BaseClass, SubClass>;  // insert() is giving me compiler errors
//			return true;
        }


        return true;
    }

    BaseClass* Create(IdType id)
    {
        ObjectCreationIterator  findIt = m_creationFunctions.find(id);
        if (findIt != m_creationFunctions.end())
        {
            ObjectCreationFunction pFunc = findIt->second;
            return pFunc();
        }

        return NULL;
    }

	BaseClass* Copy(IdType id,BaseClass* other)
    {
        
		ObjectCopyIterator findIt = m_copyFunctions.find(id);

        if (findIt != m_copyFunctions.end())
        {
            ObjectCopyFunction pFunc = findIt->second;
            return pFunc(other);
        }

        return NULL;
    }
};



#endif