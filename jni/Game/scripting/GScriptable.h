#ifndef GSCRIPTABLE_CLASS
#define GSCRIPTABLE_CLASS


#include "oolua.h"

class GScriptManager;

class GScriptable
{
public:
	enum SCRIPTSTATE
	{
		NONE,
		RUNNING,
		PAUSED,
		FINALIZED
	};

	friend class GScriptManager;
	
protected:
	OOLUA::Script *mScript;	
	std::string mScriptPath; 
	int mScriptState;
	

	
public:
	
	//called when the script is set	
	virtual void OnSetScript(OOLUA::Script *script) =0;


	//Called when the script is about to start executing
	virtual void OnStartScript()=0;

	
	
	//called when the script has finished executed
	virtual void OnScriptFinished()=0;
	
		
};
#endif