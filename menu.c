#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/system.h>
#include <ace/managers/game.h>
#include <ace/utils/palette.h>
#include <ace/utils/font.h>
#include <stdio.h>
#include <ace/managers/state.h>
#include <ace/utils/chunky.h>
#include <ace/managers/blit.h>

static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;

#define STAR_COUNT 12

extern tState g_sStateGame;
extern tState g_sStateIntro;
extern tState g_sStateCredits;
extern tStateManager *g_pStateMachineGame;

UBYTE cheatmodeEnablerWhenEqual3 = 0;
UBYTE secondCheatEnablerWhenEqual3 = 0;
UBYTE cheatModeStarColor = 0;

UBYTE creditsControl = 0;

UBYTE pxColor[] = {1, 2, 3, 4, 5};
UBYTE bgColor = 0;

UBYTE starTick1 = 0;
UBYTE starCount1 = 0;
UBYTE starTick2 = 0;

UBYTE stars1TickTempo = 4;
UBYTE stars2TickTempo = 6;

UBYTE starCount2 = 0;
UBYTE starPrev1 = 0;
UBYTE starPrev2 = 0;

UWORD p1x[STAR_COUNT][5] = {
	// x dla pierwszej grupy gwiazdek

	{158, 153, 120, 80, 5},
	{154, 144, 110, 84, 33},
	{152, 146, 142, 138, 132},

	{162, 167, 200, 260, 315},
	{166, 178, 193, 216, 246},
	{170, 190, 228, 265, 310},

	{164, 176, 200, 260, 311},
	{160, 165, 170, 175, 180},
	{174, 190, 228, 265, 299},

	{158, 153, 120, 80, 5},
	{154, 144, 110, 84, 14},
	{152, 140, 128, 116, 100},
};
UWORD p1y[STAR_COUNT][5] = {
	// y dla pierwszej grupy gwiazdek
	{124, 122, 120, 118, 116},
	{120, 100, 80, 60, 40},
	{121, 105, 96, 42, 2},

	{124, 122, 120, 118, 116},
	{120, 100, 82, 62, 10},
	{118, 105, 96, 42, 2},

	{130, 158, 180, 204, 250},
	{140, 155, 172, 200, 246},
	{148, 146, 144, 142, 140},

	{130, 158, 180, 204, 245},
	{140, 141, 142, 144, 146},
	{148, 160, 185, 210, 240},
};

UWORD p2x[STAR_COUNT][5] = {
	// x dla drugiej grupy gwiazdek

	{156, 125, 100, 64, 12},
	{142, 122, 92, 52, 6},
	{138, 129, 121, 116, 110},

	{160, 175, 200, 260, 315},
	{162, 178, 186, 210, 232},
	{164, 190, 228, 265, 310},

	{167, 177, 202, 263, 318},
	{165, 169, 180, 185, 194},
	{173, 188, 225, 261, 291},

	{155, 140, 120, 80, 17},
	{149, 115, 80, 57, 9},
	{158, 138, 122, 101, 78},
};
UWORD p2y[STAR_COUNT][5] = {
	// y dla drugiej grupy gwiazdek

	{120, 114, 110, 106, 100},
	{127, 100, 70, 40, 10},
	{122, 101, 65, 33, 2},

	{118, 114, 110, 106, 100},
	{127, 100, 70, 40, 10},
	{122, 101, 65, 33, 2},

	{133, 157, 177, 201, 247},
	{145, 153, 171, 198, 241},
	{149, 147, 145, 144, 142},

	{135, 155, 180, 214, 250},
	{143, 153, 171, 198, 241},
	{146, 145, 143, 141, 140},
};

