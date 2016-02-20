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
#include <Box2D.h>
#include "GameDefines.h"
#include "GComponent.h"
#include "GObject.h"
#include "GController.h"
#include "GControllerComponent.h"
#include "GSpriteComponent.h"
#include "irrnode/Base2dNode.h"
#include "TonatiuhDefines.h"
#include "scenenode/EnergyBarNode.h"



void EnergyBarNode::Load(char* filename,s32 frameWidth,s32 frameHeight)
{
    IVideoDriver* driver = SceneManager->getVideoDriver();

//    float x = width/2.0f;
 //   float y = height/2.0f;
   
	 
	setTexture0(driver, filename);

    mMaterial.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;


	dimension2d<s32> size = (dimension2d<s32>)mTexture->getOriginalSize();
    fWidth  = (float)frameWidth/(float)size.Width;
    fHeight = (float)frameHeight/(float)size.Height;
	stepww = size.Width / frameWidth;
    stephh = size.Height / frameHeight;



	
	frame = 7;
                          
}
EnergyBarNode::EnergyBarNode(scene::ISceneNode* parent, 
			  scene::ISceneManager* mgr, 
			  s32 id, 
			  float width, 
			  float height,
			  video::SColor startColor,
			  video::SColor endColor,
			  video::SColor frameColor,
			  int type
			  ) :  
	  Base2dNode(parent, mgr, id),
	  mEnergyValue(1.0f),
	  mEnergyNeedsUpdate(true),
	  mStartColor(startColor),
	  mEndColor(endColor),
	  mFrameColor(frameColor),
	  mHeight(height),
	  mWidth(width),
	  mType(type)
{
	float textSize=512.0f;
	float tx =  0.0f/ textSize;
	float ty =  0.0f / textSize;
	float tx2 =  76.0f / textSize;
	float ty2 =  160.0f/ textSize;

	if(type == 0)
	{
		tx =  0.0f/ textSize;
		ty =  0.0f / textSize;
		tx2 =  76.0f / textSize;
		ty2 =  160.0f/ textSize;
		frameUV1.set(tx, ty);
		frameUV2.set(tx2, ty2);
		tx =  78.0f/ textSize;
		ty =  0.0f / textSize;
		tx2 =  118.0f / textSize;
		ty2 =  160.0f/ textSize;	
		barUV1.set(tx, ty);
		barUV2.set(tx2, ty2);
		mXFactor=0.0f;
		mYFactor=0.0f;
		mXFactor2=0.0f;
		mYFactor2=12.0f / textSize;

		mXFactor2=9.0f / textSize;	
		mXFactor = mYFactor2/(ty2-ty);
		mXFactor = mXFactor * width;
		mYFactor = mXFactor2/(tx2-tx);
		mYFactor = mYFactor * height;
	}
	else
	{
		tx =  128.0f/ textSize;
		ty =  0.0f / textSize;
		tx2 = 511.0f / textSize;
		ty2 =  76.0f/ textSize;
		frameUV1.set(tx, ty);
		frameUV2.set(tx2, ty2);
		tx =  128.0f/ textSize;
		ty =  78.0f / textSize;
		tx2 =  511.0f / textSize;
		ty2 =  118.0f/ textSize;	
		barUV1.set(tx, ty);
		barUV2.set(tx2, ty2);
		mXFactor=0.0f;
		mYFactor=0.0f;
		mXFactor2=0.0f;
		mYFactor2=12.0f / textSize;
		mXFactor2=9.0f / textSize;	

		mXFactor = mYFactor2/(tx2-tx);
		mXFactor = mXFactor * width;

		mYFactor = mXFactor2/(ty2-ty);
		mYFactor = mYFactor * height;
	
	}



	irr::video::SColor scolor(255,255,255,255);

	mMaterial.Wireframe = false;
	mMaterial.Lighting = false;
	mMaterial.AmbientColor = scolor;
	mMaterial.DiffuseColor = scolor;
	mMaterial.EmissiveColor  = scolor;
	mMaterial.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;


	//internal frame
	/*
	vertices[0] = video::S3DVertex(-width/2, height/2,0.01f,0,0,0, mFrameColor,0,0);
	vertices[1] = video::S3DVertex( width/2, height/2,0.01f,0,0,0, mFrameColor,0,1);
	vertices[2] = video::S3DVertex(-width/2,-height/2,0.01f,0,0,0, mFrameColor,1,0);
	vertices[3] = video::S3DVertex( width/2,-height/2,0.01f,0,0,0, mFrameColor,1,1);
	*/

	vertices[0] = video::S3DVertex(-width/2, height/2, 0.001f,0,0,0, mFrameColor,0,0);
	vertices[1] = video::S3DVertex( width/2, height/2, 0.001f,0,0,0, mFrameColor,0,1);
	vertices[2] = video::S3DVertex(-width/2,-height/2, 0.001f,0,0,0, mFrameColor,1,0);
	vertices[3] = video::S3DVertex( width/2,-height/2, 0.001f,0,0,0, mFrameColor,1,1);	
	/*
	vertices[4] =  video::S3DVertex((-width/2)+mXFactor,( height/2)-mYFactor,  0.01f,0,0,0, mFrameColor,0,0);
	vertices[5] =  video::S3DVertex(( width/2)-mXFactor,( height/2)-mYFactor,  0.01f,0,0,0, mFrameColor,  0,1);	
	vertices[6] = video::S3DVertex((-width/2) +mXFactor,(-height/2)+mYFactor,   0.01f,0,0,0, mFrameColor,1,0);
	vertices[7] = video::S3DVertex(( width/2) -mXFactor,(-height/2)+mYFactor,   0.01f,0,0,0, mFrameColor,  1,1);
	*/
	//bar 
	vertices[4] = video::S3DVertex((-width/2),         ( height/2)-mYFactor,0,0,0,0, mStartColor,0,0);
	vertices[5] = video::S3DVertex((-width/2)+mXFactor,( height/2)-mYFactor,0,0,0,0, mEndColor,  0,1);	
	vertices[6] = video::S3DVertex((-width/2),         (-height/2)+mYFactor,0,0,0,0, mStartColor,1,0);
	vertices[7] = video::S3DVertex((-width/2)+mXFactor,(-height/2)+mYFactor,-0,0,0,0,mEndColor,  1,1);

	


	u16 tmpindices[]= { 0,1,2, 
						1,3,2,  

						4,5,6, 
						5,7,6, 

						8,9,10, 
						9,11,10
						};

	for (s32 i=0; i<18; ++i)
	{
		indices[i]= tmpindices[i];
	}

	mBox.reset(vertices[0].Pos);
	for (s32 i=0; i<12; i++)
	{
		mBox.addInternalPoint(vertices[i].Pos);
	}


}
	 
	  

