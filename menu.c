#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/system.h>
#include <ace/managers/game.h>
#include <ace/utils/palette.h>
#include <ace/utils/font.h>
#include <stdio.h>
#include <ace/managers/state.h>

static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;

extern tState g_sStateGame;
extern tState g_sStateIntro;
extern tState g_sStateCredits;
extern tStateManager *g_pStateMachineGame;


void stateMenuCreate(void){
	s_pView = viewCreate(0,
		TAG_VIEW_COPLIST_MODE, COPPER_MODE_BLOCK,
		TAG_VIEW_GLOBAL_CLUT, 1,
		TAG_END
	);
	s_pVp = vPortCreate(0,
		TAG_VPORT_VIEW, s_pView,
		TAG_VPORT_BPP, 5,
		TAG_END
	);

	// Paleta z falkona
	paletteLoad("data/falkon.plt", s_pVp->pPalette, 32);

	s_pVpManager = simpleBufferCreate(0,
			TAG_SIMPLEBUFFER_VPORT, s_pVp,
			TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED,
			TAG_SIMPLEBUFFER_IS_DBLBUF, 0,
			TAG_END
	);


	bitmapLoadFromFile(s_pVpManager->pBack, "data/title.bm", 0, 0);
	joyOpen();
	keyCreate();
	systemUnuse();
	viewLoad(s_pView);
}

void stateMenuLoop(void){
	joyProcess();
	keyProcess();

	if(keyUse(KEY_ESCAPE)) {
		gameExit();
		return;
	}

	if(joyUse(JOY1_FIRE) || keyUse(KEY_RETURN)) {
		stateChange(g_pStateMachineGame, &g_sStateGame);
		return;
	}
	
	if(keyUse(KEY_I)) {
		stateChange(g_pStateMachineGame, &g_sStateIntro);
		return;
	}
	if(keyUse(KEY_C)) {
		stateChange(g_pStateMachineGame, &g_sStateCredits);
		return;
	}

	viewProcessManagers(s_pView);
	copProcessBlocks();
	vPortWaitForEnd(s_pVp);
}

void stateMenuDestroy(void){
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
  .pPrev = 0
};
