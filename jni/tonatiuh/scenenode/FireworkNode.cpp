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
#include "GameDefines.h"
#include "GComponent.h"
#include "GObject.h"
#include "GController.h"
#include "GControllerComponent.h"
#include "irrnode/Base2dNode.h"
#include "TonatiuhDefines.h"
#include "shaders/FireWorkShader.h"
#include "scenenode/FireworkNode.h"


u32 FireworkNode::mVertexVBO=0;
u32 FireworkNode::mIndexVBO=0;
u32 FireworkNode::mVBOInitialed = false;


 FireworkNode::FireworkNode(
		ISceneNode* parent, 
		ISceneManager* mgr, 
		s32 id): 
	Base2dNode(parent, mgr, id)
{     
	//initiale the VBO 
	if(!mVBOInitialed)
	{
		mVBOInitialed= true;
		InitialzeVBO(64,64,mgr);
	}

	mFireColor = SColor(255,255,255,255);

    mMaterial.Wireframe = false;
    mMaterial.Lighting = false;    					  
	mMaterial.ZBuffer = ECFN_NEVER;
	mMaterial.MaterialType = EMT_SOLID ;
	
	setMaterialType((video::E_MATERIAL_TYPE)FireWorkShader::gFireWorkMaterialType);

	mBox.reset(vector3df(-1.0,-1.0,-1.0));
	mBox.addInternalPoint(vector3df(1.0,1.0,1.0));
	mMaterial.setTexture(0,NULL); 

	setTransparent(false);


}     
 


void FireworkNode::Load(char* filename,s32 frameWidth,s32 frameHeight, float width, float height)
{
    IVideoDriver* driver = SceneManager->getVideoDriver();

    float x = width/2.0f;
    float y = height/2.0f;
    setTexture0(driver, filename);


    mMaterial.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
	

                                      
}

//elapsed time in seconds?
void FireworkNode::SetElapsedTime(irr::f32 elapsedTime)
{
	mElapsedTime=elapsedTime;
}

void FireworkNode::render()
{   
  IVideoDriver* driver = SceneManager->getVideoDriver();

	  
      driver->setMaterial(mMaterial);
	  IMaterialRenderer*mat = driver->getMaterialRenderer(mMaterial.MaterialType);

	  //set the firework shader uniforms

	  FireWorkShader::SetUniforms(mElapsedTime, irr::video::SColorf(mFireColor));

	  driver->setTransform(ETS_WORLD, AbsoluteTransformation);
    
	  //render using VBOs
	  COGLES2Driver* es2driver =  (COGLES2Driver*)driver;
	  es2driver->drawVertexPrimitiveListVBO(
			NULL,FIRE_MAXVERTEX,
			NULL, FIRE_MAXINDEX/3,
			EVT_STANDARD, 
			EPT_TRIANGLES,
			EIT_16BIT,
			mVertexVBO,
			mIndexVBO						
			);
}
     

//initialize the VBO
void FireworkNode::InitialzeVBO(float fWidth, float fHeight,ISceneManager* mgr)
{

	S3DVertex* Vertices = new S3DVertex[FIRE_MAXVERTEX];
	u16* Indices = new u16[FIRE_MAXINDEX];

	
	float angle = 720.0f /(FIRE_MAXVERTEX/4.0f);
	 
	int q=0;
	 
	for(int a=0;a<FIRE_MAXVERTEX;a+=4)
	{
		float step = (float)a/4.0;
		int  nn = a/4;

		float nangle = (step*angle)*3.1416f/180.0f;
		
		float dirX=cos(nangle); 
		float dirY=sin(nangle); 

		//delay float speed = 1.0f+ ((float)(rand()%10))/10.0f;

		float speed = ((float)(1+rand()%5) ) /100.0f;
		float length = 8 + (float)(rand()%10)*0.4f;
		float width = 0.3f;
		float varl = 0.004f + ((float)(rand()%4))/100.0f;

		SColor extracolor(255,0,255,255);
		
		Vertices[a+0] = S3DVertex(dirX,dirY,length, speed, width,varl,extracolor,0,1);
		Vertices[a+1] = S3DVertex(dirX,dirY,length, speed,-width,varl,extracolor,0,1);	

		Vertices[a+2] = S3DVertex(dirX,dirY,length, speed, width,0.0f,extracolor,0,1);	
		Vertices[a+3] = S3DVertex(dirX,dirY,length, speed,-width,0.0f,extracolor,0,1);	

	   float tx = 0;
	   float ty = 0;
	   Vertices[a+0].TCoords.X = tx;
	   Vertices[a+0].TCoords.Y = ty;
	   Vertices[a+1].TCoords.X = tx+1.0f;
	   Vertices[a+1].TCoords.Y = ty;
	   Vertices[a+2].TCoords.X = tx;
	   Vertices[a+2].TCoords.Y = ty+1.0f;
	   Vertices[a+3].TCoords.X = tx+1.0f;
	   Vertices[a+3].TCoords.Y = ty+1.0f;
       	               
	 
	   Indices[q+0] = a+0;
	   Indices[q+1] = a+1;
	   Indices[q+2] = a+3;

	   Indices[q+3] = a+0;
	   Indices[q+4] = a+3;
	   Indices[q+5] = a+2;

	   q+=6;
	
	}

  IVideoDriver* driver = mgr->getVideoDriver();
  COGLES2Driver* es2driver =  (COGLES2Driver*)driver;

  es2driver->genVBO(Vertices,FIRE_MAXVERTEX,
					Indices,FIRE_MAXINDEX,
					mVertexVBO,
					mIndexVBO);
  delete[] Vertices;
  delete[] Indices;
}


