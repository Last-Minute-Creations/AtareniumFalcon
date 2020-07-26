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


extern tState g_sStateGame;
extern tStateManager *g_pStateMachineGame;

extern BYTE robboMsgNr;

static tFont *s_pFont;
static tTextBitMap *s_pBmText;

char szRobboMsg[80];
char *szRobbo1stLine = "ROBBO says:";
char *szRobboContinue = "(Fire or Return to continue.)";


void stateRobboCreate(void){
    systemUse();

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

viewLoad(s_pView);

s_pFont = fontCreate("data/topaz.fnt");
s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight);

blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 22);
blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 22);

  switch (robboMsgNr){
    case 0:
    sprintf(szRobboMsg, "Keep an eye on your coal supplies.");
    break;
    case 1:
    sprintf(szRobboMsg, "Follow the Atari portal.");
    break;
    case 2:
    sprintf(szRobboMsg, "Avoid the meteorites.");
    break;
    case 3:
    sprintf(szRobboMsg, "Try to steal some red and blue capacitors.");
    break;
    case 4:
    sprintf(szRobboMsg, "Infiltrate the Amigans territory.");
    break;
    case 5:
    sprintf(szRobboMsg, "Minister Renton is counting on you, Sir.");
    break;
    case 6:
    sprintf(szRobboMsg, "Hey, you need to clean up here, I found some GermZ ");
    break;
    case 7:
    sprintf(szRobboMsg, "Take me home, this place sucks!");
    break;
    case 8:
    sprintf(szRobboMsg, "You must reach the coal warehouse and reclaim it for us.");
    break;
    case 9:
    sprintf(szRobboMsg, "We're close, I feel it in my DSP.");
    break; 
    case 10:
    sprintf(szRobboMsg, "Well done! Now collect the coal and GTFO !!!");
    break; 
    
  }
        
      fontFillTextBitMap(s_pFont, s_pBmText, szRobbo1stLine);
      fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  1, 1, 5, FONT_COOKIE);
      
      for(BYTE k = 0 ; k < 10 ; ++k){
      vPortWaitForEnd(s_pVp);
      }
      
      fontFillTextBitMap(s_pFont, s_pBmText, szRobboMsg);
      fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  1, 10, 5, FONT_COOKIE);

      for(BYTE k = 0 ; k < 10 ; ++k){
      vPortWaitForEnd(s_pVp);
      }
      
      fontFillTextBitMap(s_pFont, s_pBmText, szRobboContinue);
      fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  1, 20, 5, FONT_COOKIE);
      
      


    
}


void stateRobboLoop(void){
	joyProcess();
	keyProcess();

	if(joyUse(JOY1_FIRE) || keyUse(KEY_RETURN)) {
		statePop(g_pStateMachineGame);
		return;
  }

	viewProcessManagers(s_pView);
	copProcessBlocks();
	vPortWaitForEnd(s_pVp);
}

void stateRobboDestroy(void){
	systemUse();
	
	viewDestroy(s_pView);
	systemUnuse();
}


tState g_sStateRobbo = {
  .cbCreate = stateRobboCreate,
  .cbLoop = stateRobboLoop,
  .cbDestroy = stateRobboDestroy,
  .cbSuspend = 0,
  .cbResume = 0,
  .pPrev = 0
};
