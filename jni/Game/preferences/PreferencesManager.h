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
#ifndef GPREFERENCESMANAGER_CLASS
#define GPREFERENCESMANAGER_CLASS

class IPreferences
{
public:
	virtual void setPreference(int preferenceID, int value) = 0;
	virtual int  getPreference(int preferenceID) =0;
	virtual void vibrate() = 0;

};


class PreferencesManager
{

private:
	IPreferences* mPreferences;
	static PreferencesManager* gInstance;

public:

	PreferencesManager();

	static PreferencesManager* getInstance();
	void initialize(IPreferences* preferences);
	
	void setPreference(int preferenceID, int value);
	int  getPreference(int preferenceID, int defValue);
	void vibrate();


};


#endif