void pixel1(void)
{
	// STEP 1   ONLY FIRST GROUP
	if (starTick1 == stars1TickTempo && starCount1 == 0){
		for (UBYTE i = 0; i < STAR_COUNT ; ++i){
		chunkyToPlanar(pxColor[1 + cheatModeStarColor], p1x[i][0], p1y[i][0], s_pVpManager->pBack);
		}
		++starCount1;
	starTick1 = 0;
	}
	// STEP 2	ONLY 2ND GROUP
	if (starTick1 == stars1TickTempo && starCount1 == 1){
		for (UBYTE i = 0; i < STAR_COUNT ; ++i){
		chunkyToPlanar(bgColor, p1x[i][0], p1y[i][0], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[2 + cheatModeStarColor], p1x[i][1], p1y[i][1], s_pVpManager->pBack);
		}
		++starCount1;
	starTick1 = 0;
	}
	// STEP 3 GROUPS 1  3
	if (starTick1 == stars1TickTempo && starCount1 == 2){
		for (UBYTE i = 0; i < STAR_COUNT ; ++i){
		chunkyToPlanar(bgColor, p1x[i][1], p1y[i][1], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[1 + cheatModeStarColor], p1x[i][0], p1y[i][0], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[3 + cheatModeStarColor], p1x[i][2], p1y[i][2], s_pVpManager->pBack);

		}
		++starCount1;
	starTick1 = 0;
	} 

	// STEP 4 GROUPS 2 4
	if (starTick1 == stars1TickTempo && starCount1 == 3){
		for (UBYTE i = 0; i < STAR_COUNT ; ++i){
		chunkyToPlanar(bgColor, p1x[i][0], p1y[i][0], s_pVpManager->pBack);
		chunkyToPlanar(bgColor, p1x[i][2], p1y[i][2], s_pVpManager->pBack);
		chunkyToPlanar(bgColor, p1x[i][4], p1y[i][4], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[2 + cheatModeStarColor], p1x[i][1], p1y[i][1], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[4 + cheatModeStarColor], p1x[i][3], p1y[i][3], s_pVpManager->pBack);

		}
		++starCount1;
	starTick1 = 0;
	} 

	// STEP 5 GROUPS 1 3 5
	if (starTick1 == stars1TickTempo && starCount1 == 4){
		for (UBYTE i = 0; i < STAR_COUNT ; ++i){
		chunkyToPlanar(bgColor, p1x[i][1], p1y[i][1], s_pVpManager->pBack);
		chunkyToPlanar(bgColor, p1x[i][3], p1y[i][3], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[1 + cheatModeStarColor], p1x[i][0], p1y[i][0], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[3 + cheatModeStarColor], p1x[i][2], p1y[i][2], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[4 + cheatModeStarColor], p1x[i][4], p1y[i][4], s_pVpManager->pBack);
		}
		starCount1 = 3;
	starTick1 = 0;
	} 		
}

void pixel2(void)
{
	// STEP 1   ONLY FIRST GROUP
	if (starTick2 == stars2TickTempo && starCount2 == 0){
		for (UBYTE i = 0; i < STAR_COUNT ; ++i){
		chunkyToPlanar(pxColor[1 + cheatModeStarColor], p2x[i][0], p2y[i][0], s_pVpManager->pBack);
		}
		++starCount2;
	starTick2 = 0;
	}
	// STEP 2	ONLY 2ND GROUP
	if (starTick2 == stars2TickTempo && starCount2 == 1){
		for (UBYTE i = 0; i < STAR_COUNT ; ++i){
		chunkyToPlanar(bgColor, p2x[i][0], p2y[i][0], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[2 + cheatModeStarColor], p2x[i][1], p2y[i][1], s_pVpManager->pBack);
		}
		++starCount2;
	starTick2 = 0;
	}
	// STEP 3 GROUPS 1  3
	if (starTick2 == stars2TickTempo && starCount2 == 2){
		for (UBYTE i = 0; i < STAR_COUNT ; ++i){
		chunkyToPlanar(bgColor, p2x[i][1], p2y[i][1], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[1 + cheatModeStarColor], p2x[i][0], p2y[i][0], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[3 + cheatModeStarColor], p2x[i][2], p2y[i][2], s_pVpManager->pBack);

		}
		++starCount2;
	starTick2 = 0;
	} 

	// STEP 4 GROUPS 2 4
	if (starTick2 == stars2TickTempo && starCount2 == 3){
		for (UBYTE i = 0; i < STAR_COUNT ; ++i){
		chunkyToPlanar(bgColor, p2x[i][0], p2y[i][0], s_pVpManager->pBack);
		chunkyToPlanar(bgColor, p2x[i][2], p2y[i][2], s_pVpManager->pBack);
		chunkyToPlanar(bgColor, p2x[i][4], p2y[i][4], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[2 + cheatModeStarColor], p2x[i][1], p2y[i][1], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[4 + cheatModeStarColor], p2x[i][3], p2y[i][3], s_pVpManager->pBack);

		}
		++starCount2;
	starTick2 = 0;
	} 

	// STEP 5 GROUPS 1 3 5
	if (starTick2 == stars2TickTempo && starCount2 == 4){
		for (UBYTE i = 0; i < STAR_COUNT ; ++i){
		chunkyToPlanar(bgColor, p2x[i][1], p2y[i][1], s_pVpManager->pBack);
		chunkyToPlanar(bgColor, p2x[i][3], p2y[i][3], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[1 + cheatModeStarColor], p2x[i][0], p2y[i][0], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[3 + cheatModeStarColor], p2x[i][2], p2y[i][2], s_pVpManager->pBack);
		chunkyToPlanar(pxColor[4 + cheatModeStarColor], p2x[i][4], p2y[i][4], s_pVpManager->pBack);
		}
		starCount2 = 3;
	starTick2 = 0;
	} 
}



