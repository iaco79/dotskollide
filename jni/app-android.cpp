// Copyright (C) 2002-2009 Laurent Mallet & Maxime Biais
// This file is part of the "Irrlicht Android Port".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#include <jni.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>
#include <importgl.h>
#include <irrlicht.h>
#include "COGLES2Driver.h"
// needed here also because of the create methods' parameters
#include "CNullDriver.h"
#include <CIrrDeviceStub.h>
#include <os.h>
#include <android-logger.h>
#include "GameDefines.h"
#include "GGameAppAndroid.h"



bool useWhite;

f32 gNodeElapsedTime;

f32 GetGlobalNodeElapsedTime(void)
{
	return gNodeElapsedTime;
}

void SetGlobalNodeElapsedTime(f32 value)
{
	gNodeElapsedTime= value;
}


static long _getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}





bool nativeInitialize(int width,int height)
{	
	
	GGameApp::Initialize(new GGameAppAndroid(width, height));

	return true;
}


void nativeDrawIteration()
{
    GGameApp::getInstance()->Render();

}

