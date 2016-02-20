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
#include "COGLES2Driver.h"
#include <Box2D.h>
#include "GameDefines.h"
#include "GGameApp.h"
#include "TonatiuhDefines.h"
#include "shaders/FireworkShader.h"

irr::f32 FireWorkShader::gElapsedTime=0.0f;
irr::video::SColorf FireWorkShader::gFireColor;
s32 FireWorkShader::gFireWorkMaterialType=0;	


FireWorkShader::FireWorkShader()
{ 
	
}


//in seconds
irr::f32 FireWorkShader::GetElapsedTime()
{
	return gElapsedTime;
} 

video::SColorf FireWorkShader::GetFireColor()
{
	return gFireColor;
} 


void FireWorkShader::SetUniforms(irr::f32 elapsedTime,irr::video::SColorf fireColor)
{
	gElapsedTime = elapsedTime;
	gFireColor = fireColor;
}

	

void FireWorkShader::Initialize()
{
	
	irr::IrrlichtDevice* device = GGameApp::getInstance()->getIrrlichtDevice();
	video::IGPUProgrammingServices* gpu = device->getVideoDriver()->getGPUProgrammingServices();
	
	FireWorkShader* mc = new FireWorkShader();
	mc->mDevice = device;

	gFireWorkMaterialType = gpu->addHighLevelShaderMaterialFromFiles(
#if defined(_IRR_ANDROID_PLATEFORM_)
  	"FireworksShader.vsh", "main", video::EVST_VS_1_1,
	"FireworksShader.fsh", "main", video::EPST_PS_1_1,
#else
	"IrrlichtSdcard/FireworksShader.vsh", "main", video::EVST_VS_1_1,
	"IrrlichtSdcard/FireworksShader.fsh", "main", video::EPST_PS_1_1,
#endif			
	mc, 
	video::EMT_TRANSPARENT_ALPHA_CHANNEL,
	NULL);

	mc->drop();

}

void FireWorkShader::OnSetMaterial(const SMaterial& material) 
{ 

	//COGLES2Driver* es2driver =  (COGLES2Driver*)mDevice->getVideoDriver();
	//es2driver->blendFunc( EBF_SRC_ALPHA, EBF_ONE);
	COGLES2Driver* es2driver =  (COGLES2Driver*)mDevice->getVideoDriver();
	es2driver->blendFunc( EBF_SRC_ALPHA, EBF_ONE);
    es2driver->enableBlend();
	  
}

void FireWorkShader::OnSetConstants(video::IMaterialRendererServices* services,
		s32 userData)
{		
	
	COGLES2Driver* es2driver =  (COGLES2Driver*)mDevice->getVideoDriver();

	f32 timer = GetElapsedTime();
	video::SColorf scolor = GetFireColor();
	f32 elapsedTime  = timer;


	video::IVideoDriver* driver = services->getVideoDriver();
	core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(video::ETS_PROJECTION);
	worldViewProj *= driver->getTransform(video::ETS_VIEW);
	worldViewProj *= driver->getTransform(video::ETS_WORLD);
	services->setVertexShaderConstant("uMvpMatrix", worldViewProj.pointer(), 1);
	
	float colorr[4];
	colorr[0]= scolor.r;
	colorr[1]= scolor.g;
	colorr[2]= scolor.b;
	colorr[3]= scolor.a;
	
	

	core::vector3df pos = mDevice->getSceneManager()->getActiveCamera()->getAbsolutePosition();		
	core::matrix4 world = driver->getTransform(video::ETS_WORLD);
	world = world.getTransposed();
	services->setVertexShaderConstant("uWorldMatrix", world.pointer(), 1);

	services->setVertexShaderConstant("uFogColor", &colorr[0], 1);
	

	services->setVertexShaderConstant("uTimer", &elapsedTime, 1);
		
		
}
