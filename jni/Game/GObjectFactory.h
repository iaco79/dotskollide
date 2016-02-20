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
#ifndef GOBJECTFACTORY_CLASS
#define GOBJECTFACTORY_CLASS

#include "GameDefines.h"

namespace irr
{
    namespace io
    {
        class IFileSystem;
    }
}

typedef GenericObjectFactory<GComponent, GComponentId, GC_MAX_SIZE> GComponentFactory;

class GObjectFactory
{
	
private:
	GObjectId lastObjectId;
	irr::io::IFileSystem* mFS;
	std::string ReadStringFile(const char* resource);
	typedef std::map<GObjectPrototypeId, GObjectPtr> GObjectPrototypes;
	typedef std::map<GObjectPrototypeId, GObjectPtr>::iterator GObjectPrototypesIterator;
	static GObjectFactory* mInstance;

	irr::core::stringc GetResourceByPrototype(GObjectPrototypeId id);

protected:
	GObjectPrototypes prototypes; 
    
public:
	GComponentFactory mComponentFactory;


	GObjectFactory(irr::io::IFileSystem* fs);
	GObjectPtr CreateEmptyObject(int type, int subtype, bool prototype = false);

    GObjectPtr CreateObject(const char *resource, const GObjectId gObjectId,bool prototype = false);
	GObjectPtr CreateObjectFromPrototype(GObjectPrototypeId id, const GObjectId gObjectId = 0);

	GObjectPtr CloneObject(GObjectPtr other, const GObjectId gObjectId);

	void SetPrototype(GObjectPrototypeId prototypeId,GObjectPtr gobject);
	GObjectId GetNextObjectId(void) { ++lastObjectId; return lastObjectId; }
	
	
//protected:
    // [rez] This function can be overridden by a subclass so you can create game-specific C++ components.  If you do
    // this, make sure you call the base-class version first.  If it returns NULL, you know it's not an engine component.
    virtual GComponentPtr VCreateComponent(Json::Value json,GObjectPtr gobject);

	static GObjectFactory* getInstance() {return mInstance;}
	static void Initialize(GObjectFactory* objectFactory);

};

#endif
