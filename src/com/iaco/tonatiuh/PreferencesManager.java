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
package com.iaco.tonatiuh;

import android.content.SharedPreferences;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Vibrator;
import android.util.Log;

public class PreferencesManager {

	private static PreferencesManager instance = null;

	public static final int PREFERENCE_TIMESPLAYED = 0;	
	public static final int PREFERENCE_SOUNDTOGGLE = 1;
	public static final int PREFERENCE_MUSICTOGGLE = 2;
	public static final int PREFERENCE_VIBRATE = 3;
	public static final int PREFERENCE_RATED = 4;
	
	
	public static final String PREFS_NAME = "TonatiuhPreferences";
	private Context context;
	private boolean hasVibrator=false;
	
	public PreferencesManager(Context contextParam) {
		context = contextParam;
		
		
		//check vibrate permission
		String permission = "android.permission.VIBRATE";
	    int res = context.checkCallingOrSelfPermission(permission);
	    boolean vibratePermission = (res == PackageManager.PERMISSION_GRANTED);            
		
	    Log.i("PreferencesManager", "vibrate Permision:" + vibratePermission);
		
	    if(vibratePermission)
	    {	
	    	Vibrator v = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
	    	
	    	if(v!=null)
	    		hasVibrator = v.hasVibrator();
	    	
	    	Log.i("PreferencesManager", "has vibrator:" + hasVibrator);
	    }
		
		
	}

	public void Initialize() {

	}
	
	void Vibrate()
	{
		
		if(hasVibrator)
		{
			Vibrator v = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
			v.vibrate(20);
			
		}
	}

	static public PreferencesManager getInstance(Context contextParam) {
		if (instance == null) {
			instance = new PreferencesManager(contextParam);
			instance.Initialize();
		}

		return instance;
	}

	public void SetPreference(int preferenceID, int value) {
		if (preferenceID >= PREFERENCE_TIMESPLAYED
				&& preferenceID <= PREFERENCE_RATED) {
			SharedPreferences settings = context.getSharedPreferences(
					PREFS_NAME, 0);
			SharedPreferences.Editor editor = settings.edit();
			String preferenceKey = "KEY" + Integer.toString(preferenceID);
			editor.putInt(preferenceKey, value);
			editor.commit();
		}

	}

	public int GetPreference(int preferenceID) {
		int value = 0;
		if (preferenceID >= PREFERENCE_TIMESPLAYED
				&& preferenceID <= PREFERENCE_RATED) {
			SharedPreferences settings = context.getSharedPreferences(
					PREFS_NAME, 0);
			String preferenceKey = "KEY" + Integer.toString(preferenceID);
			value = settings.getInt(preferenceKey, 1);
		}

		return value;
	}

}
