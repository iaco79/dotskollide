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

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Vibrator;
import android.util.Log;
import android.media.*;
import android.view.KeyEvent;
import android.view.VelocityTracker;
import android.view.View;
import android.util.SparseArray;
import android.view.InputDevice;
import android.widget.LinearLayout;

import java.io.IOException;

/** */


public class GameActivity extends Activity {

    public IrrlichtGLView mGLView;
  
	static public Boolean pauseInput = new Boolean(false);
	static PreferencesManager mPreferencesMgr=null;
	


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        
        mPreferencesMgr = PreferencesManager.getInstance(this);
      
        nativeEnvJ2C(Environment.getExternalStorageDirectory().getAbsolutePath());
        int currentapiVersion = android.os.Build.VERSION.SDK_INT;
        
      
        
        
    	setContentView(R.layout.main);
        mGLView = (IrrlichtGLView)findViewById(R.id.RTSurfaceView);
        
        
        
        if (currentapiVersion > 14){
            View decorView = this.getWindow().getDecorView();            	
            int uiOptions = View.SYSTEM_UI_FLAG_LOW_PROFILE;
            decorView.setSystemUiVisibility(uiOptions);
        }
         
        
        //setContentView(mGLView);
        
        
        
        AssetManager assetMgr = this.getAssets();        
        nativeOnCreate(assetMgr);
        
        
		
		
    }

    @Override
    public void onBackPressed()
    {
			  
			Log.i("IrrlichtGLView", "onkeyUp");
			IrrlichtEvent iev = new IrrlichtEvent();;
			iev.mAction = 7777000; //back button
			nativeSendEvent(iev);
			  
    	
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        
        Log.i("Tonatiuh", "Activity onPause");
     
        
        
        mGLView.onPause();
        nativeOnPause();
    }

    @Override
    protected void onStart() {
    	
    	super.onStart();
    	
    	  int currentapiVersion = android.os.Build.VERSION.SDK_INT;
          
          
          if (currentapiVersion > 14){
              View decorView = this.getWindow().getDecorView();            	
              int uiOptions = View.SYSTEM_UI_FLAG_LOW_PROFILE;
              decorView.setSystemUiVisibility(uiOptions);
          }
                    
    	
    }
    @Override
    protected void onResume() {
        super.onResume();
		
      
        
        Log.i("Tonatiuh", "Activity onResume");
        
        
        mGLView.onResume();
        nativeOnResume();
    }

    @Override    
    public void onDestroy() {
    	
    	
        
        nativeOnDestroy();
        super.onDestroy();
    }

    /** load irrlicht.so */
    static {
        System.loadLibrary("gamedemo");
    }

	
	// Audio
    public static void audioInit(int sampleRate, boolean is16Bit, boolean isStereo, int desiredFrames) {
        int channelConfig = isStereo ? AudioFormat.CHANNEL_CONFIGURATION_STEREO : AudioFormat.CHANNEL_CONFIGURATION_MONO;
        int audioFormat = is16Bit ? AudioFormat.ENCODING_PCM_16BIT : AudioFormat.ENCODING_PCM_8BIT;
        int frameSize = (isStereo ? 2 : 1) * (is16Bit ? 2 : 1);
        
        Log.v("SDL", "SDL audio: wanted " + (isStereo ? "stereo" : "mono") + " " + (is16Bit ? "16-bit" : "8-bit") + " " + ((float)sampleRate / 1000f) + "kHz, " + desiredFrames + " frames buffer");
        
        // Let the user pick a larger buffer if they really want -- but ye
        // gods they probably shouldn't, the minimums are horrifyingly high
        // latency already
        desiredFrames = Math.max(desiredFrames, (AudioTrack.getMinBufferSize(sampleRate, channelConfig, audioFormat) + frameSize - 1) / frameSize);
        
        mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate,
                channelConfig, audioFormat, desiredFrames * frameSize, AudioTrack.MODE_STREAM);
        
        
        audioStartThread();
        
        Log.v("SDL", "SDL audio: got " + ((mAudioTrack.getChannelCount() >= 2) ? "stereo" : "mono") + " " + ((mAudioTrack.getAudioFormat() == AudioFormat.ENCODING_PCM_16BIT) ? "16-bit" : "8-bit") + " " + ((float)mAudioTrack.getSampleRate() / 1000f) + "kHz, " + desiredFrames + " frames buffer");
    }
    
    public static void audioStartThread() {
        mAudioThread = new Thread(new Runnable() {
            public void run() {
                mAudioTrack.play();
                nativeRunAudioThread();
            }
        });
        
        // I'd take REALTIME if I could get it!
        mAudioThread.setPriority(Thread.MAX_PRIORITY);
        mAudioThread.start();
    }
    
    public static void audioWriteShortBuffer(short[] buffer) {
        for (int i = 0; i < buffer.length; ) {
            int result = mAudioTrack.write(buffer, i, buffer.length - i);
            if (result > 0) {
                i += result;
            } else if (result == 0) {
                try {
                    Thread.sleep(1);
                } catch(InterruptedException e) {
                    // Nom nom
                }
            } else {
                Log.w("SDL", "SDL audio: error return from write(short)");
                return;
            }
        }
    }
    
    public static void audioWriteByteBuffer(byte[] buffer) {
        for (int i = 0; i < buffer.length; ) {
            int result = mAudioTrack.write(buffer, i, buffer.length - i);
            if (result > 0) {
                i += result;
            } else if (result == 0) {
                try {
                    Thread.sleep(1);
                } catch(InterruptedException e) {
                    // Nom nom
                }
            } else {
                Log.w("SDL", "SDL audio: error return from write(short)");
                return;
            }
        }
    }

    public static void audioQuit() {
        if (mAudioThread != null) {
            try {
                mAudioThread.join();
            } catch(Exception e) {
                Log.v("SDL", "Problem stopping audio thread: " + e);
            }
            mAudioThread = null;

            //Log.v("SDL", "Finished waiting for audio thread");
        }

        if (mAudioTrack != null) {
            mAudioTrack.stop();
            mAudioTrack = null;
        }
    }
	

    public static void setPreferenceValue(int prefId, int value)
    {    	
    	mPreferencesMgr.SetPreference(prefId, value);
    }
	
    public static int getPreferenceValue(int prefId)
    {    	
    	return mPreferencesMgr.GetPreference(prefId);
    	
    }
	
    public static void vibrate()
    {
    	mPreferencesMgr.Vibrate();
    	   	
    }
    
	
	// Audio
    private static Thread mAudioThread;
    private static AudioTrack mAudioTrack;

	
    public native void nativeOnCreate(AssetManager assetManager);
    public native void nativeOnPause();
    public native void nativeOnResume();
    public native void nativeOnDestroy();
    public native void nativeInitGL();
    public native void nativeResize(int w, int h);
    public native void nativeGetStatus(IrrlichtStatus status);
    public native void nativeSendEvent(IrrlichtEvent event);
    public native void nativeEnvJ2C(String sdcardPath);
    
    
    
    public static native void nativeDrawIteration();
	public static native void nativeInitSDL();
	public static native void nativeRunAudioThread();
	
    
}

