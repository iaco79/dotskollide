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
#ifndef SPRINGGRIDNODE_CLASS
#define SPRINGGRIDNODE_CLASS

using namespace irr;
using namespace irr::core;
using namespace	irr::video;
using namespace	irr::scene;

#define SPRING_MAXVERTEX 3364
#define SPRING_MAXINDEX  5046
#define SPRING_MAXCOLOR  841
#define SRPING_MAX_ALPHA (96.0f)

class SpringNode : public Base2dNode
{
    protected:
            
		SColorf  quadColors[SPRING_MAXCOLOR];
		SColor  quadColorsBase[SPRING_MAXCOLOR];
			   
		SColor VertexColor;
		springrid::SpringGrid*		  grid;
		S3DVertex Vertices[SPRING_MAXVERTEX];
		u16		Indices[SPRING_MAXVERTEX];
		f32                 fWidth,fHeight;               
        s32                 stepww,stephh;
		int frame;
		int rendercount;

		SmoothMotionInfo mSmoothPointsW;
		SmoothMotionInfo mSmoothPointsH;
               
    public:
		SpringNode(ISceneNode* parent, ISceneManager* mgr, s32 id,springrid::SpringGrid* lgrid): 
		Base2dNode(parent, mgr, id),
		frame(0)
        {     
                mMaterial.Wireframe = false;
                mMaterial.Lighting = false;                     
				//Material.ZBuffer = ECFN_NEVER;
				//Material.setFlag(video::EMF_BILINEAR_FILTER,false);

				grid = lgrid;
					  
        }
            

		void Load(char* filename,s32 frameWidth,s32 frameHeight, float width, float height);
              
		void SetVertexColor(SColor newColor)
		{	                              
			VertexColor=newColor;				  
		}
                       
		virtual void setFrame(int n)
        {
            frame = n;
        }

		virtual void render();
		void render1();

};

#endif