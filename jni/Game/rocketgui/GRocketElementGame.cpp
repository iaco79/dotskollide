/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "GameDefines.h"

#include "rocketgui/GRocketElementGame.h"
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/Input.h>

GRocketElementGame::GRocketElementGame(const Rocket::Core::String& tag) : Rocket::Core::Element(tag)
{
//	game = new Game();
}

GRocketElementGame::~GRocketElementGame()
{		
	//delete game;
}

/*
const String MOUSEDOWN = "mousedown";
const String MOUSEMOVE = "mousemove";
const String MOUSEUP =   "mouseup";

const String CLICK = "click";

const String MOUSESCROLL = "mousescroll";
const String MOUSEOVER = "mouseover";
const String MOUSEOUT = "mouseout";
const String FOCUS = "focus";
const String BLUR = "blur";
const String KEYDOWN = "keydown";

const String DRAGSTART = "dragstart";
const String DRAGOVER = "dragover";
const String LOAD = "load";
const String UNLOAD = "unload";
const String KEYUP = "keyup";
const String TEXTINPUT = "textinput";

const String DRAGMOVE = "dragmove";
const String DBLCLICK = "dblclick";
const String DRAGDROP = "dragdrop";
const String DRAGOUT = "dragout";
const String DRAGEND = "dragend";
const String RESIZE = "resize";
*/
// Intercepts and handles key events.

void GRocketElementGame::ProcessEventStatic(Rocket::Core::Event& rocketEvent)
{

	if (rocketEvent == "keydown") 
	{
	
	//	Rocket::Core::Input::KeyIdentifier key_identifier = (Rocket::Core::Input::KeyIdentifier) event.GetParameter<int>("key_identifier", 0);
		

	}
	else if (rocketEvent == "keyup")
	{


		
	}
	else if (rocketEvent == "mousemove")
	{
		int mouseX=0;
		int mouseY=0;		
		mouseX = rocketEvent.GetParameter<int>("mouse_x",0);
		mouseY = rocketEvent.GetParameter<int>("mouse_y",0);
		
		irr::SEvent irrEvent;
		irrEvent.EventType= irr::EET_MOUSE_INPUT_EVENT;
		irrEvent.MouseInput.X = mouseX;
		irrEvent.MouseInput.Y = mouseY;
		irrEvent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;

		

		if(GGameApp::getInstance() && GGameApp::getInstance()->getEventManager())
			GGameApp::getInstance()->getEventManager()->OnEvent(irrEvent);



	}
	else if (rocketEvent == "mousedown")
	{
		int mouseX=0;
		int mouseY=0;
		int buttonIndex=0;

		

		mouseX = rocketEvent.GetParameter<int>("mouse_x",0);
		mouseY = rocketEvent.GetParameter<int>("mouse_y",0);
		buttonIndex = rocketEvent.GetParameter<int>("button",0);

		irr::SEvent irrEvent;
		irrEvent.EventType= irr::EET_MOUSE_INPUT_EVENT;
		irrEvent.MouseInput.X = mouseX;
		irrEvent.MouseInput.Y = mouseY;
		irrEvent.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;

		if(GGameApp::getInstance() && GGameApp::getInstance()->getEventManager())
			GGameApp::getInstance()->getEventManager()->OnEvent(irrEvent);


	}	
	else if (rocketEvent == "mouseup")
	{

		int mouseX=0;
		int mouseY=0;
		int buttonIndex=0;
		mouseX = rocketEvent.GetParameter<int>("mouse_x",0);
		mouseY = rocketEvent.GetParameter<int>("mouse_y",0);
		buttonIndex = rocketEvent.GetParameter<int>("button",0);


		irr::SEvent irrEvent;
		irrEvent.EventType= irr::EET_MOUSE_INPUT_EVENT;
		irrEvent.MouseInput.X = mouseX;
		irrEvent.MouseInput.Y = mouseY;
		irrEvent.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;

		if(GGameApp::getInstance() && GGameApp::getInstance()->getEventManager())
			GGameApp::getInstance()->getEventManager()->OnEvent(irrEvent);

		
	}

}

void GRocketElementGame::ProcessEvent(Rocket::Core::Event& rocketEvent)
{
//	Rocket::Core::Element::ProcessEvent(event);
	ProcessEventStatic(rocketEvent);

	
}

/*
// Updates the game.
void ElementGame::OnUpdate()
{
	game->Update();
}

// Renders the game.
void ElementGame::OnRender()
{
	game->Render();
}

void ElementGame::OnChildAdd(Rocket::Core::Element* element)
{
	Rocket::Core::Element::OnChildAdd(element);

	if (element == this)
		GetOwnerDocument()->AddEventListener("load", this);
}
*/