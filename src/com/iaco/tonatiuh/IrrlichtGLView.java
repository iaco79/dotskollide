package com.iaco.tonatiuh;

import android.content.Context;
import android.graphics.Point;
import android.opengl.GLSurfaceView;

import android.os.Build;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.WindowManager;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.view.VelocityTracker;

/** IrrlichtGLView */
//class IrrlichtGLView extends com.iaco.tonatiuh.GLSurfaceView {
class IrrlichtGLView extends android.opengl.GLSurfaceView {
	private GameActivity mActivity;
	
	static public Boolean isMoving = new Boolean(false);
	public float mScreenTouchFactorX=1.0f;
	public float mScreenTouchFactorY=1.0f;
	
	
	
	public VelocityTracker mVelocityTracker;
	//public boolean isMoving = false;
	public boolean mVelocityTrackerRecycled = false;
	public IrrlichtEvent lastEvent;

	private Point mRes;
	
	private static final int INVALID_POINTER_ID = -1;
	private int mActivePointerId = INVALID_POINTER_ID;

	Point  GetScreenXY(GameActivity activity)
	{
	
		WindowManager w = activity.getWindowManager();
		Display d = w.getDefaultDisplay();
		DisplayMetrics metrics = new DisplayMetrics();
		d.getMetrics(metrics);
		// since SDK_INT = 1;
		int widthPixels = metrics.widthPixels;
		int heightPixels = metrics.heightPixels;
		// includes window decorations (statusbar bar/menu bar)
		if (Build.VERSION.SDK_INT >= 14 && Build.VERSION.SDK_INT < 17)
		try {
		    widthPixels = (Integer) Display.class.getMethod("getRawWidth").invoke(d);
		    heightPixels = (Integer) Display.class.getMethod("getRawHeight").invoke(d);
		} catch (Exception ignored) {
		}
		// includes window decorations (statusbar bar/menu bar)
		if (Build.VERSION.SDK_INT >= 17)
		try {
		    Point realSize = new Point();
		    Display.class.getMethod("getRealSize", Point.class).invoke(d, realSize);
		    widthPixels = realSize.x;
		    heightPixels = realSize.y;
		} catch (Exception ignored) {
		}
		
		Point result = new Point(widthPixels, heightPixels);
		return result;
		
	}
	
	
	//static intializer
	public IrrlichtGLView(Context context, AttributeSet attrs) {
	        super(context, attrs);
	        Log.i("IrrlichtGLView", "RTSurface constructor - Layout Form");
	        	        
	        mActivity = (GameActivity)context;
			
			setEGLContextClientVersion(2);
			mRenderer = new IrrlichtRenderer(mActivity);
			setRenderer(mRenderer);
			

			mVelocityTracker = VelocityTracker.obtain();

			//get screen size 
			
			mRes = GetScreenXY(mActivity);			 
			Log.i("IrrlichtGLView", "Client size: w:" + mRes.x +" h:" + mRes.y );			
		    final int maxX=720;
		    final int maxY=1280;
		    
			if((mRes.x > maxX) && (mRes.y > maxY))		
			{
			
				Display display = ((WindowManager) mActivity.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
				float ww = display.getWidth();
				float hh = display.getHeight();			
				Log.i("IrrlichtGLView", "Client ww: " + ww +" hh:" + hh );
				
			    mRes.set(maxX,maxY);
				getHolder().setFixedSize(mRes.x, mRes.y);				
				mScreenTouchFactorX= mRes.x/ww;
				mScreenTouchFactorY= mRes.y/hh;
				
				
				Log.i("IrrlichtGLView", "Client fixing size to : w: " + mRes.x +" h:" + mRes.y );
				
				Log.i("IrrlichtGLView", "Client fixing size to : fx: " +mScreenTouchFactorX +" fy:" + mScreenTouchFactorY );
				
				
			}
			
					
			
			lastEvent = new IrrlichtEvent();		
			lastEvent.mX = mRes.x/2.0f;
			lastEvent.mY = mRes.y/2.0f;
			lastEvent.mSpeedX = 0.0f;
			lastEvent.mSpeedY = 0.0f;
			
	     } 
	
	public IrrlichtGLView(GameActivity activity) {
		super(activity);
		mActivity = activity;
				
		Log.i("IrrlichtGLView", "RTSurface constructor - normal Form");
		
		setEGLContextClientVersion(2);
		mRenderer = new IrrlichtRenderer(activity);
		setRenderer(mRenderer);
		

		mVelocityTracker = VelocityTracker.obtain();

		//get screen size 
		mRes = GetScreenXY(mActivity);			 
		Log.i("IrrlichtGLView", "Client size: w:" + mRes.x +" h:" + mRes.y );			
	    final int maxX=720;
	    final int maxY=1280;
	    
		if((mRes.x > maxX) && (mRes.y > maxY))		
		{
		
			Display display = ((WindowManager) mActivity.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
			float ww = display.getWidth();
			float hh = display.getHeight();			
			Log.i("IrrlichtGLView", "Client ww: " + ww +" hh:" + hh );
			
		    mRes.set(maxX,maxY);
			getHolder().setFixedSize(mRes.x, mRes.y);				
			mScreenTouchFactorX= mRes.x/ww;
			mScreenTouchFactorY= mRes.y/hh;
			
			
			Log.i("IrrlichtGLView", "Client fixing size to : w: " + mRes.x +" h:" + mRes.y );
			
			Log.i("IrrlichtGLView", "Client fixing size to : fx: " +mScreenTouchFactorX +" fy:" + mScreenTouchFactorY );
			
			
		}
		
		
		lastEvent = new IrrlichtEvent();		
		lastEvent.mX = mRes.x/2.0f;
		lastEvent.mY = mRes.y/2.0f;
		lastEvent.mSpeedX = 0.0f;
		lastEvent.mSpeedY = 0.0f;

		
	}
	
	
	@SuppressWarnings("deprecation")
	public boolean onTouchEvent(final MotionEvent ev) {
		/*
		 * lastEvent.mAction = event.getAction(); lastEvent.mX = event.getX();
		 * lastEvent.mY = event.getY(); if (mRenderer!=null) {
		 * mActivity.nativeSendEvent(lastEvent); }
		 */

		final int action = ev.getAction();
		switch (action & MotionEvent.ACTION_MASK) {
		case MotionEvent.ACTION_DOWN: {

			// Save the ID of this pointer
			mActivePointerId = ev.getPointerId(0);

		//	synchronized(isMoving)
			{
				if (mVelocityTracker == null) {
					// Retrieve a new VelocityTracker object to watch the velocity
					// of a motion.
					mVelocityTracker = VelocityTracker.obtain();
				} else {
					// Reset the velocity tracker back to its initial state.
					mVelocityTracker.clear();
				}
				mVelocityTrackerRecycled = false;			
				mVelocityTracker.addMovement(ev);
				isMoving = false;
			}
			
			
			
			
			lastEvent.mAction = MotionEvent.ACTION_DOWN;
			lastEvent.mX = ev.getX()*mScreenTouchFactorX;
			lastEvent.mY = ev.getY()*mScreenTouchFactorY;
			if (mRenderer != null) {

		
				mActivity.nativeSendEvent(lastEvent);

			}
			break;
		}
		case MotionEvent.ACTION_POINTER_DOWN: {
			final int pointerIndex = (action & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
			final int pointerId = ev.getPointerId(pointerIndex);
			
			
						
			final float x = ev.getX(pointerIndex)*mScreenTouchFactorX;
			final float y = ev.getY(pointerIndex)*mScreenTouchFactorY;

			if (mRenderer != null) {

				mActivePointerId = pointerId;
				lastEvent.mAction = MotionEvent.ACTION_DOWN;
				lastEvent.mX = x;
				lastEvent.mY = y;
				mActivity.nativeSendEvent(lastEvent);

			}
			break;

		}
		case MotionEvent.ACTION_MOVE: {
			
		
			
			if (mActivePointerId != INVALID_POINTER_ID) {
				
				// Find the index of the active pointer and fetch its position
				final int pointerIndex = ev.findPointerIndex(mActivePointerId);
				float x = ev.getX(pointerIndex)*mScreenTouchFactorX;
				float y = ev.getY(pointerIndex)*mScreenTouchFactorY;
				
				lastEvent.mAction = MotionEvent.ACTION_MOVE;
				lastEvent.mX = x;
				lastEvent.mY = y;

				{
				         
		          mVelocityTracker.addMovement(ev);
		          isMoving = true;

				}
				
				//update the speed 
				
				if (mRenderer!=null) 
				{
					
								
					lastEvent.mX = x;
					lastEvent.mY = y;
					mActivity.nativeSendEvent(lastEvent);
					
					
				 }
			}

			break;
		}

		case MotionEvent.ACTION_UP: {
			mActivePointerId = INVALID_POINTER_ID;
			lastEvent.mAction = MotionEvent.ACTION_UP;
			
			{
				if(!mVelocityTrackerRecycled)
				{
					mVelocityTrackerRecycled = true;
					
					try {
						mVelocityTracker.recycle();
						mVelocityTracker = null;
					}
					catch (NullPointerException e) {
					}
					
				}
				isMoving = false;
			}
			
			if (mRenderer != null) {
				mActivity.nativeSendEvent(lastEvent);
			}

			break;
		}

		case MotionEvent.ACTION_CANCEL: {
			mActivePointerId = INVALID_POINTER_ID;
			lastEvent.mAction = MotionEvent.ACTION_CANCEL;
			{
				if(!mVelocityTrackerRecycled)
				{
					mVelocityTrackerRecycled = true;
					
					try {
						mVelocityTracker.recycle();
						mVelocityTracker = null;
					}
					catch (NullPointerException e) {
					}
					
				}
				isMoving = false;
			}

			break;
		}

		case MotionEvent.ACTION_POINTER_UP: {
			// Extract the index of the pointer that left the touch sensor
			final int pointerIndex = (action & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
			final int pointerId = ev.getPointerId(pointerIndex);
			
		
			if (pointerId == mActivePointerId) {
				// This was our active pointer going up. Choose a new
				// active pointer and adjust accordingly.
				// final int newPointerIndex = pointerIndex == 0 ? 1 : 0;
				mActivePointerId = INVALID_POINTER_ID; // ev.getPointerId(newPointerIndex);

				if (mRenderer != null) {
					lastEvent.mAction = MotionEvent.ACTION_UP;
					mActivity.nativeSendEvent(lastEvent);
				}

			}
			break;
		}
		}

		return true;
	}

	public IrrlichtRenderer mRenderer;

}

class IrrlichtRenderer implements android.opengl.GLSurfaceView.Renderer {
	private GameActivity mActivity;
	public int mWindowWidth;
	public int mWindowHeight;
	
	
	private IrrlichtStatus mStatus;

	public IrrlichtRenderer(GameActivity activity) {
		mActivity = activity;
		mStatus = new IrrlichtStatus();
		mStatus.mQuit = false;
	}

	public void onSurfaceLost() {
		
	}
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {

		Log.i("IrrlichtGLView", "onSurfaceCreated");
		
		surfaceCreated=true;
		mActivity.nativeInitGL();
		//gl.glDisable(GL10.GL_DITHER);
		//gl.glEnable(GL10.GL_BLEND);
	
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		
		Log.i("IrrlichtGLView", "onSurfaceChanged");
		
		mWindowWidth = width;
		mWindowHeight = height;
		mActivity.nativeResize(width, height);
	}

	public void onDrawFrame(GL10 gl) {
		// render a frame
		if (firstTime) {
			Log.i("MESSAGE", "Calling Native Init SDL");

			GameActivity.nativeInitSDL();
			firstTime = false;
		}

	
		
		GameActivity.nativeDrawIteration();
		// get status
		mActivity.nativeGetStatus(mStatus);
		if (mStatus.mQuit) {
			Log.e("MESSAGE", "QUIT");
			mActivity.finish();
		}
	}

	boolean firstTime = true;
	boolean surfaceCreated = false;
	
}

class IrrlichtEvent {
	public int mAction;
	public float mX;
	public float mY;
	public float mSpeedX;
	public float mSpeedY;

}

class IrrlichtStatus {
	public boolean mQuit;
}
