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
#include "GComponent.h"
#include "GController.h"
#include "GControllerFactory.h"




GControllerFactory::GControllerFactory()
{

	//factoryMap.Register<GElementController>(GCT_ELEMENT_IDX,GComponent::GetIdFromName(GElementController::mName));

}





GController* GControllerFactory::CreateController(const Json::Value& json)
{
	std::string nodeName = json["sController"].asString();
	GControllerId id = GComponent::GetIdFromName(nodeName);
	GController* controller = factoryMap.Create(id);

	controller->gcontrollerId = id; 
	controller->VInit(json);

	return controller;
}
GController* GControllerFactory::CloneController(GController* other)
{
	GController* controller =  factoryMap.Copy(other->VGetId(),other);
	
	return controller;
}


GControllerFactory::~GControllerFactory()
{

}

