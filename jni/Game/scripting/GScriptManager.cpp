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
#include <Irrlicht.h>
#include <IFileSystem.h>
#include "os.h"
#include "scripting/GScriptable.h"
#include "GScriptable.h"
#include "GScriptManager.h"


void GScriptManager::AttachScript(const char* scriptPath, GScriptable* scriptable)
{
	OOLUA::Script *script = new  OOLUA::Script();
	scriptable->mScriptState = (int)GScriptable::NONE;
	scriptable->mScriptPath = scriptPath;
	scriptable->mScript = script;
	scriptable->OnSetScript(script);

	
}


int  GScriptManager_readtoken(irr::io::IReadFile* f, irr::core::stringc* str )
{
  char  ch;
  *str = "";
  int l = 0;


  while ( 0 != f->read( &ch, 1 ) )
  {
	l++;
    if ( ( ch == '\n') )
	{
	  break;
	}
    else
	{
		*str += ch;
	}
  }

  return l;
}

bool GScriptManager::RunScriptable(GScriptable* scriptable)
{
	
	bool result =  false;

	irr::core::stringc fileName(scriptable->mScriptPath.c_str());

	irr::io::IReadFile* textfile = mDevice->getFileSystem()->createAndOpenFile(fileName);

	if (textfile )
	{
		irr::core::stringc s;
		std::string buffer;
		buffer.clear();

		while(GScriptManager_readtoken( textfile, &s )>0 )
		{
			buffer.append(s.c_str());			
		//	irr::os::Printer::log(s.c_str());
		}

		textfile->drop();

		result =   scriptable->mScript->run_chunk(buffer);
	}

	if(result)
	{		
		scriptable->mScriptState = (int)GScriptable::RUNNING;
		scriptable->OnStartScript();
	}

	return result;
}

void GScriptManager::DetachScript(GScriptable* scriptable)
{
}
void GScriptManager::Initialize(irr::IrrlichtDevice* device)
{
	mDevice = device;

}
void GScriptManager::Finalize()
{
}