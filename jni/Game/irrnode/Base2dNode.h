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
using namespace irr::core;
using namespace	irr::video;
using namespace	irr::scene;

class Base2dNode : public ISceneNode
      {
         protected:               
               
			   bool	mIsTransparent;               			   
			   bool mCanReloadTexture;

			   core::aabbox3d<f32> mBox;
			   video::SMaterial    mMaterial;
			   video::ITexture*    mTexture;
			   irr::core::stringc mTextureName;

          public:
              Base2dNode(ISceneNode* parent, ISceneManager* mgr, s32 id): ISceneNode(parent, mgr, id)
               {     
				   irr::video::SColor scolor(255,255,255,255);	
                      mMaterial.Wireframe = false;
                      mMaterial.Lighting = false;
                      mIsTransparent = true;
					  mMaterial.AmbientColor = scolor;
					  mMaterial.DiffuseColor = scolor;
					  mMaterial.EmissiveColor  = scolor;
					  mCanReloadTexture=false;

               }     
             
			  void setTexture0(IVideoDriver* driver, char* filename);
			  void reloadTexture0(IVideoDriver* driver);



			  void setTransparent(bool value)
			  {
					mIsTransparent = value;
			  }

			  virtual void OnRegisterSceneNode()
			  {
				if (IsVisible)    
				{
					if(mIsTransparent)					
						SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
					else
						SceneManager->registerNodeForRendering(this, ESNRP_SOLID);
				}

				ISceneNode::OnRegisterSceneNode();

			  }

              virtual void render() = 0;
              
              virtual const aabbox3d<f32>& getBoundingBox() const
               {
                  return mBox;
               }
     
			  virtual u32 getMaterialCount() const
              {
                  return 1;
              }
     		  
              virtual SMaterial& getMaterial(u32 i)
			  {
                 return  mMaterial;
              }
  
      };
