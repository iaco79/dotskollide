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
//Node to draw several sprites at once

using namespace irr;
using namespace irr::core;
using namespace	irr::video;
using namespace	irr::scene;


class BatchedSpriteNode : public Base2dNode
{
private:

	int mJointCount;
	
	irr::video::S3DVertex*  Vertices;
	u16*		Indices;
    //s32                 stepww,stephh;
	//float fWidth, fHeight;
	int mVertexCount;
	int mIndexCount;
	int mMaxQuads;
	irr::core::array<GObjectPtr>* mGameObjects;

	~BatchedSpriteNode() // destructor
    {		
 
		delete[] Vertices;
		delete[] Indices;

    };

	
public:

	BatchedSpriteNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int maxObjects);
	

	void render();

	void setGameObjects(irr::core::array<GObjectPtr>* gameObjects);

	virtual void Load(char* filename,s32 frameWidth,s32 frameHeight)
    {
		IVideoDriver* driver = SceneManager->getVideoDriver();  


		setTexture0(driver, filename);

        		
/*       
		dimension2d<s32> size = (dimension2d<s32>)mTexture->getOriginalSize();
		/*fWidth  = (float)frameWidth/(float)size.Width;
		fHeight = (float)frameHeight/(float)size.Height;
		stepww = size.Width / frameWidth;
		stephh = size.Height / frameHeight;
		*/
                  
   }

	
};