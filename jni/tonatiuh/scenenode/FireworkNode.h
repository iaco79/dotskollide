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
#ifndef TFIREWORKNODE_CLASS
#define TFIREWORKNODE_CLASS

using namespace irr;
using namespace irr::core;
using namespace	irr::video;
using namespace	irr::scene;

#if defined(_IRR_ANDROID_PLATEFORM_)
#define FIRE_MAXVERTEX 400
#define FIRE_MAXINDEX  600
#else
#define FIRE_MAXVERTEX 400
#define FIRE_MAXINDEX  600
#endif


class FireworkNode : public Base2dNode
{
protected:
	irr::f32 mElapsedTime;
	irr::video::SColor mFireColor;

	static irr::u32 mVertexVBO;
	static irr::u32 mIndexVBO;
	static irr::u32 mVBOInitialed;

public:

	FireworkNode(ISceneNode* parent, ISceneManager* mgr, s32 id);			  
	static void InitialzeVBO(float fWidth, float fHeight,ISceneManager* mgr);
	virtual void render();
	virtual void Load(char* filename,s32 frameWidth,s32 frameHeight, float width, float height);

	void SetFireColor ( irr::video::SColor fireColor)
	{
		mFireColor = fireColor;
	}

	void SetElapsedTime(irr::f32 elapsedTime);

              
};
#endif