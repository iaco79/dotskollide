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
#include "Controller/TShieldController.h"
#include "scenenode/ShieldNode.h"


using namespace irr;



ShieldNode::ShieldNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int maxLines):
		Base2dNode(parent, mgr, id)	
{
	irr::video::SColor scolor(255,255,255,255);
	mMaterial.Wireframe = false;
	mMaterial.Lighting = false;
	mMaterial.AmbientColor = scolor;
	mMaterial.DiffuseColor = scolor;
	mMaterial.EmissiveColor  = scolor;
	mMaterial.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
	//mMaterial.setFlag(EMF_BILINEAR_FILTER,false);

	//material.Thickness=1.0f;
	
	

	mBox.reset(-5.0f,-5.0f,0.0f);				
	mBox.addInternalPoint(5.0f,5.0f,0.0f);

	//lets create de vertex buffers with maxLines

	mMaxQuads = maxLines;
	mVertexCount = mMaxQuads*4;
	mIndexCount = mMaxQuads*6;

	Vertices = new S3DVertex[mVertexCount];
	Indices = new u16[mIndexCount];
	
}


void ShieldNode::Load(char* filename,s32 frameWidth,s32 frameHeight)
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

	frame = 0;
                          
}

void ShieldNode::render()
{
	f32 rad90 = 
		90.0f * 3.1416f/180.0f;
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	
	//material.getTextureMatrix(0).setTextureScale(1.0f,1.0f);
//	mMaterial.getTextureMatrix(0).setTextureTranslate(tti,0.0f);
	


	driver->setMaterial(mMaterial);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	b2Vec2 p1, p2;
	
	b2Vec2 p11, p12,p21,p22;
	


	float lineWidth = 0.4f/1.5f;

	SColor clr = SColor(255,255,255,255);


	int quadCount=0;
	int a=0;
	int q=0;

	for(int j=0;j<mGameObjects->size();j++)
	{	
		if(quadCount<mMaxQuads)
		{

			GObjectPtr go = (*mGameObjects)[j];
			
			

			GControllerComponent* ccomp = go->GetComponent<GControllerComponent>(go->GetComponentIdByIndex(GC_CONTROLLER_IDX));
			TShieldController* shield = (TShieldController*)ccomp->gameController;

	
			if(shield)
			{
				int sframe =  0;

				
				if (shield->mState == SHIELD_STATE_INACTIVE)
				{
					sframe = shield->mPower+1;

				}

				
				p1 = shield->mShieldVertexes[0].mPosition;
				p2 = shield->mShieldVertexes[1].mPosition;

				
				//create geometry

				b2Vec2 nor = p2-p1;
				int lent = (int)nor.Length();
				nor.Normalize();

				b2Vec2 nor90 =  b2Mul(b2Rot(rad90*1.0f), nor)*lineWidth;
				b2Vec2 nor270 = b2Mul(b2Rot(rad90*3.0f), nor)*lineWidth;
				
				p11 = p2 + nor270;
				p12 = p2 + nor90;
				p21 = p1 + nor90;
				p22 = p1 + nor270;

				Vertices[a+0] = S3DVertex(p11.x,p11.y,0,0,0,0,clr,0,1);							
				Vertices[a+1] = S3DVertex(p12.x,p12.y,0,0,0,0,clr,1,1);
				Vertices[a+2] = S3DVertex(p21.x,p21.y,0,0,0,0,clr,1,0);
				Vertices[a+3] = S3DVertex(p22.x,p22.y,0,0,0,0,clr,0,0);
					
				
				float tx = (sframe  % stepww)*fWidth;
				float ty = (sframe  / stepww)*fHeight;

				Vertices[a+0].TCoords.X = tx+fWidth;
				Vertices[a+0].TCoords.Y = ty+fHeight;

				Vertices[a+1].TCoords.X = tx+fWidth;
				Vertices[a+1].TCoords.Y = ty;

				Vertices[a+2].TCoords.X = tx;
				Vertices[a+2].TCoords.Y = ty;

				Vertices[a+3].TCoords.X = tx;
				Vertices[a+3].TCoords.Y = ty+fHeight;
	                   
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


void ShieldNode::setGameObjects(irr::core::array<GObjectPtr>* gameObjects)
{
	mGameObjects = gameObjects;	
}
