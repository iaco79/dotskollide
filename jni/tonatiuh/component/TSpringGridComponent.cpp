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
#include <irrlicht.h>
#include "TSpringGridComponent.h"

const char* TSpringGridComponent::mName = "SpringGrid";


TSpringGridComponent::TSpringGridComponent()
{
	gcomponentId = GComponent::GetIdFromName(TSpringGridComponent::mName);
	mSpringGrid = NULL;

}

void TSpringGridComponent::VUpdate(int deltaMs) 
{	
	//operate at 60fps
	mSpringGrid->operate(0.016f);

	//mtimer animation 


}


void TSpringGridComponent::VInit(springrid::SpringGrid* springGrid)
{
	mSpringGrid = springGrid;
}

bool TSpringGridComponent::VInit(Json::Value& jcomponent)
{

	return true;
}

TSpringGridComponent& TSpringGridComponent::operator= (const TSpringGridComponent& other )
{
	GComponent::operator= (other); //call base assignment operator

	return *this;
}
