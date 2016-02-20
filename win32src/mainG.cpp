
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
#include <windows.h>
#include "GameDefines.h"
#include "GGameAppWin.h"
#include "TonatiuhDefines.h"


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
/*
bool useWhite;

irr::f32 gNodeElapsedTime;

irr::f32 GetGlobalNodeElapsedTime(void)
{
	return gNodeElapsedTime;
}

void SetGlobalNodeElapsedTime(irr::f32 value)
{
	gNodeElapsedTime= value;
}
*/
/*
This is the main method. We can now use main() on every platform.
*/

#ifdef __cplusplus
   extern "C"
 #endif
int main()
{
	GGameApp::Initialize(new GGameAppWin());


	while(GGameApp::getInstance()->Render())
	{
		;
	}

	GGameApp::getInstance()->Finish();
	


	return 0;
}


/*
That's it. Compile and run.
**/
