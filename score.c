#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/system.h>
#include <ace/managers/game.h>
#include <ace/utils/palette.h>
#include <stdio.h>
#include <ace/managers/state.h>
#include <ace/managers/blit.h>
#include <ace/utils/font.h>

static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;


extern tState g_sStateMenu;
extern tStateManager *g_pStateMachineGame;

extern BYTE coal;
extern BYTE capacitors;

static tFont *s_pFont;
static tTextBitMap *s_pBmText;


char szScore[80];

void stateScoreCreate(void){

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

systemUnuse();
joyOpen();
keyCreate();
viewLoad(s_pView);

s_pFont = fontCreate("data/uni54.fnt");
s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight);

blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 22);
blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 22);

for(BYTE i = 0 ; i < 13 ; ++i){

  switch (i){
    case 0:
    sprintf(szScore, "MISSION COMPLETE !");
    break;
    case 1:
    sprintf(szScore, "");
    break;
    case 2:
    sprintf(szScore, "The ATARI tribe can now plow their fields");
    break;
    case 3:
    sprintf(szScore, "with peace. Now go get some rest.");
    break;
    case 4:
    sprintf(szScore, "");
    break;
    case 5:
    sprintf(szScore, "You reclaimed %d of our coal.", coal);
    break;
    case 6:
    sprintf(szScore, "");
    break;
    case 7:
    sprintf(szScore, "%d Amigas died with acid leaking from", capacitors);
    break;
    case 8:
    sprintf(szScore, "their old capacitors.");
    break;
    case 9:
    sprintf(szScore, "");
    break;
    case 10:
    sprintf(szScore, "... But Sir, I fear all we have done");
    break;
    case 11:
    sprintf(szScore, "is to awaken a sleeping giant.");
    break;
    case 12:
    sprintf(szScore, "Amigans will be back in...");
    break; 
    
  }
    
      fontFillTextBitMap(s_pFont, s_pBmText, szScore);
      fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  1, i * 9, 5, FONT_COOKIE);
      for(BYTE k = 0 ; k < 10 ; ++k){
      vPortWaitForEnd(s_pVp);
      }
}

}


void stateScoreLoop(void){
	joyProcess();
	keyProcess();

	if(joyUse(JOY1_FIRE) || keyUse(KEY_RETURN)) {
		stateChange(g_pStateMachineGame, &g_sStateMenu);
		return;
  }

	viewProcessManagers(s_pView);
	copProcessBlocks();
	vPortWaitForEnd(s_pVp);
}

void stateScoreDestroy(void){
	systemUse();
	joyClose();
	keyDestroy();
	viewDestroy(s_pView);
	
}


tState g_sStateScore = {
  .cbCreate = stateScoreCreate,
  .cbLoop = stateScoreLoop,
  .cbDestroy = stateScoreDestroy,
  .cbSuspend = 0,
  .cbResume = 0,
  .pPrev = 0
};
