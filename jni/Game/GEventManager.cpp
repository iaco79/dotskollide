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
#include "menu/GRocketEvent.h"
#include "menu/GRocketEventHandler.h"
#include "GEventManager.h"

GEventManager::GEventManager()
{

}

void GEventManager::OnKeyInputEvent(const irr::SEvent& event)
{

	KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	if(event.KeyInput.PressedDown)
	{
		SendKeyDown(event.KeyInput.Key);
			
	}
	else
	{
		SendKeyUp(event.KeyInput.Key);

		//test.KeyboardUp(event.KeyInput.Key);
	}
}

void GEventManager::OnMouseInputEvent(const irr::SEvent& event)
{
		switch(event.MouseInput.Event)
        {
		case irr::EMIE_LMOUSE_PRESSED_DOWN:
			{
				GGameApp::getInstance()->getRocketGUI()->SetSkipGUIEvents(true);
					
				MouseState.Position.X = event.MouseInput.X;
				MouseState.Position.Y = event.MouseInput.Y;
				//CalculateWorldXY(true,true);


				//if(!MouseState.LeftButtonDown)
				SendMouseDown(MouseState.Position.X, MouseState.Position.Y);
				MouseState.LeftButtonDown = true;
				

				break;
			}
        case irr::EMIE_LMOUSE_LEFT_UP:
			{
				MouseState.LeftButtonDown = false;
				//test.MouseUp(mBoxWorldXY);
				SendMouseUp(MouseState.Position.X, MouseState.Position.Y);
				
				GGameApp::getInstance()->getRocketGUI()->SetSkipGUIEvents(false);



				break;
			}
        case irr::EMIE_MOUSE_MOVED:
			{
	                
				//transform xy to world xy
				MouseState.Position.X = event.MouseInput.X;
				MouseState.Position.Y = event.MouseInput.Y;
				//CalculateWorldXY(false, MouseState.LeftButtonDown);
				SendMouseMove(MouseState.Position.X, MouseState.Position.Y);
				
				//test.MouseMove(mBoxWorldXY);
				break;
		}
        default:
			break;
		}
}

 bool GEventManager::OnEvent(const irr::SEvent& event)
{
    // Remember whether each key is down or up
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		OnKeyInputEvent(event);

	}
	else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
		OnMouseInputEvent(event);
    }

    return false;
}


void GEventManager::SendKeyDown(irr::EKEY_CODE keycode)
{
	 for (EventHandlerListIterator iter = mlisteners.begin();
            iter != mlisteners.end();
            ++iter)
       {
              GEventHandler* pEventHandler = *iter;
             
			  if (pEventHandler)
              {
                     pEventHandler->OnKeyDown(keycode);
              }
       }

}
void GEventManager::SendKeyUp(irr::EKEY_CODE keycode)
{
	 for (EventHandlerListIterator iter = mlisteners.begin();
            iter != mlisteners.end();
            ++iter)
       {
              GEventHandler* pEventHandler = *iter;
             
			  if (pEventHandler)
              {
                     pEventHandler->OnKeyUp(keycode);
              }
       }
}
void GEventManager::SendMouseDown(int x, int y)
{

	for (EventHandlerListIterator iter = mlisteners.begin();
            iter != mlisteners.end();
            ++iter)
       {
              GEventHandler* pEventHandler = *iter;
             
			  if (pEventHandler)
              {
				  pEventHandler->OnMouseDown(x,y);
              }

       }
}
void GEventManager::SendMouseUp(int x, int y)
{
	for (EventHandlerListIterator iter = mlisteners.begin();
            iter != mlisteners.end();
            ++iter)
       {
              GEventHandler* pEventHandler = *iter;
             
			  if (pEventHandler)
              {
				  pEventHandler->OnMouseUp(x,y);
              }

       }
}
void GEventManager::SendMouseMove(int x, int y)
{
	for (EventHandlerListIterator iter = mlisteners.begin();
            iter != mlisteners.end();
            ++iter)
       {
              GEventHandler* pEventHandler = *iter;
             
			  if (pEventHandler)
              {
				  pEventHandler->OnMouseMove(x,y);
              }

       }

}

void GEventManager::AttachListener(GEventHandler& eventHandler)
{
	mlisteners.push_back(&eventHandler);
}
void GEventManager::DetachListener(GEventHandler& eventHandler)
{
	
       for (EventHandlerListIterator iter = mlisteners.begin();
            iter != mlisteners.end();
            ++iter)
       {
              if (&eventHandler == *iter)
              {
                     mlisteners.erase(iter);
                     break;
              }
       }

}

   



// Processes an event coming through from Rocket.
void GEventManager::ProcessRocketEvent(Rocket::Core::Event& event, const Rocket::Core::String& value)
{
	
	Rocket::Core::StringList commands;
	Rocket::Core::StringUtilities::ExpandString(commands, value, ';');

	for (RocketHandlerListIterator iter = mrocketlisters.begin();
            iter != mrocketlisters.end();
            ++iter)
       {
		   (*iter)->ProcessRocketEvent(value);
       }
}


void GEventManager::AttachRocketListener(GRocketEventHandler& eventHandler)
{
	mrocketlisters.push_back(&eventHandler);
}
void GEventManager::DetachRocketListener(GRocketEventHandler& eventHandler)
{
	
       for (RocketHandlerListIterator iter = mrocketlisters.begin();
            iter != mrocketlisters.end();
            ++iter)
       {
              if (&eventHandler == *iter)
              {
                     mrocketlisters.erase(iter);
                     break;
              }
       }

}