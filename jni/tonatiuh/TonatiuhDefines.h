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
#ifndef TONATIUH_DEFINES
#define TONATIUH_DEFINES
#include "gamedefines.h"


/*
#if defined(_IRR_ANDROID_PLATEFORM_)
#define T_SCREEN_WIDTH (1280)
#define T_SCREEN_HEIGHT (720)
#else
	#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

		#define T_SCREEN_WIDTH (800)
		#define T_SCREEN_HEIGHT (480)


	#endif
#endif
*/

enum GAME_PREFERENCES
{
	GAME_PREFERENCES_TIMESPLAYED=0,
	GAME_PREFERENCES_SFX,
	GAME_PREFERENCES_MUSIC,
	GAME_PREFERENCES_VIBRATE,
	GAME_PREFERENCES_RATED
};

enum GAME_ONFOCUS_PARAMS
{
	GAME_ONFOCUS_PARAMS_NONE=0,
	GAME_ONFOCUS_PARAMS_RESUME,
	GAME_ONFOCUS_PARAMS_TUTORIAL,
	GAME_ONFOCUS_PARAMS_LEVEL1,
	GAME_ONFOCUS_PARAMS_LEVEL2,
	GAME_ONFOCUS_PARAMS_LEVEL3

};


enum GAME_STATES
{
	GAME_STATE_NONE,
	GAME_STATE_INTRO,
	GAME_STATE_MAINMENU,
	GAME_STATE_LEVELSELECT,
	GAME_STATE_OPTIONS,
	GAME_STATE_EXTRAS,
	GAME_STATE_GAME
	
};

enum TONATIUH_PROTOTYPES
{
	T_PROT_ELEMENT_ENERGY=1,
	T_PROT_ELEMENT_ENERGY2=2,
	T_PROT_ELEMENT_POINTS=3,
	T_PROT_ELEMENT_SHIELD=4,	
	T_PROT_ELEMENT_BIT=5,
	T_PROT_ELEMENT_CROSS=6,
	T_PROT_ELEMENT_SIZE
};

enum TONATIUH_OBJECTYPES
{
	T_OT_PLAYER=1,
	T_OT_ELEMENT,
	T_OT_SYNTHETIZER,
	T_OT_SPRINGGRID,
	T_OT_SHIELD,
	T_OT_FIREWORK,
	T_OT_BAR,
	T_OT_BIT,
	T_OT_CREATOR,
	T_OT_SQUARESHIELD,
	T_OT_OBJECTIVE,
	T_OT_OTHER

};

enum TONATIUH_OBJECSUBTYPES
{
	T_OS_NONE=0,
	T_OS_ELEMENT_ENERGY,
	T_OS_ELEMENT_ENERGY2,
	T_OS_ELEMENT_POINTS,
	T_OS_ELEMENT_SHIELD,	
	T_OS_ELEMENT_BAD1,
	T_OS_ELEMENT_BAD2
};


enum TONATIUH_CONTROLLERS_IDX
{
	T_CTLR_ELEMENT_IDX=0,
	T_CTLR_SYNTH_IDX=1,
	T_CTLR_PLAYER_IDX=2,
	T_CTLR_SHIELD_IDX=3,
};

enum TONATIUH_COMPONENTS_IDX
{
	T_GC_SPRINGRID_IDX=GC_SIZE	
};

enum TUTORIAL_STATE
{
	TUTORIAL_STATE_NONE,
	TUTORIAL_STATE_START=1,
	TUTORIAL_STATE_RUNNING=2
};



enum CRASH_STATE
{
	CRASH_STATE_NONE=0,
	CRASH_STATE_CRASHED,
	CRASH_STATE_CANCRASH,
	CRASH_STATE_CANJOIN,	
	CRASH_STATE_CANCLOSE,
	CRASH_STATE_CANOPEN,	
	CRASH_STATE_CANSWAP,
	CRASH_STATE_INVALIDCRASH
		
};

enum GOAL_STATE
{
	GOAL_STATE_NONE=0,	
	GOAL_STATE_NOT_FOUND,
	GOAL_STATE_CRASHABLE,
	GOAL_STATE_SEARCHING
};

enum ELEMENT_CYLCE
{
	ELEMENT_CYCLE_CREATING=0,
	ELEMENT_CYCLE_CREATED,
	ELEMENT_CYLLE_DESTROYED
};

enum CREATOR_STATE
{   CREATOR_STATE_NONE,
	CREATOR_STATE_CREATING,
	CREATOR_STATE_CREATED,
	CREATOR_STATE_FINISHED	
};

enum SHIELD_STATE 
{
	SHIELD_STATE_CREATING=0,
	SHIELD_STATE_ACTIVE,	
	SHIELD_STATE_DAMAGED,	
	SHIELD_STATE_INACTIVE,
	SHIELD_STATE_FIXED
};
	


enum MOLE_STATE
{   MOLE_STATE_NONE=0,
	MOLE_STATE_COLLECTING,	
	MOLE_STATE_PROCESSING,
	MOLE_STATE_PROCESSED
};

enum BIT_TYPE
{
	BIT_ENERGY=0,
	BIT_SHIELD
};

enum BIT_STATE 
{
	BIT_STATE_ACTIVE=0,
	BIT_STATE_FINISHED
};
	
const float CRASH_AREA_HALF_WIDTH  = 5.0f;
const float CRASH_AREA_HALF_HEIGHT = 7.0f; 

/* SHIELD PATH

 LQ   L1a
   t0-----t1  L2
t7 /       \ t2
  |         |
  |         |  L1b
  |         |
t6 \       / t3
  t5 ----- t4  


*/
const float SHIELD_PATH_LENGTH1a = 11.00f;
const float SHIELD_PATH_LENGTH1b = 07.00f;

const float SHIELD_PATH_LENGTH2 = 2.3562f;
const float SHIELD_PATH_LENGTH2Q = 1.5f;
const float SHIELD_PATH_LENGTH2C = 2.0f* 3.1416f* SHIELD_PATH_LENGTH2Q;

const float SHIELD_PATH_LENGTHT = 2.0f * (SHIELD_PATH_LENGTH1a + SHIELD_PATH_LENGTH1b) + SHIELD_PATH_LENGTH2*4.0f;

const float SHIELD_SPEED = SHIELD_PATH_LENGTHT/64.0f; 

const float SHIELD_SIZE = 2.163f; //the size of each shield object 
const irr::u32 SHIELD_SIZE_TIME =(int) ((SHIELD_SIZE/SHIELD_SPEED)*1000.0f); 
const int SHIELD_MAX_OBJECTS = 14;

const irr::u32 SHIELD_PERIOD = (int) ((SHIELD_PATH_LENGTHT/SHIELD_SPEED)*1000.0f);

const int TRIPLE_CONTACT_TIMER = 1000;

#endif