void stateMenuCreate(void)
{
	s_pView = viewCreate(0,
						 TAG_VIEW_COPLIST_MODE, COPPER_MODE_BLOCK,
						 TAG_VIEW_GLOBAL_CLUT, 1,
						 TAG_END);
	s_pVp = vPortCreate(0,
						TAG_VPORT_VIEW, s_pView,
						TAG_VPORT_BPP, 5,
						TAG_END);

	// Paleta z falkona
	paletteLoad("data/falkon.plt", s_pVp->pPalette, 32);

	s_pVpManager = simpleBufferCreate(0,
									  TAG_SIMPLEBUFFER_VPORT, s_pVp,
									  TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED,
									  TAG_SIMPLEBUFFER_IS_DBLBUF, 0,
									  TAG_END);

	blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 0);
    blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 0);

  // logo starting 80 32
  // options       80 112
  //  lmc          80 240
	
	bitmapLoadFromFile(s_pVpManager->pBack, "data/menulogo.bm", 80, 32); 
	bitmapLoadFromFile(s_pVpManager->pBack, "data/menuoptions.bm", 80, 112);
	bitmapLoadFromFile(s_pVpManager->pBack, "data/menulmc.bm", 80, 240);   
	
	
	joyOpen();
	keyCreate();
	systemUnuse();
	viewLoad(s_pView);

	creditsControl = 1;
}

void stateMenuLoop(void)
{
	joyProcess();
	keyProcess();
	++starTick1;
	++starTick2;
	//pixel1();
	//pixel2();

	if (keyUse(KEY_ESCAPE))
	{
		gameExit();
		return;
	}

	if (joyUse(JOY1_FIRE) || keyUse(KEY_RETURN))
	{
		stateChange(g_pStateMachineGame, &g_sStateGame);
		return;
	}

	if (keyUse(KEY_I))
	{
		stateChange(g_pStateMachineGame, &g_sStateIntro);
		return;
	}
	if (keyUse(KEY_C))
	{
		stateChange(g_pStateMachineGame, &g_sStateCredits);
		return;
	}

	if (keyUse(KEY_W) && cheatmodeEnablerWhenEqual3 == 0){
		++cheatmodeEnablerWhenEqual3;
	}
	if (keyUse(KEY_T) && cheatmodeEnablerWhenEqual3 == 1){
		++cheatmodeEnablerWhenEqual3;
	}
	if (keyUse(KEY_F) && cheatmodeEnablerWhenEqual3 == 2){
		++cheatmodeEnablerWhenEqual3;
		cheatModeStarColor += 6 ;
	}
	if (keyUse(KEY_L) && secondCheatEnablerWhenEqual3 == 0){
		++secondCheatEnablerWhenEqual3;
	}
	if (keyUse(KEY_S) && secondCheatEnablerWhenEqual3 == 1){
		++secondCheatEnablerWhenEqual3;
	}
	if (keyUse(KEY_A) && secondCheatEnablerWhenEqual3 == 2){
		++secondCheatEnablerWhenEqual3;
		cheatModeStarColor += 6;
	}


	viewProcessManagers(s_pView);
	copProcessBlocks();
	vPortWaitForEnd(s_pVp);
}

void stateMenuDestroy(void)
{
	systemUse();
	viewDestroy(s_pView);
	joyClose();
	keyDestroy();
}

tState g_sStateMenu = {
	.cbCreate = stateMenuCreate,
	.cbLoop = stateMenuLoop,
	.cbDestroy = stateMenuDestroy,
	.cbSuspend = 0,
	.cbResume = 0,
	.pPrev = 0};
