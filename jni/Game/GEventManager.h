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
#ifndef GEVENTMANAGER_CLASS
#define GEVENTMANAGER_CLASS
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <irrlicht.h>


class GEventHandler 
{
public:
	virtual void OnKeyDown(irr::EKEY_CODE keycode) = 0;
	virtual void OnKeyUp(irr::EKEY_CODE keycode) = 0;
	virtual void OnMouseDown(int x, int y) = 0;
	virtual void OnMouseUp(int x, int y) = 0;
	virtual void OnMouseMove(int x, int y) = 0;
	virtual ~GEventHandler()    {};
};

class GEventManager : public irr::IEventReceiver
{

private:
    // We use this array to store the current state of each key
    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
	
	struct SMouseState
    {
        irr::core::position2di Position;
        bool LeftButtonDown;
        SMouseState() : LeftButtonDown(false) { }
    } MouseState;

	 typedef std::vector<GEventHandler*> EventHandlerList;
	 typedef EventHandlerList::iterator  EventHandlerListIterator;

	 EventHandlerList   mlisteners;
	 
	 typedef std::vector<GRocketEventHandler*> RocketHandlerList;
	 typedef RocketHandlerList::iterator		RocketHandlerListIterator;

	 RocketHandlerList mrocketlisters;
	 //input events
	 
	 void SendKeyDown(irr::EKEY_CODE keycode);
	 void SendKeyUp(irr::EKEY_CODE keycode);
	 void SendMouseDown(int x, int y);
	 void SendMouseUp(int x, int y);
	 void SendMouseMove(int x, int y);


	
	

public:
	virtual void OnKeyInputEvent(const irr::SEvent& event);
	 virtual void OnMouseInputEvent(const irr::SEvent& event);
	GEventManager();

    // This is the one method that we have to implement
    virtual bool OnEvent(const irr::SEvent& event);

    
    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }
    
	void AttachListener(GEventHandler& eventHandler);
    void DetachListener(GEventHandler& eventHandler);
     //rocket events
	void AttachRocketListener(GRocketEventHandler& eventHandler);
	void DetachRocketListener(GRocketEventHandler& eventHandler);

	/// Processes an event coming through from Rocket.
	/// @param[in] event The Rocket event that spawned the application event.
	/// @param[in] value The application-specific event value.
	void ProcessRocketEvent(Rocket::Core::Event& event, const Rocket::Core::String& value);
	

};

#endif