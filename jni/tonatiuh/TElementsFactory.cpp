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
#include "TonatiuhDefines.h"
#include "GGameApp.h"
#include "GObject.h"
#include "GObjectFactory.h"
#include "GController.h"
#include "GTransformComponent.h"
#include "GBox2dComponent.h"
#include "GControllerComponent.h"
#include "GSpriteComponent.h"
#include "GIrrlichtComponent.h"
#include "spline/spline.h"
#include "irrnode/Base2dNode.h"
#include "irrnode/B2dNode.h"
#include "irrnode/SpriteNode.h"
#include "irrnode/BatchedSpriteNode.h"
#include "springrid/SpringGrid.h"
#include "shaders/SpiralShader.h"
#include "shaders/FireworkShader.h"
#include "scenenode/SpringNode.h"
#include "scenenode/ShieldNode.h"
#include "SceneNode/FireworkNode.h"
#include "SceneNode/EnergyBarNode.h"
#include "SceneNode/SpiralNode.h"
#include "component/TSpringGridComponent.h"
#include "controller/TElementController.h"
#include "controller/TSynthetizerController.h"
#include "TGoalManager.h"
#include "Controller/TPlayerController.h"
#include "Controller/TShieldController.h"
#include "Controller/TFireWorkController.h"
#include "Controller/TEnergyBarController.h"
#include "Controller/TElementCreatorController.h"
#include "Controller/TBitController.h"
#include "TElementsFactory.h"



GObjectPtr TElementsFactory::CreateBitElement(GObjectPtr player, GObjectPtr source, GObjectPtr target, float value, irr::video::SColor color)
{
	GTransformComponent* stcomp = source->GetComponent<GTransformComponent>(source->GetComponentIdByIndex(GC_TRANSFORM_IDX));
	
	float x = stcomp->mTrans.p.x;
	float y = stcomp->mTrans.p.y;
	
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_BIT,0);
	

	b2Transform trans;
	trans.Set(b2Vec2(x,y),0.0f);

	
	
	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);

	
	
	//add the sprite comopnent  
	GSpriteComponent* scomp = new GSpriteComponent();
	scomp->SetOwner(object);
	scomp->VInit("", //the name of the sprite sheet (optional)
		irr::core::vector2di(164,288), //UV1 in pixels
		irr::core::vector2di(216,346), //UV2 in pixels
		irr::core::vector2df(0.3f,0.3f)); //size in world coordinates

	scomp->mColor = color;


	object->AddComponent(scomp);



	//add the controller component
	//add the controller
	TBitController* zcont = new TBitController();
	
	GControllerComponent* ccomp = new GControllerComponent();
	ccomp->VInit(zcont);
	object->AddComponent(ccomp);
	ccomp->SetOwner(object);
	zcont->VInit(player, target, value);
	object->PostInit(); //post initialize all components 

	return object;

}



GObjectPtr TElementsFactory::CreateObjective()
{	
	
	float x = 0.0f;
	float y = 0.0f;
	
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_OBJECTIVE,0);
	

	b2Transform trans;
	trans.Set(b2Vec2(x,y),0.0f);

	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);

	//add the sprite comopnent  
	GSpriteComponent* scomp = new GSpriteComponent();
	scomp->SetOwner(object);
	scomp->VInit("", //the name of the sprite sheet (optional)
		irr::core::vector2di(0,128), //UV1 in pixels
		irr::core::vector2di(0,128), //UV2 in pixels
		irr::core::vector2df(1.0f,1.0f)); //size in world coordinates
	scomp->mVisible=false;

	object->AddComponent(scomp);
	object->PostInit(); //post initialize all components 

	return object;

}

GObjectPtr TElementsFactory::CreateCreator()
{

	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_CREATOR,0);
	
	b2Transform trans;
	trans.Set(b2Vec2(0.0f,0.0f),0.0f);

	//create the scene node
	irr::video::SColor scolor = irr::video::SColor(255,255,255,255);
	
	
	SpriteNode* node = new SpriteNode(
				app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
				app->getIrrlichtDevice()->getSceneManager(),
				object->GetId());				
#if defined(_IRR_ANDROID_PLATEFORM_)
	node->Load("Figures.tga",128,128,1.0f,1.0f);
	
