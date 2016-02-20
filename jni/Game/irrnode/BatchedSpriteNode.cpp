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
#include "GTransformComponent.h"
#include "GSpriteComponent.h"
#include "irrnode/Base2dNode.h"
#include "irrnode/BatchedSpriteNode.h"

using namespace irr;


BatchedSpriteNode::BatchedSpriteNode(
	scene::ISceneNode* parent, 
	scene::ISceneManager* mgr, 
	s32 id, 
	int maxObjects) :
		Base2dNode(parent, mgr, id)
	
{
	
	mMaterial.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
	
	//set bounding box = 20 units
	mBox.reset(-10.0f,-10.0f,0.0f);				
	mBox.addInternalPoint(10.0f,10.0f,0.0f);

	//lets create the vertex buffers with maxLines
	mMaxQuads = maxObjects;
	mVertexCount = mMaxQuads*4;
	mIndexCount = mMaxQuads*6;

	Vertices = new S3DVertex[mVertexCount];
	Indices = new u16[mIndexCount];
	
}



void BatchedSpriteNode::render()
{
	if(mGameObjects == NULL)
		return;

	if(mGameObjects->size() == 0)
		return;


	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	
	driver->setMaterial(mMaterial);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	dimension2d<s32> textureSize = (dimension2d<s32>)mTexture->getOriginalSize();

	float lineWidth = 0.1f/2.0f;

	SColor clr = SColor(255,255,255,255);
	SColor clr2 = SColor(255,255,255,255);

	int quadCount=0;
	int a=0;
	int q=0;


	for(int j=0;j<mGameObjects->size();j++)
	{	
		if(quadCount<mMaxQuads)
		{
		
			GObjectPtr go = (*mGameObjects)[j];
			
			//get the sprite component
			GSpriteComponent* sc= go->GetComponent<GSpriteComponent>(go->GetComponentIdByIndex(GC_SPRITE_IDX));

			//get the transform component
			GTransformComponent* tc= go->GetComponent<GTransformComponent>(go->GetComponentIdByIndex(GC_TRANSFORM_IDX));
			

			if(sc && sc->mVisible)
			{
				clr = sc->mColor;
				clr2 = clr;

				if(sc->mUse2Colors)
					clr2 = sc->mColor2;

				f32 bpx= tc->mTrans.p.x;
				f32 bpy= tc->mTrans.p.y;
				f32 width2 = sc->mSize.X /2.0f;
				f32 height2= sc->mSize.Y  /2.0f;
				f32 scalex = sc->mScale.X;
				f32 scaley = sc->mScale.Y;
				f32 anglez = tc->mTrans.q.GetAngle();//  go.mBody->GetAngle(); 

			
				//int frame=go.mSpriteFrame;

				vector3df p11= vector3df( width2, height2,0.0f);
				vector3df p12= vector3df(-width2, height2,0.0f);
				vector3df p21= vector3df(-width2,-height2,0.0f);
				vector3df p22= vector3df( width2,-height2,0.0f);
	 

				core::matrix4 trasmat;
				trasmat.buildTextureTransform( anglez,
											core::vector2df(0.0f,0.0f),
											core::vector2df(bpx,bpy),
											core::vector2df(scalex,scaley));


				trasmat.transformVect(p11);
				trasmat.transformVect(p12);
				trasmat.transformVect(p21);
				trasmat.transformVect(p22);

						//clr = SColor(255,50+(quadCount%20)*10,50,250-(quadCount%10)*20);
				
					
			   Vertices[a+0] = S3DVertex(p11.X,p11.Y,0,0,0,0,clr,0,1);							
			   Vertices[a+1] = S3DVertex(p12.X,p12.Y,0,0,0,0,clr2,1,1);


			   Vertices[a+2] = S3DVertex(p21.X,p21.Y,0,0,0,0,clr2,1,0);
			   Vertices[a+3] = S3DVertex(p22.X,p22.Y,0,0,0,0,clr,0,0);
					   
			   float x =  (float)sc->mUV1.X / textureSize.Width;
			   float y =  (float)sc->mUV1.Y / textureSize.Height;
			   float x2 = (float)sc->mUV2.X / textureSize.Width;
			   float y2 = (float)sc->mUV2.Y / textureSize.Height;
		   
			   float fWidth  =  x2-x;
			   float fHeight =  y2-y;

			   Vertices[a+0].TCoords.X = x+fWidth;
			   Vertices[a+0].TCoords.Y = y;

			   Vertices[a+1].TCoords.X = x;
			   Vertices[a+1].TCoords.Y = y;

			   Vertices[a+2].TCoords.X = x;
			   Vertices[a+2].TCoords.Y = y+fHeight;

			   Vertices[a+3].TCoords.X = x+fWidth;
			   Vertices[a+3].TCoords.Y = y+fHeight;
                   
			   Indices[q+0] = a+0;
			   Indices[q+1] = a+3;
			   Indices[q+2] = a+1;

			   Indices[q+3] = a+1;
			   Indices[q+4] = a+3;
			   Indices[q+5] = a+2;
			   a+=4;
			   q+=6;

			   quadCount++;
			}
	
		}
	}		

	
	if(quadCount>0)
	{
		
				

		driver->drawVertexPrimitiveList(
							Vertices,quadCount*4,
							Indices, quadCount*2,
							EVT_STANDARD, 
							EPT_TRIANGLES);
	}
	

}




void BatchedSpriteNode::setGameObjects(irr::core::array<GObjectPtr>* gameObjects)
{
	mGameObjects = gameObjects;	
}

