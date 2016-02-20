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
#ifndef TENERGYNODE_CLASS
#define TENERGYNODE_CLASS



using namespace irr;

class EnergyBarNode: public Base2dNode
{
private:


	video::S3DVertex vertices[12];
	u16 indices[18];

	float mEnergyValue;
	bool mEnergyNeedsUpdate;
	float mWidth;
	float mHeight;
	float mXFactor;
	float mYFactor;
	float mXFactor2;
	float mYFactor2;

	irr::core::dimension2df frameUV1;
	irr::core::dimension2df frameUV2;
	irr::core::dimension2df barUV1;
	irr::core::dimension2df barUV2;
	
	video::SColor mStartColor;
	video::SColor mEndColor;
	video::SColor mFrameColor;
	int frame;
	f32                 fWidth,fHeight;
    s32                 crntFrm,TotalFrm;
    s32                 stepww,stephh;
	s32 mType;
public:
EnergyBarNode(scene::ISceneNode* parent, 
			  scene::ISceneManager* mgr, 
			  s32 id, 
			  float width, 
			  float height,
			  video::SColor startColor,
			  video::SColor endColor,
			  video::SColor frameColor,
			  s32 type
			  );
	  
virtual void Load(char* filename,s32 frameWidth,s32 frameHeight);
    

void setEnergyValue(float value);
float getEnergyValue () {return mEnergyValue;}
virtual void render();

};

#endif