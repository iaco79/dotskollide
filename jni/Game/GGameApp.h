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
#ifndef GGAMEAPP_CLASS
#define GGAMEAPP_CLASS

#include <Box2D.h>
#include <irrlicht.h>
#include "menu/GRocketEvent.h"
#include "menu/GRocketEventHandler.h"
#include "scripting/GScriptable.h"
#include "scripting/GScriptManager.h"
#include "GEventManager.h"
#include "Audio/AudioManager.h"
#include <Rocket/Core.h>
#include "rocketgui/IrrRocketInterface.h"
#include "rocketgui/IrrRocketFileInterface.h"
#include "rocketgui/IrrRocketRenderer.h"
#include "rocketgui/CIrrRocketGUI.h"

class GGameManager;
class GControllerFactory;
class GNodeFactory;
class GObjectFactory;
class GScriptManager;
class PreferencesManager;
class GGameApp  
{
	friend class GGameManager;

public:
	
	GGameApp();


	static GGameApp* getInstance() {return mInstance;}

	static void Initialize(GGameApp* instance);

	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual bool Render() = 0;
	virtual void Init() = 0;
	virtual void Finish() = 0;
	virtual void ReleaseRenderContext() {;}
	virtual void ReInitRenderContext() {;}


	GEventManager* getEventManager() {return mEventManager;}
	irr::IrrlichtDevice* getIrrlichtDevice() {return mDevice;}
	AudioManager* getAudioManager() {return mAudioManager;}
	CIrrRocketGUI* getRocketGUI() {return mRocketGUI;} 
	b2World* getCurrentBox2dWorld() {return mBox2dWorld;} 
	void setCurrentBox2dWorld(b2World* world) {mBox2dWorld = world;} 
	GControllerFactory* getControllerFactory() {return  mControllerFactory;}
	GNodeFactory* getNodeFactory() {return mNodeFactory;}
	GObjectFactory* getObjectFactory() {return mObjectFactory;}
	GScriptManager* getScriptManager() {return mScriptManager;}
	PreferencesManager* getPreferencesManager() {return mPreferencesManager;}
	static GGameApp* mInstance; //the app instance
	GEventManager*   mEventManager; 
	GGameManager*   mGameManager; 
	irr::IrrlichtDevice*  mDevice;
	AudioManager* mAudioManager;
	CIrrRocketGUI* mRocketGUI;
	irr::u32 mLastTime; 
	b2World* mBox2dWorld; 
	GControllerFactory* mControllerFactory;
	GNodeFactory* mNodeFactory;
	GObjectFactory* mObjectFactory;
	GScriptManager* mScriptManager;
	PreferencesManager* mPreferencesManager;
};

#endif