#else
	node->Load("IrrlichtSdcard/Figures.tga",128,128,1.0f,1.0f);
#endif
	node->setFrame(11);
	 


	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	tcomp->mZ = 0.05f;
	object->AddComponent(tcomp);
	
	
	GIrrlichtComponent* ncomp = new GIrrlichtComponent();
	ncomp->SetOwner(object);
	ncomp->VInit(node);
	object->AddComponent(ncomp);
	

	//add the controller component
	//add the controller
	TElementCreatorController* zcont = new TElementCreatorController();
	GControllerComponent* ccomp = new GControllerComponent();
	ccomp->VInit(zcont);
	object->AddComponent(ccomp);
	ccomp->SetOwner(object);
	zcont->VInit(); 
	 
	object->PostInit(); //post initialize all components 

	 
	return object;

}



//create points element (shield)
GObjectPtr TElementsFactory::CreateShieldElement(b2World* world,GObjectPtr synth,float initialSpeed,bool isStatic )
{
	GTransformComponent* tcomp = synth->GetComponent<GTransformComponent>(synth->GetComponentIdByIndex(GC_TRANSFORM_IDX));
	
	float x = tcomp->mTrans.p.x;
	float y = tcomp->mTrans.p.y;
	float angle = tcomp->mTrans.q.GetAngle();
	
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();

	GGameApp::getInstance()->setCurrentBox2dWorld(world); //<-- use this to provide cloning context
	
	GObjectPtr object = of->CreateObjectFromPrototype(T_PROT_ELEMENT_SHIELD);

	//if the object is null means that the prototype does not exist
	if(object == NULL)
	{        
			//create an empty object as prototype
			GObjectPtr protoype = NULL;
			protoype =  of->CreateEmptyObject(T_OT_ELEMENT,T_OS_ELEMENT_SHIELD,true); 	

			//set the box2d phyisics definition
			GBox2dComponent::FixtureDefinitionsList fixtures;

			b2Transform trans;
			trans.Set(b2Vec2(0.0f,0.0f),0.0f);

			//create the phyisics definition
			b2BodyDef bd;		
			
			bd.type =  b2_dynamicBody;
			bd.position = trans.p;
			bd.angle = trans.q.GetAngle();
			b2PolygonShape* shape = new  b2PolygonShape();
			
			b2Vec2 vertices[3];		
					

			vertices[0].Set( 0.0f*0.0f,  0.5f*0.5f);
			vertices[1].Set(-0.5f*0.5f, -0.5f*0.5f);
			vertices[2].Set( 0.5f*0.5f, -0.5f*0.5f);
			shape->Set(vertices,3);
		


			b2FixtureDef fd;		
			
			fd.shape = shape;
			fd.density = 1.0f;
			fd.friction = 0.02f;
			fd.restitution = 1.0f;
			fixtures.push_back(fd);

			//add the components to the object

			//add the transform component
			GTransformComponent* tcomp = new GTransformComponent();
			tcomp->SetOwner(protoype);
			tcomp->VInit(trans);
			protoype->AddComponent(tcomp);

			//add the box2d component
			GBox2dComponent* bcomp = new GBox2dComponent();
			bcomp->SetOwner(protoype);
			bcomp->VInit(world,bd,fixtures);
			protoype->AddComponent(bcomp);

			//add the controller
			TElementController* zcont = new TElementController();
			GControllerComponent* ccomp = new GControllerComponent();
			ccomp->VInit(zcont);
			protoype->AddComponent(ccomp);
			ccomp->SetOwner(protoype);
			zcont->VInit(T_OS_ELEMENT_SHIELD);

			//add the sprite comopnent  
			GSpriteComponent* scomp = new GSpriteComponent();
			scomp->SetOwner(protoype);
			scomp->VInit("", //the name of the sprite sheet (optional)
				irr::core::vector2di(256,0), //UV1 in pixels
				irr::core::vector2di(383,127), //UV2 in pixels
				irr::core::vector2df(0.9f,0.9f)); //size in world coordinates

			protoype->AddComponent(scomp);

			//add the prototype to the object factory
			of->SetPrototype(T_PROT_ELEMENT_SHIELD, protoype);

			//try to create the object again using the prototype
			object = of->CreateObjectFromPrototype(T_PROT_ELEMENT_SHIELD);

	}


	//initialize the cloned object 
	if(object)
	{
		float period = 2.0f + (float)(rand()%2);
		float rangle = period * 360.0f * (3.1416f /180.0f);



		//set the  initial transform
		GTransformComponent* tcomp  = object->GetComponent<GTransformComponent>(object->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		tcomp->mTrans.Set(b2Vec2(x ,y), angle);
		
		GBox2dComponent* bcomp  = object->GetComponent<GBox2dComponent>(object->GetComponentIdByIndex(GC_BOX2D_IDX));				
		if(isStatic)
			bcomp->mBodyDef.type = b2_staticBody;
		
		//perform a post init (this will instantiate the box2d body)
		object->PostInit();
		bcomp->mBody->SetActive(false);
		
		
		GControllerComponent* ccomp = object->GetComponent<GControllerComponent>(object->GetComponentIdByIndex(GC_CONTROLLER_IDX));		
		TElementController* econt = (TElementController*)ccomp->gameController;
		econt->setSynth(synth);
		econt->setInitialSpeed(initialSpeed);
		econt->mColor= irr::video::SColor(255,255,128,255);
		econt->mOriColor = econt->mColor;
	}
	
	return object;

}

//create energy element (CIRCLE)
GObjectPtr TElementsFactory::CreateEnergyElement(b2World* world,GObjectPtr synth,float px, float py,float initialSpeed, int color, bool isStatic)
{
	float x=px;
	float y=py;
	float angle =0.0f;

	if(synth)
	{
		GTransformComponent* tcomp = synth->GetComponent<GTransformComponent>(synth->GetComponentIdByIndex(GC_TRANSFORM_IDX));	
		x = tcomp->mTrans.p.x;
		y = tcomp->mTrans.p.y;
		angle = tcomp->mTrans.q.GetAngle();
	}


	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();

	GGameApp::getInstance()->setCurrentBox2dWorld(world); //<-- use this to provide cloning context
	
	GObjectPtr object = of->CreateObjectFromPrototype(T_PROT_ELEMENT_ENERGY);

	//if the object is null means that the prototype does not exist
	if(object == NULL)
	{        
			//create an empty object as prototype
			GObjectPtr protoype = NULL;
			protoype =  of->CreateEmptyObject(T_OT_ELEMENT,T_OS_ELEMENT_ENERGY,true); 	

			//set the box2d phyisics definition
			GBox2dComponent::FixtureDefinitionsList fixtures;

			b2Transform trans;
			trans.Set(b2Vec2(0.0f,0.0f),0.0f);

			//create the phyisics definition
			b2BodyDef bd;		
			bd.type =  b2_dynamicBody;
			bd.position = trans.p;
			bd.angle = trans.q.GetAngle();
			b2CircleShape* shape = new  b2CircleShape;
			shape->m_radius = 0.25f;		
			b2FixtureDef fd;		
			fd.shape = shape;
			fd.density = 1.0f;
			fd.friction = 0.8f;
			fd.restitution = 0.4f;
			fixtures.push_back(fd);

			//add the components to the object

			//add the transform component
			GTransformComponent* tcomp = new GTransformComponent();
			tcomp->SetOwner(protoype);
			tcomp->VInit(trans);
			protoype->AddComponent(tcomp);

			//add the box2d component
			GBox2dComponent* bcomp = new GBox2dComponent();
			bcomp->SetOwner(protoype);
			bcomp->VInit(world,bd,fixtures);
			protoype->AddComponent(bcomp);

			//add the controller
			TElementController* zcont = new TElementController();
			GControllerComponent* ccomp = new GControllerComponent();
			ccomp->VInit(zcont);
			protoype->AddComponent(ccomp);
			ccomp->SetOwner(protoype);
			zcont->VInit(T_OS_ELEMENT_ENERGY);

			//add the sprite comopnent  
			GSpriteComponent* scomp = new GSpriteComponent();
			scomp->SetOwner(protoype);
			scomp->VInit("", //the name of the sprite sheet (optional)
				irr::core::vector2di(256,256), //UV1 in pixels
				irr::core::vector2di(384,384), //UV2 in pixels
				irr::core::vector2df(1.3f,1.3f)); //size in world coordinates
			/*scomp->VInit("", //the name of the sprite sheet (optional)
				irr::core::vector2di(128,384), //UV1 in pixels
				irr::core::vector2di(256,512), //UV2 in pixels
				irr::core::vector2df(0.9f,0.9f)); //size in world coordinates
				*/
			protoype->AddComponent(scomp);

			//add the prototype to the object factory
			of->SetPrototype(T_PROT_ELEMENT_ENERGY, protoype);

			//try to create the object again using the prototype
			object = of->CreateObjectFromPrototype(T_PROT_ELEMENT_ENERGY);

	}


	//initialize the cloned object 
	if(object)
	{

		//set the  initial transform
		//set a random angle 
		GTransformComponent* tcomp  = object->GetComponent<GTransformComponent>(object->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		tcomp->mTrans.Set(b2Vec2(x,y), angle);
				
		
		GBox2dComponent* bcomp  = object->GetComponent<GBox2dComponent>(object->GetComponentIdByIndex(GC_BOX2D_IDX));				
		if(isStatic)
			bcomp->mBodyDef.type = b2_staticBody;
		//perform a post init (this will instantiate the box2d body)

		object->PostInit();
		bcomp->mBody->SetActive(false);
		
		GControllerComponent* ccomp = object->GetComponent<GControllerComponent>(object->GetComponentIdByIndex(GC_CONTROLLER_IDX));		
		TElementController* econt = (TElementController*)ccomp->gameController;
		econt->setSynth(synth);
		econt->mColorId = color;
		econt->setInitialSpeed(initialSpeed);


		//get the sprite component and set the correct color		
		econt->mColor= irr::video::SColor(255,255,0,0);
		switch(color)
		{
			case 1:
				{
					econt->mColor= irr::video::SColor(255,255,1,169);
					econt->mRadiationFactor = 3.0;

				}
			break;
			case 2:
				{
					econt->mRadiationFactor = 3.0f;
					econt->mColor = irr::video::SColor(255,139,8,232);
					
				}
			break;	
			case 3:
				{
					econt->mRadiationFactor = 3.0f;
					econt->mColor = irr::video::SColor(255,10,51,255);
				}
			break;	
			case 4:
				{
					econt->mRadiationFactor = 3.0f;
					econt->mColor = irr::video::SColor(255,23,196,232);
				}
			break;	
			default:
				{
					econt->mRadiationFactor = 3.0f;
					econt->mColor = irr::video::SColor(255,18,255,131);
				}
			break;
		}
		econt->mOriColor = econt->mColor;

		
	}
	
	return object;

}


//create energy element (CIRCLE)
GObjectPtr TElementsFactory::CreateEnergyHeavyElement(b2World* world,GObjectPtr synth,float initialSpeed, int color,bool isStatic )
{
	GTransformComponent* tcomp = synth->GetComponent<GTransformComponent>(synth->GetComponentIdByIndex(GC_TRANSFORM_IDX));
	
	float x = tcomp->mTrans.p.x;
	float y = tcomp->mTrans.p.y;
	float angle = tcomp->mTrans.q.GetAngle();


	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();

	GGameApp::getInstance()->setCurrentBox2dWorld(world); //<-- use this to provide cloning context
	
	GObjectPtr object = of->CreateObjectFromPrototype(T_PROT_ELEMENT_ENERGY2);

	//if the object is null means that the prototype does not exist
	if(object == NULL)
	{        
			//create an empty object as prototype
			GObjectPtr protoype = NULL;
			protoype =  of->CreateEmptyObject(T_OT_ELEMENT,T_OS_ELEMENT_ENERGY2,true); 	

			//set the box2d phyisics definition
			GBox2dComponent::FixtureDefinitionsList fixtures;

			b2Transform trans;
			trans.Set(b2Vec2(0.0f,0.0f),0.0f);

			//create the phyisics definition
			b2BodyDef bd;		
			bd.type =  b2_dynamicBody;
			bd.position = trans.p;
			bd.angle = trans.q.GetAngle();
			b2CircleShape* shape = new  b2CircleShape;
			shape->m_radius = 0.25f;		
			b2FixtureDef fd;		
			fd.shape = shape;
			fd.density = 3.0f; 
			fd.friction = 0.6f;
			fd.restitution = 0.7f;
			fixtures.push_back(fd);
 
			//add the components to the object

			//add the transform component
			GTransformComponent* tcomp = new GTransformComponent();
			tcomp->SetOwner(protoype);
			tcomp->VInit(trans);
			protoype->AddComponent(tcomp);

			//add the box2d component
			GBox2dComponent* bcomp = new GBox2dComponent();
			bcomp->SetOwner(protoype);
			bcomp->VInit(world,bd,fixtures);
			protoype->AddComponent(bcomp);

			//add the controller
			TElementController* zcont = new TElementController();
			GControllerComponent* ccomp = new GControllerComponent();
			ccomp->VInit(zcont);
			protoype->AddComponent(ccomp);
			ccomp->SetOwner(protoype);
			zcont->VInit(T_OS_ELEMENT_ENERGY2);

			//add the sprite comopnent  
			GSpriteComponent* scomp = new GSpriteComponent();
			scomp->SetOwner(protoype);
			scomp->VInit("", //the name of the sprite sheet (optional)
				irr::core::vector2di(128,384), //UV1 in pixels
				irr::core::vector2di(256,512), //UV2 in pixels
				irr::core::vector2df(0.9f,0.9f)); //size in world coordinates

			protoype->AddComponent(scomp);

			//add the prototype to the object factory
			of->SetPrototype(T_PROT_ELEMENT_ENERGY2, protoype);

			//try to create the object again using the prototype
			object = of->CreateObjectFromPrototype(T_PROT_ELEMENT_ENERGY2);

	}


	//initialize the cloned object 
	if(object)
	{

		//set the  initial transform
		//set a random angle 
		GTransformComponent* tcomp  = object->GetComponent<GTransformComponent>(object->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		tcomp->mTrans.Set(b2Vec2(x,y), angle);
				
		

		//perform a post init (this will instantiate the box2d body)
		GBox2dComponent* bcomp  = object->GetComponent<GBox2dComponent>(object->GetComponentIdByIndex(GC_BOX2D_IDX));				
		if(isStatic)
			bcomp->mBodyDef.type = b2_staticBody;
		object->PostInit();
		//bcomp->mBody->SetActive(false);

		GControllerComponent* ccomp = object->GetComponent<GControllerComponent>(object->GetComponentIdByIndex(GC_CONTROLLER_IDX));		
		TElementController* econt = (TElementController*)ccomp->gameController;
		econt->setSynth(synth);
		econt->mColorId = color;
		econt->setInitialSpeed(initialSpeed);


		//get the sprite component and set the correct color		
		switch(color)
		{
			case 1:
				{
					econt->mColor= irr::video::SColor(255,255,255,0);
					econt->mRadiationFactor = 3.0f;

				}
			break;
			case 2:
				{
					econt->mRadiationFactor = 6.0f;
					econt->mColor = irr::video::SColor(255,128,255,255);
				}
			break;	
			default:
				{
					econt->mRadiationFactor = 1.5f;
					econt->mColor = irr::video::SColor(255,0,255,0);		
				}
			break;
		}

	}
	
	return object;

}




//create points element (SQUARE)
GObjectPtr TElementsFactory::CreatePointsElement(b2World* world,GObjectPtr synth,float initialSpeed,bool isStatic )
{
	GTransformComponent* tcomp = synth->GetComponent<GTransformComponent>(synth->GetComponentIdByIndex(GC_TRANSFORM_IDX));
	
	float x = tcomp->mTrans.p.x;
	float y = tcomp->mTrans.p.y;
	float angle = tcomp->mTrans.q.GetAngle();
   	
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();

	GGameApp::getInstance()->setCurrentBox2dWorld(world); //<-- use this to provide cloning context
	
	GObjectPtr object = of->CreateObjectFromPrototype(T_PROT_ELEMENT_POINTS);

	//if the object is null means that the prototype does not exist
	if(object == NULL)
	{        
			//create an empty object as prototype
			GObjectPtr protoype = NULL;
			protoype =  of->CreateEmptyObject(T_OT_ELEMENT,T_OS_ELEMENT_POINTS,true); 	

			//set the box2d phyisics definition
			GBox2dComponent::FixtureDefinitionsList fixtures;

			b2Transform trans;
			trans.Set(b2Vec2(0.0f,0.0f),0.0f);

			//create the phyisics definition
			b2BodyDef bd;		
			bd.type =  b2_dynamicBody;
			bd.position = trans.p;
			bd.angle = trans.q.GetAngle();
			b2PolygonShape* shape = new  b2PolygonShape();
			shape->SetAsBox(0.25f,0.25f);

			b2FixtureDef fd;		
			fd.shape = shape;
			fd.density = 1.0f;
			fd.friction = 0.02f;
			fd.restitution = 0.7f;
			fixtures.push_back(fd);

			//add the components to the object

			//add the transform component
			GTransformComponent* tcomp = new GTransformComponent();
			tcomp->SetOwner(protoype);
			tcomp->VInit(trans);
			protoype->AddComponent(tcomp);

			//add the box2d component
			GBox2dComponent* bcomp = new GBox2dComponent();
			bcomp->SetOwner(protoype);
			bcomp->VInit(world,bd,fixtures);
			protoype->AddComponent(bcomp);

			//add the controller
			TElementController* zcont = new TElementController();
			GControllerComponent* ccomp = new GControllerComponent();
			ccomp->VInit(zcont);
			protoype->AddComponent(ccomp);
			ccomp->SetOwner(protoype);
			zcont->VInit(T_OS_ELEMENT_POINTS);

			//add the sprite comopnent  
			GSpriteComponent* scomp = new GSpriteComponent();
			scomp->SetOwner(protoype);
			scomp->VInit("", //the name of the sprite sheet (optional)
				irr::core::vector2di(0,128), //UV1 in pixels
				irr::core::vector2di(127,255), //UV2 in pixels
				irr::core::vector2df(1.0,1.0f)); //size in world coordinates

			protoype->AddComponent(scomp);

			//add the prototype to the object factory
			of->SetPrototype(T_PROT_ELEMENT_POINTS, protoype);

			//try to create the object again using the prototype
			object = of->CreateObjectFromPrototype(T_PROT_ELEMENT_POINTS);

	}


	//initialize the cloned object 
	if(object)
	{
		float period = 2.0f + (float)(rand()%2);
		float rangle = period * 360.0f * (3.1416f /180.0f);



		//set the  initial transform
		GTransformComponent* tcomp  = object->GetComponent<GTransformComponent>(object->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		tcomp->mTrans.Set(b2Vec2(x ,y), angle);
				
		
		//perform a post init (this will instantiate the box2d body)
		
		GBox2dComponent* bcomp  = object->GetComponent<GBox2dComponent>(object->GetComponentIdByIndex(GC_BOX2D_IDX));				
		if(isStatic)
			bcomp->mBodyDef.type = b2_staticBody;
		
		object->PostInit();
		bcomp->mBody->SetActive(false);
		
		GControllerComponent* ccomp = object->GetComponent<GControllerComponent>(object->GetComponentIdByIndex(GC_CONTROLLER_IDX));		
		TElementController* econt = (TElementController*)ccomp->gameController;
		econt->setSynth(synth);
		econt->setInitialSpeed(initialSpeed);

	}
	
	return object;

}


//create energy element (CIRCLE)
GObjectPtr TElementsFactory::CreateCrossElement(b2World* world,GObjectPtr synth,float initialSpeed, bool isStatic)
{
	GTransformComponent* tcomp = synth->GetComponent<GTransformComponent>(synth->GetComponentIdByIndex(GC_TRANSFORM_IDX));
	
	float x = tcomp->mTrans.p.x;
	float y = tcomp->mTrans.p.y;
	float angle = tcomp->mTrans.q.GetAngle();
	
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();

	GGameApp::getInstance()->setCurrentBox2dWorld(world); //<-- use this to provide cloning context
	
	GObjectPtr object = of->CreateObjectFromPrototype(T_PROT_ELEMENT_CROSS);

	//if the object is null means that the prototype does not exist
	if(object == NULL)
	{        
			//create an empty object as prototype
			GObjectPtr protoype = NULL;
			protoype =  of->CreateEmptyObject(T_OT_ELEMENT,T_OS_ELEMENT_BAD1,true); 	

			//set the box2d phyisics definition
			GBox2dComponent::FixtureDefinitionsList fixtures;

			b2Transform trans;
			trans.Set(b2Vec2(0.0f,0.0f),0.0f);

			//create the phyisics definition
			b2BodyDef bd;		
			
			bd.type =  b2_dynamicBody;
			bd.position = trans.p;
			bd.angle = trans.q.GetAngle();
			b2PolygonShape* shape = new  b2PolygonShape();
			b2PolygonShape* shape2 = new  b2PolygonShape();
			const float fr = 0.7f;
			const float fw = 0.2f;
			b2Vec2 vertices[4];
			vertices[0].Set(-fw*fr, 1.0f*fr);
			vertices[1].Set(-fw*fr, -1.0f*fr);
			vertices[2].Set(fw*fr, -1.0f*fr);
			vertices[3].Set(fw*fr, 1.0f*fr);

			b2Vec2 vertices2[4];
			vertices2[0].Set(-1.0f*fr, fw*fr);
			vertices2[1].Set(-1.0f*fr, -fw*fr);
			vertices2[2].Set(1.0f*fr, -fw*fr);
			vertices2[3].Set(1.0f*fr, fw*fr);
	

			
			b2FixtureDef fd;					
			b2FixtureDef fd2;		

			shape->Set(vertices,4);
			fd.shape = shape;
			fd.density = 1.0f;
			fd.friction = 0.02f;
			fd.restitution = 1.0f;
			fixtures.push_back(fd);

			shape2->Set(vertices2,4);
			fd2.shape = shape2;
			fd2.density = 1.0f;
			fd2.friction = 0.02f;
			fd2.restitution = 1.0f;
			

			
			fixtures.push_back(fd2);


			//add the components to the object

			//add the transform component
			GTransformComponent* tcomp = new GTransformComponent();
			tcomp->SetOwner(protoype);
			tcomp->VInit(trans);
			protoype->AddComponent(tcomp);

			//add the box2d component
			GBox2dComponent* bcomp = new GBox2dComponent();
			bcomp->SetOwner(protoype);
			bcomp->VInit(world,bd,fixtures);
			protoype->AddComponent(bcomp);

			//add the controller
			TElementController* zcont = new TElementController();
			GControllerComponent* ccomp = new GControllerComponent();
			ccomp->VInit(zcont);
			protoype->AddComponent(ccomp);
			ccomp->SetOwner(protoype);
			zcont->VInit(T_OS_ELEMENT_BAD1);

			//add the sprite comopnent  
			GSpriteComponent* scomp = new GSpriteComponent();
			scomp->SetOwner(protoype);
			scomp->VInit("", //the name of the sprite sheet (optional)
				irr::core::vector2di(256,128), //UV1 in pixels
				irr::core::vector2di(383,255), //UV2 in pixels
				irr::core::vector2df(1.8f,1.8f)); //size in world coordinates

			protoype->AddComponent(scomp);

			//add the prototype to the object factory
			of->SetPrototype(T_PROT_ELEMENT_CROSS, protoype);

			//try to create the object again using the prototype
			object = of->CreateObjectFromPrototype(T_PROT_ELEMENT_CROSS);

	}


	//initialize the cloned object 
	if(object)
	{
		float period = (float)(rand()%2);
		float rangle = period * 360.0f * (3.1416f /180.0f);



		//set the  initial transform
		GTransformComponent* tcomp  = object->GetComponent<GTransformComponent>(object->GetComponentIdByIndex(GC_TRANSFORM_IDX));
		tcomp->mTrans.Set(b2Vec2(x ,y), angle);
		
		GBox2dComponent* bcomp  = object->GetComponent<GBox2dComponent>(object->GetComponentIdByIndex(GC_BOX2D_IDX));				
		if(isStatic)
			bcomp->mBodyDef.type = b2_staticBody;
		
		//perform a post init (this will instantiate the box2d body)
		object->PostInit();
		bcomp->mBody->SetActive(false);
		
		
		GControllerComponent* ccomp = object->GetComponent<GControllerComponent>(object->GetComponentIdByIndex(GC_CONTROLLER_IDX));		
		TElementController* econt = (TElementController*)ccomp->gameController;
		econt->setSynth(synth);
		econt->setInitialSpeed(initialSpeed);
		econt->mColor= irr::video::SColor(255,255,64,64);

	}
	
	return object;

}


GObjectPtr TElementsFactory::CreateBasicBatchedSprite(float x, float y, float width, float height, b2Vec2 uv1,b2Vec2 uv2, float rotation)
{
	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_CREATOR,0);
	
	b2Transform trans;
	trans.Set(b2Vec2(x,y),rotation);



	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	tcomp->mZ = 0.0f;
	object->AddComponent(tcomp);
	
	
	//add the sprite comopnent  
	GSpriteComponent* scomp = new GSpriteComponent();
	scomp->SetOwner(object);
	scomp->VInit("", 
		irr::core::vector2di(uv1.x,uv1.y),
		irr::core::vector2di(uv2.x,uv2.y),
		irr::core::vector2df(width,height)); //size in world coordinates	
	scomp->mColor.set(255,255,255,255);

	object->AddComponent(scomp);

	object->PostInit(); //post initialize all components 
 
	return object;

}

//Creates an object
//with transform component
//+ spritenode
//using the figures sprite sheet
GObjectPtr TElementsFactory::CreateBasicSprite(float x, float y, float width, float height, int frame)
{
	//create an empty object
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_CREATOR,0);
	
	b2Transform trans;
	trans.Set(b2Vec2(x,y),0.0f);

	//create the scene node
	irr::video::SColor scolor = irr::video::SColor(255,255,255,255);
	SpriteNode* node = new SpriteNode(
				app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
				app->getIrrlichtDevice()->getSceneManager(),
				object->GetId());				
#if defined(_IRR_ANDROID_PLATEFORM_)
	node->Load("Figures.tga",128,128,width,height);
	
#else
	node->Load("IrrlichtSdcard/Figures.tga",128,128,width,height);
#endif
	node->setFrame(frame);


	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	tcomp->mZ = 0.05f;
	object->AddComponent(tcomp);
	
	
	GIrrlichtComponent* ncomp = new GIrrlichtComponent();
	ncomp->SetOwner(object);
	ncomp->VInit(node);
	object->AddComponent(ncomp);
	
	object->PostInit(); //post initialize all components 

	 
	return object;

}


GObjectPtr TElementsFactory::CreateColorBar(irr::video::SColor color, float x, float y, float width, float height, int colorId)
{
	GGameApp* app = GGameApp::getInstance();
	GObjectFactory* of= app->getObjectFactory();
	GObjectPtr object =  of->CreateEmptyObject(T_OT_BAR,colorId);
	
	b2Transform trans;
	trans.Set(b2Vec2(x,y),90.0*3.1416/180.0);
	irr::video::SColor colorback;
	irr::video::SColor colorfrom;

	colorback.setRed(color.getRed()/4);
	colorback.setGreen(color.getGreen()/4);
	colorback.setBlue(color.getBlue()/4);

	colorfrom.setRed(color.getRed()/2);
	colorfrom.setGreen(color.getGreen()/2);
	colorfrom.setBlue(color.getBlue()/2);

	//create the scene node
	//create the energy Bar
	
	EnergyBarNode* energyNode = new 
		EnergyBarNode(app->getIrrlichtDevice()->getSceneManager()->getRootSceneNode(),				
					  app->getIrrlichtDevice()->getSceneManager(),
					  object->GetId(),
					width,height,
					colorfrom, 
					color,
					color,0);

	#if defined(_IRR_ANDROID_PLATEFORM_)
	energyNode->Load("Figures.tga",128,128);
	#else
	energyNode->Load("IrrlichtSdcard/Figures.tga",128,128);
	#endif
	

	GIrrlichtComponent* ncomp = new GIrrlichtComponent();
	ncomp->SetOwner(object);
	ncomp->VInit(energyNode);
	object->AddComponent(ncomp);
	

	
	//add the components to the object
	GTransformComponent* tcomp = new GTransformComponent();
	tcomp->SetOwner(object);
	tcomp->VInit(trans);
	object->AddComponent(tcomp);
	
	object->PostInit(); //post initialize all components 

	return object;


}



