/*
  Copyright (C) 2011 Daniel Sudmann

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

  Daniel Sudmann suddani@googlemail.com
*/

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <irrlicht.h>

#include "rocketgui/IrrRocketInterface.h"
#include "rocketgui/IrrRocketFileInterface.h"
#include "rocketgui/IrrRocketRenderer.h"
#include "rocketgui/CIrrRocketGUI.h"
#include "rocketgui/CIrrRocketEventReceiver.h"
#include "rocketgui/GRocketElementGame.h"
#include "menu/GRocketEvent.h"
#include "menu/GRocketEventInstancer.h"

unsigned int CIrrRocketGUI::RocketContextCount = 0;

void CIrrRocketGUI::Initialize()
{
	  //ctor
    irr::video::IVideoDriver* driver = Device->getVideoDriver();

	mRocketFS = new IrrRocketFileInterface(Device->getFileSystem());
	mRocketRenderer = new IrrRocketRenderer(Device->getVideoDriver());
	mRocketInterface = new IrrRocketInterface(Device->getTimer());

	Reset();
	
	//librocket will take care of handling irrlicht input events
	Device->setEventReceiver(InputEvents);
    GUI_Camera = Device->getSceneManager()->addCameraSceneNode(0, irr::core::vector3df(driver->getScreenSize().Width/2.f, driver->getScreenSize().Height/2.f, 5), irr::core::vector3df(driver->getScreenSize().Width/2.f, driver->getScreenSize().Height/2.f, 0), -1, false);
    GUI_Camera->setUpVector(irr::core::vector3df(0, -1, 0));
    irr::core::matrix4 mat;
	mat.buildProjectionMatrixOrthoLH(driver->getScreenSize().Width, driver->getScreenSize().Height, 0.1,10);
	GUI_Camera->setProjectionMatrix(mat);

}

void CIrrRocketGUI::Reset()
{
	Rocket::Core::Shutdown();

    Rocket::Core::SetFileInterface(mRocketFS);
    Rocket::Core::SetRenderInterface(mRocketRenderer);
    Rocket::Core::SetSystemInterface(mRocketInterface);

	Rocket::Core::Initialise();

#if defined(_IRR_ANDROID_PLATEFORM_)

	Rocket::Core::FontDatabase::LoadFontFace("AK_SANS.ttf");
	Rocket::Core::FontDatabase::LoadFontFace("ak_sans_bo.ttf");
	Rocket::Core::FontDatabase::LoadFontFace("Delicious-Roman.otf");

#else	
	Rocket::Core::FontDatabase::LoadFontFace("IrrlichtSdcard/AK_SANS.ttf");
	Rocket::Core::FontDatabase::LoadFontFace("IrrlichtSdcard/ak_sans_bo.ttf");
	Rocket::Core::FontDatabase::LoadFontFace("IrrlichtSdcard/Delicious-Roman.otf");
	
#endif
	RocketContext = Rocket::Core::CreateContext("default", Rocket::Core::Vector2i(Device->getVideoDriver()->getScreenSize().Width, Device->getVideoDriver()->getScreenSize().Height));
	InputEvents = new CIrrRocketEventReceiver(RocketContext);
	Device->setEventReceiver(InputEvents);

	//register the GameInputElement as a librocket GUI element
	Rocket::Core::ElementInstancer* element_instancer = new Rocket::Core::ElementInstancerGeneric< GRocketElementGame >();
	Rocket::Core::Factory::RegisterElementInstancer("gameinputelement", element_instancer);
	element_instancer->RemoveReference();

	//load de game input element
	//this will dispatch the events to the game states	
	#ifndef _IRR_ANDROID_PLATEFORM_
		LoadDocument("IrrlichtSdcard/GameInputElement.rml");
	#else
		LoadDocument("GameInputElement.rml");
	#endif

	//register the menu event instance
	GRocketEventInstancer* event_instancer = new GRocketEventInstancer();
	Rocket::Core::Factory::RegisterEventListenerInstancer(event_instancer);
	event_instancer->RemoveReference();


}

CIrrRocketGUI::CIrrRocketGUI(irr::IrrlichtDevice* device) : Device(device)
{
	 mSkipGUIEvents = false;	
	 mRocketFS = NULL;
	mRocketRenderer = NULL;
	mRocketInterface = NULL;


	  
}

CIrrRocketGUI::~CIrrRocketGUI()
{
    //dtor
    RocketContextCount--;
    delete InputEvents;
    InputEvents = 0;
}

irr::IEventReceiver* CIrrRocketGUI::getGUIEventReceiver(void)
{
    return InputEvents;
}

void CIrrRocketGUI::run(void)
{
    RocketContext->Update();
    GUI_Camera->render();


	//update color animation
	mRocketRenderer->Update();


    RocketContext->Render();
}


bool CIrrRocketGUI::LoadDocument(const char* documentPath, bool show)
{
	Rocket::Core::ElementDocument* document = RocketContext->LoadDocument(documentPath);
	
	if(document)
	{
		if(show)
		{
			document->Focus();		
			document->Show();
		}
		document->RemoveReference();
		
		return true;
	}

	return false;
	
}
bool CIrrRocketGUI::CloseDocument(const char* documentId)
{
	Rocket::Core::ElementDocument* document = RocketContext->GetDocument(documentId);
	
	if (document)
	{
		document->Close();

		return true;
	}

	return false;

}
bool CIrrRocketGUI::SetDocumentVisible(const char* documentId, const bool visible)
{
	Rocket::Core::ElementDocument* document = RocketContext->GetDocument(documentId);
			
	if (document)
	{
		if(visible)
		{
			document->Focus();
			document->Show();
		}
		else
			document->Hide();

		return true;
	}
	return false;
}



bool CIrrRocketGUI::SetElementInnerText(const char* documentId,const char* elementName, const char* value)
{
	if(RocketContext->GetDocument(documentId))
	{
		Rocket::Core::Element* element = RocketContext->GetDocument(documentId)->GetElementById(elementName);
		if (element != NULL)
		{
			element->SetInnerRML(Rocket::Core::String(256, "%s", value).CString());	
			return true;
		}
	}
	return false;
}
bool CIrrRocketGUI::SetElementInnerText(const char* documentId,const char* elementName, const int value)
{
	if(RocketContext->GetDocument(documentId))
	{
		Rocket::Core::Element* element = RocketContext->GetDocument(documentId)->GetElementById(elementName);
		if (element != NULL)
		{
			element->SetInnerRML(Rocket::Core::String(256, "%d", value).CString());
			return true;
		}
	}
	return false;
}
bool CIrrRocketGUI::SetElementInnerText(const char* documentId,const char* elementName, const float value)
{
	if(RocketContext->GetDocument(documentId))
	{
		Rocket::Core::Element* element = RocketContext->GetDocument(documentId)->GetElementById(elementName);
		if (element != NULL)
		{
			element->SetInnerRML(Rocket::Core::String(256, "%.2f", value).CString());
			return true;
		}
	}
	return false;
}



bool CIrrRocketGUI::SetElementVisibility(const char* documentId,const char* elementName, bool visible)
{
	if(RocketContext->GetDocument(documentId))
	{
		Rocket::Core::Element* element = RocketContext->GetDocument(documentId)->GetElementById(elementName);
			if (element != NULL)
			{
				
				if(!visible)
					element->SetProperty("visibility", "hidden");
				else
					element->SetProperty("visibility", "visible");
				
				return true;
			}
	}
	return false;
}
