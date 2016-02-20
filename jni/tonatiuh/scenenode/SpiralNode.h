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
#ifndef SPIRALNODE_CLASS
#define SPIRALNODE_CLASS



using namespace irr;
using namespace irr::core;
using namespace	irr::video;
using namespace	irr::scene;

#if defined(_IRR_ANDROID_PLATEFORM_)
#define SPIRAL_MAXVERTEX 100
#define SPIRAL_MAXINDEX  150
#else
#define SPIRAL_MAXVERTEX 100
#define SPIRAL_MAXINDEX  150
#endif


class SpiralNode  : public Base2dNode
 {
         protected:
              
			   f32 mFactor;
			   irr::u32 mElapsedTime;
			   static u32 mVertexVBO;
			   static u32 mIndexVBO;
			   static irr::u32 mVBOInitialed;
			   
          public:
			 
			  SpiralNode(ISceneNode* parent, ISceneManager* mgr, s32 id,f32 factor);

			  
			  
			void setElapsedTime(irr::u32 elapsedTime ) 
				{
					mElapsedTime = elapsedTime;
				}
	
			irr::u32 getElapsedTime() 
				{
	
				  return mElapsedTime;
				}



			  void UpdateTime(u32 currentTime);			  
			  static void InitialzeVBO(float fWidth, float fHeight,ISceneManager* mgr);
              virtual void Load(char* filename,s32 frameWidth,s32 frameHeight, float width, float height);
              virtual void render();
			  
               
      };

#endif