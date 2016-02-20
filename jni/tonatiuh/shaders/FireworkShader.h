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
#ifndef TFIREWORKSHADER_CLASS
#define TFIREWORKSHADER_CLASS

using namespace irr;
using namespace irr::core;
using namespace	irr::video;
using namespace	irr::scene;


class FireWorkShader: public video::IShaderConstantSetCallBack
{

private:

	irr::IrrlichtDevice* mDevice;

public:

	FireWorkShader();
	
	//in seconds
	static s32 gFireWorkMaterialType;	
	static irr::f32 GetElapsedTime(); 	
	static irr::video::SColorf GetFireColor(); 	
	static irr::f32 gElapsedTime;	
	static irr::video::SColorf gFireColor;

	static void SetUniforms(irr::f32 elapsedTime,irr::video::SColorf fireColor);
	static void Initialize();	

	
	virtual void OnSetMaterial(const SMaterial& material);
	virtual void OnSetConstants(video::IMaterialRendererServices* services,s32 userData);
};

#endif