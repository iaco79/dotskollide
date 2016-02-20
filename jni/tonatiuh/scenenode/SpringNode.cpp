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
#include <Box2d.h>
#include "spline/spline.h"
#include "irrnode/Base2dNode.h"
#include "springrid/SpringGrid.h"
#include "scenenode/SpringNode.h"

  
void SpringNode::Load(char* filename,s32 frameWidth,s32 frameHeight, float width, float height)
{
    IVideoDriver* driver = SceneManager->getVideoDriver();

    float x = width/2.0f;
    float y = height/2.0f;
	rendercount=0;
 			
	mBox.reset(vector3df(-grid->getTotalWidth()/2.0f,-grid->getTotalHeight()/2.0f,0));
	mBox.addInternalPoint(vector3df(grid->getTotalWidth()/2.0f,grid->getTotalHeight()/2.0f,0));
	
	//setTexture0(driver, filename);
	mMaterial.setTexture(0,0);
    mMaterial.MaterialType = EMT_SOLID;
	mMaterial.ZBuffer = ECFN_NEVER;

	/*
	dimension2d<s32> size = (dimension2d<s32>)mTexture->getOriginalSize();
    fWidth  = (float)frameWidth/(float)size.Width;
    fHeight = (float)frameHeight/(float)size.Height;
	stepww = size.Width / frameWidth;
    stephh = size.Height / frameHeight;
	*/
       
	mSmoothPointsW.clear();
	mSmoothPointsW.initWithSize(grid->getMassCountW()-1);

	mSmoothPointsH.clear();
	mSmoothPointsH.initWithSize(grid->getMassCountH()-1);


}

