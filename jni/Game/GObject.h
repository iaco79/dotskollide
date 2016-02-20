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
#ifndef GOBJECT_CLASS
#define GOBJECT_CLASS

#include "GameDefines.h"


class GObject
{
	friend class GObjectFactory;

public:

	typedef std::map<GComponentId, GComponentPtr> GComponents;

private:
	GObjectId mId;
	GComponents mComponents;
	GObjecType mType;
	GObjecSubType mSubType;
	bool mIsPrototype;
	bool mIsCloned;
	bool mStdComponents[GC_SIZE];

	GObjectPtr mPrototype; 

	void SetIsPrototype(bool value ) {mIsPrototype = value;}
	void SetIsCloned(bool value ) {mIsCloned = value;}

public:

	explicit GObject(GObjectId id);
	
	bool IsPrototype() {return mIsPrototype;}
	bool IsCloned() {return mIsCloned;}


	bool Init(Json::Value& json);
    void PostInit(void);
    void Destroy(void);
    void Update(int deltaMs);

	 GObjecType GetId(void) const { return mId; }
     GObjecSubType GetType(void) const { return mType; }
	 GObjecSubType GetSubType(void) const { return mSubType; }

	 GObjectPtr GetPrototype() {return mPrototype;}

	~GObject(void);


	// template function for retrieving components
	GComponentId GetComponentIdByIndex(int idx);


    template <class ComponentType>
    ComponentType* GetComponent(GComponentId id)
    {
        GComponents::iterator findIt = mComponents.find(id);
        if (findIt != mComponents.end())
        {
            GComponentPtr component = findIt->second;

            return dynamic_cast<ComponentType*>(component);  // return the weak pointer
        }
        else
        {
            return NULL;
        }
    }

	const GComponents* GetComponents() { return &mComponents; }

	//asignment operator
	GObject& operator= (const GObject& other );


    void AddComponent(GComponentPtr pComponent);

};

#endif //GOBJECT_CLASS