void EnergyBarNode::setEnergyValue(float value)
{

	if(value != mEnergyValue)
		mEnergyNeedsUpdate=true;

	mEnergyValue = value;

}	


void EnergyBarNode::render()
{    
	float length = frameUV2.Height-(2.0f*mYFactor2);

	if(mType==1)
	{
		length = (frameUV2.Width-frameUV1.Width) - (2.0f*mYFactor2);

	}

	length = length * mEnergyValue;

	
	if(mEnergyNeedsUpdate)
	{
		float len2 = (mWidth-2.0f*mXFactor);
			vertices[5] = video::S3DVertex(				
				(-mWidth/2)+mXFactor+ (len2*mEnergyValue),
				( mHeight/2)-mYFactor,0,0,0,0, mEndColor,0,0);
			
			vertices[5].Color = mEndColor.getInterpolated(mStartColor,mEnergyValue);
			
			
			vertices[7] = video::S3DVertex(
				(-mWidth/2)+mXFactor+ (len2*mEnergyValue),
				(-mHeight/2)+mYFactor,0,0,0,0, mEndColor,0,0);
			
			vertices[7].Color = mEndColor.getInterpolated(mStartColor,mEnergyValue);

		mEnergyNeedsUpdate=false;
	}

	//float tx = (frame  % stepww)*fWidth;
	//float ty = (frame  / stepww)*fHeight;

	//vertices[0] = video::S3DVertex(-width/2, height/2,0.02f,0,0,0, mFrameColor,0,0);
	//vertices[1] = video::S3DVertex( width/2, height/2,0.02f,0,0,0, mFrameColor,0,1);
	//vertices[2] = video::S3DVertex(-width/2,-height/2,0.02f,0,0,0, mFrameColor,1,0);
	//vertices[3] = video::S3DVertex( width/2,-height/2,0.02f,0,0,0, mFrameColor,1,1);
	
	int a = 0;

	if(mType==0)
	{

		vertices[a+0].TCoords.X = frameUV1.Width;
		vertices[a+0].TCoords.Y = frameUV1.Height;
		vertices[a+1].TCoords.X = frameUV1.Width;
		vertices[a+1].TCoords.Y = frameUV2.Height;
		vertices[a+2].TCoords.X = frameUV2.Width;
		vertices[a+2].TCoords.Y = frameUV1.Height;
		vertices[a+3].TCoords.X = frameUV2.Width;
		vertices[a+3].TCoords.Y = frameUV2.Height;

		a=4;
		vertices[a+0].TCoords.X = barUV1.Width;
		vertices[a+0].TCoords.Y = barUV1.Height; //barUV2.Height-mYFactor2-length;
		vertices[a+1].TCoords.X = barUV1.Width;
		vertices[a+1].TCoords.Y = barUV1.Height+mYFactor2+length;

		vertices[a+2].TCoords.X = barUV2.Width;
		vertices[a+2].TCoords.Y = barUV1.Height; //barUV2.Height-mYFactor2-length;
		vertices[a+3].TCoords.X = barUV2.Width;
		vertices[a+3].TCoords.Y = barUV1.Height+mYFactor2+length;
	}
	else
	{
		vertices[a+0].TCoords.X = frameUV1.Width;
		vertices[a+0].TCoords.Y = frameUV2.Height;

		vertices[a+1].TCoords.X = frameUV2.Width;
		vertices[a+1].TCoords.Y = frameUV2.Height;

		vertices[a+2].TCoords.X = frameUV1.Width;
		vertices[a+2].TCoords.Y = frameUV1.Height;
		vertices[a+3].TCoords.X = frameUV2.Width;
		vertices[a+3].TCoords.Y = frameUV1.Height;

		a=4;
		vertices[a+0].TCoords.X = barUV1.Width;
		vertices[a+0].TCoords.Y = barUV2.Height;

		vertices[a+1].TCoords.X = barUV1.Width+mYFactor2+length;
		vertices[a+1].TCoords.Y = barUV2.Height;

		vertices[a+2].TCoords.X = barUV1.Width;
		vertices[a+2].TCoords.Y = barUV1.Height;
		vertices[a+3].TCoords.X = barUV1.Width+mYFactor2+length;
		vertices[a+3].TCoords.Y = barUV1.Height;
	
	
	}
	/*
	a=8;
	vertices[a+0].TCoords.X = tx;
	vertices[a+0].TCoords.Y = ty;
	vertices[a+1].TCoords.X = tx;
	vertices[a+1].TCoords.Y = ty+fHeight;
	vertices[a+2].TCoords.X = tx+fWidth;
	vertices[a+2].TCoords.Y = ty;
	vertices[a+3].TCoords.X = tx+fWidth;
	vertices[a+3].TCoords.Y = ty+ fHeight;
	*/
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
    driver->setMaterial(mMaterial);  
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    driver->drawVertexPrimitiveList(&vertices[0],8,&indices[0],4);
}