void SpringNode::render()
{   
    IVideoDriver* driver = SceneManager->getVideoDriver();
    driver->setMaterial(mMaterial);				  
    driver->setTransform(ETS_WORLD, AbsoluteTransformation);
                  
				  
	int quads = (grid->getMassCountH()-1)*(grid->getMassCountW()-1);
	int vertexCount = quads*4;
	int indexCount = quads*2*3;
	int q=0;
				  	
				  
	SColor color = VertexColor;
	float zcolor=0.0f;
	float zfactor=0.5f;
	SColor extracolor;
	SColorHSL hsl;
	hsl.fromRGB(VertexColor);
	
	int a=0;
	float hue = hsl.Hue;


	
	//draw horizontal lines
	for(int j=0;j<(grid->getMassCountH());j++)
	{
		
	for(int i=0;i<(grid->getMassCountW()-1);i++)
	{
		f32 x = grid->getMass(i,j)->pos.x;
		f32 y = grid->getMass(i,j)->pos.y;
		f32 z = grid->getMass(i,j)->pos.z;


		vector2df vel= vector2df(grid->getMass(i,j)->oripos.x,grid->getMass(i,j)->oripos.y)
						- vector2df(grid->getMass(i,j)->pos.x,grid->getMass(i,j)->pos.y);
		zcolor = (zfactor*vel.getLengthSQ());
						
		
		if(zcolor>0.6f) zcolor = 0.6f;
		hsl = irr::video::SColorHSL(hue,0.4f+zcolor,0.2+zcolor/3.0f);				
		hsl.toRGB(extracolor);
		extracolor.setAlpha(0.8f);

		Vertices[a+0] = S3DVertex(x,y,z, 0,0,0,extracolor,0,1);	
						

		x = grid->getMass(i+1,j)->pos.x;
		y = grid->getMass(i+1,j)->pos.y;
		z = grid->getMass(i+1,j)->pos.z;

		

		vel= vector2df(grid->getMass(i,j)->pos.x,grid->getMass(i,j)->pos.y)-vector2df(grid->getMass(i,j)->oripos.x,grid->getMass(i,j)->oripos.y);
		zcolor = (zfactor*vel.getLengthSQ());

		
		
		if(zcolor>0.5f) zcolor = 0.5f;
		hsl = irr::video::SColorHSL(hue,0.4f+zcolor,0.2+zcolor/3.0f);				
		hsl.toRGB(extracolor);
		extracolor.setAlpha(0.8f);
	
		Vertices[a+1] = S3DVertex(x,y,z, 0,0,0,extracolor,1,1);
		
		           
		Indices[q+0] = a+1;
		Indices[q+1] = a+0;
		

		a+=2;
		q+=2;
	}

	}


	
	driver->drawVertexPrimitiveList(&Vertices[0],a,&Indices[0], q/2,EVT_STANDARD, EPT_LINES);

	
	//draw vertical lines
	a=0;
	q=0;
	for(int j=0;j<(grid->getMassCountH()-1);j++)
	{
	for(int i=0;i<(grid->getMassCountW());i++)
	{
		

		f32 x = grid->getMass(i,j)->pos.x;
		f32 y = grid->getMass(i,j)->pos.y;
		f32 z = grid->getMass(i,j)->pos.z;
		vector2df vel= vector2df(grid->getMass(i,j)->pos.x,grid->getMass(i,j)->pos.y)-vector2df(grid->getMass(i,j)->oripos.x,grid->getMass(i,j)->oripos.y);
		zcolor = (zfactor*vel.getLengthSQ());

		if(zcolor>0.6f) zcolor = 0.6f;
		hsl = irr::video::SColorHSL(hue,0.4f+zcolor,0.2+zcolor/3.0f);				
		hsl.toRGB(extracolor);
		extracolor.setAlpha(0.8f);


		Vertices[a+0] = S3DVertex(x,y,z, 0,0,0,extracolor,1,0);
						

		x = grid->getMass(i,j+1)->pos.x;
		y = grid->getMass(i,j+1)->pos.y;
		z = grid->getMass(i,j+1)->pos.z;
		vel= vector2df(grid->getMass(i,j)->pos.x,grid->getMass(i,j)->pos.y)-vector2df(grid->getMass(i,j)->oripos.x,grid->getMass(i,j)->oripos.y);
		zcolor = (zfactor*vel.getLengthSQ());
		
		if(zcolor>0.6f) zcolor = 0.6f;
		hsl = irr::video::SColorHSL(hue,0.4f+zcolor,0.2+zcolor/3.0f);			
		hsl.toRGB(extracolor);
		extracolor.setAlpha(0.8f);


		Vertices[a+1] = S3DVertex(x,y,z, 0,0,0,extracolor,0,0);
				           
		Indices[q+0] = a+1;
		Indices[q+1] = a+0;		
		a+=2;
		q+=2;
	}
	}


	
	driver->drawVertexPrimitiveList(&Vertices[0],a,&Indices[0], q/2,EVT_STANDARD, EPT_LINES);


	//draw midle horizontal lines
	a=0;
	q=0;
	for(int j=0;j<(grid->getMassCountH()-1);j++)
	{

		
		//interpolate lines
		for(int i=0;i<grid->getMassCountW()-1;i++)
		{
			mSmoothPointsW.SetPoint(i,b2Vec2(
						(grid->getMass(i,j)->pos.x + grid->getMass(i+1,j+1)->pos.x)/2.0f,
						(grid->getMass(i,j)->pos.y + grid->getMass(i+1,j+1)->pos.y)/2.0f));
		}
		//mSmoothPointsW.interPolateAll();


	for(int i=0;i<(grid->getMassCountW()-2);i++)
	{
		//f32 x = (grid->getMass(i,j)->pos.x + grid->getMass(i+1,j+1)->pos.x)/2.0f;
		//f32 y = (grid->getMass(i,j)->pos.y + grid->getMass(i+1,j+1)->pos.y)/2.0f;
		f32 z = (grid->getMass(i,j)->pos.z + grid->getMass(i+1,j+1)->pos.z)/2.0f;
		f32 x=mSmoothPointsW.points[i].x;
		f32 y=mSmoothPointsW.points[i].y;


		vector2df vel= vector2df(grid->getMass(i,j)->oripos.x,grid->getMass(i,j)->oripos.y)
						- vector2df(grid->getMass(i,j)->pos.x,grid->getMass(i,j)->pos.y);
		zcolor = (zfactor*vel.getLengthSQ());
								
		if(zcolor>0.6f) zcolor = 0.6f;
		hsl = irr::video::SColorHSL(hue,0.4f+zcolor,0.4+zcolor/3.0f);				
		hsl.toRGB(extracolor);
		extracolor.setAlpha(0.8f);


		
		Vertices[a+0] = S3DVertex(x,y,z, 0,0,0,extracolor,0,1);	
		
		x=mSmoothPointsW.points[i+1].x;
		y=mSmoothPointsW.points[i+1].y;

		//x = (grid->getMass(i+1,j)->pos.x + grid->getMass(i+2,j+1)->pos.x)/2.0f;
		//y = (grid->getMass(i+1,j)->pos.y + grid->getMass(i+2,j+1)->pos.y)/2.0f;
		z = (grid->getMass(i+1,j)->pos.z + grid->getMass(i+2,j+1)->pos.z)/2.0f;
		vel= vector2df(grid->getMass(i,j)->pos.x,grid->getMass(i,j)->pos.y)-vector2df(grid->getMass(i,j)->oripos.x,grid->getMass(i,j)->oripos.y);
		zcolor = (zfactor*vel.getLengthSQ());

		
		if(zcolor>0.5f) zcolor = 0.5f;
		hsl = irr::video::SColorHSL(hue,0.4f+zcolor,0.4+zcolor/3.0f);				
		hsl.toRGB(extracolor);
		extracolor.setAlpha(0.8f);

			
		

		Vertices[a+1] = S3DVertex(x,y,z, 0,0,0,extracolor,1,1);
		
		           
		Indices[q+0] = a+1;
		Indices[q+1] = a+0;
		

		a+=2;
		q+=2;
	}

	}

	
	driver->drawVertexPrimitiveList(
	&Vertices[0],a,
	&Indices[0], q/2,
	EVT_STANDARD, 
	EPT_LINES);

	//draw midle vertical lines
	a=0;
	q=0;
	
	for(int i=0;i<(grid->getMassCountW()-1);i++)
	{

	//interpolate lines
		for(int j=0;j<(grid->getMassCountH()-1);j++)
		{
			mSmoothPointsH.SetPoint(j,b2Vec2(
						(grid->getMass(i,j)->pos.x + grid->getMass(i+1,j+1)->pos.x)/2.0f,
						(grid->getMass(i,j)->pos.y + grid->getMass(i+1,j+1)->pos.y)/2.0f));
		}
		//mSmoothPointsH.interPolateAll();
		
	for(int j=0;j<(grid->getMassCountH()-2);j++)
	{
		f32 x=mSmoothPointsH.points[j].x;
		f32 y=mSmoothPointsH.points[j].y;
		f32 z = (grid->getMass(i,j)->pos.z + grid->getMass(i+1,j+1)->pos.z)/2.0f;

		vector2df vel= vector2df(grid->getMass(i,j)->oripos.x,grid->getMass(i,j)->oripos.y)
						- vector2df(grid->getMass(i,j)->pos.x,grid->getMass(i,j)->pos.y);
		zcolor = (zfactor*vel.getLengthSQ());
								
		if(zcolor>0.6f) zcolor = 0.6f;
		hsl = irr::video::SColorHSL(hue,0.4f+zcolor,0.4+zcolor/3.0f);				
		hsl.toRGB(extracolor);
		extracolor.setAlpha(0.8f);



		Vertices[a+0] = S3DVertex(x,y,z, 0,0,0,extracolor,0,1);	
						
		x=mSmoothPointsH.points[j+1].x;
		y=mSmoothPointsH.points[j+1].y;
		z = (grid->getMass(i,j+1)->pos.z + grid->getMass(i+1,j+2)->pos.z)/2.0f;
		vel= vector2df(grid->getMass(i,j)->pos.x,grid->getMass(i,j)->pos.y)-vector2df(grid->getMass(i,j)->oripos.x,grid->getMass(i,j)->oripos.y);
		zcolor = (zfactor*vel.getLengthSQ());

		
		
		if(zcolor>0.5f) zcolor = 0.5f;
		hsl = irr::video::SColorHSL(hue,0.4f+zcolor,0.4+zcolor/3.0f);				
		hsl.toRGB(extracolor);
		extracolor.setAlpha(0.8f);

				
		Vertices[a+1] = S3DVertex(x,y,z, 0,0,0,extracolor,1,1);
		
		           
		Indices[q+0] = a+1;
		Indices[q+1] = a+0;
		

		a+=2;
		q+=2;
	}

	}

	driver->drawVertexPrimitiveList(
	&Vertices[0],a,
	&Indices[0], q/2,
	EVT_STANDARD, 
	EPT_LINES);

}