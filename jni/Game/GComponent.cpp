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


unsigned long hash_string(unsigned char *str) 
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

GComponentId GComponent::GetIdFromName(std::string name)
{
	return hash_string ((unsigned char*)name.c_str());

}

GComponent& GComponent::operator= (const GComponent& other )
{

		mGameObject = NULL; //this' owner is not other's owner
		gcomponentId = other.gcomponentId; //id should be the same
		return *this;
}


