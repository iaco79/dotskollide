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

using namespace irr;

#define MAXFIXTURECOUNT 4

class B2dNode : public Base2dNode
{

private:

	video::S3DVertex* mVertices[MAXFIXTURECOUNT];
	u16* mIndices[MAXFIXTURECOUNT];	
	int mVertexCount[MAXFIXTURECOUNT];
	int mShapeType[MAXFIXTURECOUNT];
	int mFixtureCount;
	
	
	~B2dNode() // destructor
    {
		for(int i=0;i<mFixtureCount;i++)
		{
			delete[] mVertices[i];
			delete[] mIndices[i];
		}
 
    };

	void CreateGeometryFromFixture(b2Fixture* fixture,int fixtureIndex,video::SColor scolor);
	void CreateGeometryFromBody(b2Body* body,video::SColor scolor);

public:

	B2dNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,b2Body* body, video::SColor scolor);
	
	virtual void render();

	

};