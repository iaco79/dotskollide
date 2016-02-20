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
//#include "COGLES2SLMaterialRenderer.h"
#include <Box2D.h>
#include "GameDefines.h"
#include "GGameApp.h"
#include "TonatiuhDefines.h"
#include "shaders/MetaballShaderPre.h"

s32 MetaballShaderPre::gMetaballMaterialType=0;	


MetaballShaderPre::MetaballShaderPre()
{ 
	
}



	

void MetaballShaderPre::Initialize()
{
	
	irr::IrrlichtDevice* device = GGameApp::getInstance()->getIrrlichtDevice();
	video::IGPUProgrammingServices* gpu = device->getVideoDriver()->getGPUProgrammingServices();
	
	MetaballShaderPre* mc = new MetaballShaderPre();
	mc->mDevice = device;

	gMetaballMaterialType = gpu->addHighLevelShaderMaterialFromFiles(
#if defined(_IRR_ANDROID_PLATEFORM_)
  	"MetaballShaderPre.vsh", "main", video::EVST_VS_1_1,
	"MetaballShaderPre.fsh", "main", video::EPST_PS_1_1,
#else
	"IrrlichtSdcard/MetaballShaderPre.vsh", "main", video::EVST_VS_1_1,
	"IrrlichtSdcard/MetaballShaderPre.fsh", "main", video::EPST_PS_1_1,
#endif			
	mc, 
	video::EMT_TRANSPARENT_ALPHA_CHANNEL,
	NULL);

	mc->drop();

}

void MetaballShaderPre::OnSetMaterial(const SMaterial& material) 
{ 

	//COGLES2Driver* es2driver =  (COGLES2Driver*)mDevice->getVideoDriver();
	//es2driver->blendFunc( EBF_SRC_ALPHA, EBF_ONE);
	COGLES2Driver* es2driver =  (COGLES2Driver*)mDevice->getVideoDriver();
	es2driver->blendFunc( EBF_SRC_COLOR, EBF_ONE);
    es2driver->enableBlend();
}

void MetaballShaderPre::OnSetConstants(video::IMaterialRendererServices* services,
		s32 userData)
{		
	
	COGLES2Driver* es2driver =  (COGLES2Driver*)mDevice->getVideoDriver();
	
	services->setPixelShaderConstant("uTextureUnit0",0);
	services->setPixelShaderConstant("uTextureUnit1",1);
	
	video::IVideoDriver* driver = services->getVideoDriver();
	core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(video::ETS_PROJECTION);
	worldViewProj *= driver->getTransform(video::ETS_VIEW);
	worldViewProj *= driver->getTransform(video::ETS_WORLD);
	services->setVertexShaderConstant("uMvpMatrix", worldViewProj.pointer(), 1);
	
		
	core::vector3df pos = mDevice->getSceneManager()->getActiveCamera()->getAbsolutePosition();		
	core::matrix4 world = driver->getTransform(video::ETS_WORLD);
	world = world.getTransposed();
	services->setVertexShaderConstant("uWorldMatrix", world.pointer(), 1);
	
		
}
