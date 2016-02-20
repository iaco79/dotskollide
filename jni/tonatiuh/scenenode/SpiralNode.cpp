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
#include "shaders/SpiralShader.h"
#include "scenenode/SpiralNode.h"



u32 SpiralNode::mVertexVBO=0;
u32 SpiralNode::mIndexVBO=0;
u32 SpiralNode::mVBOInitialed = false;

 SpiralNode::SpiralNode(ISceneNode* parent, ISceneManager* mgr, s32 id,f32 factor):
		Base2dNode(parent, mgr, id),
		mFactor(factor)            
        {     

			if(!mVBOInitialed)
			{
				mVBOInitialed= true;
				InitialzeVBO(64,64,mgr);
			}

            mMaterial.Wireframe = false;
            mMaterial.Lighting = false;                     
			mMaterial.setFlag(EMF_BILINEAR_FILTER,false);
			setMaterialType((video::E_MATERIAL_TYPE)SpiralShader::gSpiralMaterialType);

        };
        

void SpiralNode::render()
{   

      IVideoDriver* driver = SceneManager->getVideoDriver();

	  
      driver->setMaterial(mMaterial);
	  IMaterialRenderer*mat = driver->getMaterialRenderer(mMaterial.MaterialType);
	  	  
	  SpiralShader::SetUniforms((float)mElapsedTime * mFactor);
	
      driver->setTransform(ETS_WORLD, getRelativeTransformation());
      

	  COGLES2Driver* es2driver =  (COGLES2Driver*)driver;
	  es2driver->drawVertexPrimitiveListVBO(
			NULL, SPIRAL_MAXVERTEX,
			NULL, SPIRAL_MAXINDEX/3,
			EVT_STANDARD, 
			EPT_TRIANGLES,
			EIT_16BIT,
			mVertexVBO,
			mIndexVBO						
			);

				  
}     


void SpiralNode::Load(char* filename,s32 frameWidth,s32 frameHeight, float width, float height)
{
    IVideoDriver* driver = SceneManager->getVideoDriver();

    float x = width/2.0f;
    float y = height/2.0f;
               
 			
	mBox.reset(vector3df(-1.0,-1.0,-1.0));
	mBox.addInternalPoint(vector3df(1.0,1.0,1.0));
			
	setTexture0(driver, filename);
    
    mMaterial.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
                                     
}
              

void SpiralNode::InitialzeVBO(float fWidth, float fHeight,ISceneManager* mgr)
{
	S3DVertex* Vertices = new S3DVertex[SPIRAL_MAXVERTEX];
	u16* Indices = new u16[SPIRAL_MAXINDEX];

	float radiusW = 0.15f;
	float radiusH = 1.0f;
	float angle = 360.0f /(SPIRAL_MAXVERTEX/4.0f);

	int q=0;

	for(int a=0;a<SPIRAL_MAXVERTEX;a+=4)
	{
		
		int  nn = a/4;
		float step = (float)nn;


		float nangle = (step*angle +   0.0f)*3.1416f/180.0f;
		float vangle = (step*angle +  90.0f)*3.1416f/180.0f;
		float vangle2 =(step*angle + 270.0f)*3.1416f/180.0f;

		//float dangle = (angle/2)*3.1416/180;
		float nx=cos(nangle); 
		float ny=sin(nangle); 

		float speedi= ((float)(rand()%40+10)/100.0f)+0.10f;				
		float speedo= speedi; //((float)(rand()%3+1)/100.0f) + speedi;//0.03f;

		float zn=0.0f;
		SColor extracolor(255,255,255,64);
		int j = rand()%3;
		if(nn%3==0)
		{
			zn=1.0f;		

			switch(j)
			{
				case 0:extracolor=SColor(255,255,32,200);break;
				case 1:extracolor=SColor(255,255,0,0);break;
				case 2:extracolor=SColor(255,255,100,0);break;
			}			

		}
		
		float x = cos(vangle)*radiusW + nx*radiusH; 
		float y = sin(vangle)*radiusW + ny*radiusH; 	
		extracolor.setAlpha(255);
		Vertices[a+0] = S3DVertex(0.5,0.5,nangle, nx*speedo,ny*speedo,zn,extracolor,0,1);

		x = cos(vangle2)*radiusW + nx*radiusH; 
		y = sin(vangle2)*radiusW + ny*radiusH; 
		Vertices[a+1] = S3DVertex(-0.5,0.5,nangle, nx*speedo,ny*speedo,zn,extracolor,0,1);	
		
		switch(j)
		{
			case 0:extracolor=SColor(155,205,32,200);break;
			case 1:extracolor=SColor(155,205,0,0);break;
			case 2:extracolor=SColor(155,205,100,0);break;
		}		

		extracolor.setAlpha(100);
		x = cos(vangle)*radiusW; 
		y = sin(vangle)*radiusW; 
		Vertices[a+2] = S3DVertex(-0.5,-0.5,nangle, nx*speedi,ny*speedi,zn,extracolor,0,1);	

		x = cos(vangle2)*radiusW; 
		y = sin(vangle2)*radiusW; 
		Vertices[a+3] = S3DVertex(0.5,-0.5,nangle, nx*speedi,ny*speedi,zn,extracolor,0,1);	

	   float tx = 0;
	   float ty = 0;
	   /*Vertices[a+0].TCoords.X = tx;
	   Vertices[a+0].TCoords.Y = ty;
	   Vertices[a+1].TCoords.X = tx;
	   Vertices[a+1].TCoords.Y = ty+1.0;
	   Vertices[a+2].TCoords.X = tx+1.0;
	   Vertices[a+2].TCoords.Y = ty+1.0;
	   Vertices[a+3].TCoords.X = tx+1.0;
	   Vertices[a+3].TCoords.Y = ty;
	   */

	   Vertices[a+0].TCoords.X = tx+1.0;
	   Vertices[a+0].TCoords.Y = ty;
	   Vertices[a+1].TCoords.X = tx;
	   Vertices[a+1].TCoords.Y = ty;
	   Vertices[a+2].TCoords.X = tx;
	   Vertices[a+2].TCoords.Y = ty+1.0;
	   Vertices[a+3].TCoords.X = tx+1.0;
	   Vertices[a+3].TCoords.Y = ty+1.0;
    
	   Indices[q+0] = a+1;
	   Indices[q+1] = a+0;
	   Indices[q+2] = a+2;

	   Indices[q+3] = a+2;
	   Indices[q+4] = a+0;
	   Indices[q+5] = a+3;

	   q+=6;
	
	}


  IVideoDriver* driver = mgr->getVideoDriver();
  COGLES2Driver* es2driver =  (COGLES2Driver*)driver;

  es2driver->genVBO(Vertices,SPIRAL_MAXVERTEX,
					Indices,SPIRAL_MAXINDEX,
					mVertexVBO,
					mIndexVBO);

  delete[] Vertices;
  delete[] Indices;
}


