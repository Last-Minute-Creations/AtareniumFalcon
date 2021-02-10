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
static tBitMap *s_pVAM2;


extern tState g_sStateMenu;
extern tStateManager *g_pStateMachineGame;

void waitFrames();

extern BYTE startingCoal;

extern BYTE coal;
extern BYTE capacitors;
extern BYTE falkonx;
extern BYTE falkony;
extern BYTE krawedzx;
extern BYTE krawedzy;
extern BYTE kierunek;
extern BYTE falkonFace; // kierunek dziobem

extern UWORD uwPosY;

extern BYTE excesscoal;

extern BYTE stoneHit;
extern BYTE frameHit;

extern BYTE level;
extern BYTE robboMsgNr;
extern BYTE robboMsgCount;

extern BYTE amigaMode;


static tFont *s_pFont;
static tTextBitMap *s_pBmText;

BYTE vampire2 = 0;


char szScore2[80];

void clearTiles();

void vampirePage2(void){
  blitCopy(s_pVAM2, 0, 0, s_pVpManager->pBack, 0, 0, 320, 128, MINTERM_COOKIE);
  blitCopy(s_pVAM2, 0, 128, s_pVpManager->pBack, 0, 128, 320, 128, MINTERM_COOKIE);
}

void cleanUp2(void){
falkonx = 0;
falkony = 0;
krawedzx = 0;
krawedzy = 0;
kierunek = 0;
falkonFace = 0; 

stoneHit = 0;
frameHit = 0;
coal = startingCoal;
capacitors = 0;
level = 1;
robboMsgNr = 0;
robboMsgCount = 0;
excesscoal = 0;

amigaMode = 0;
}

void stateScoreAmiCreate(void){
  

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

s_pVAM2 = bitmapCreateFromFile("data/vampire.bm", 0);

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

s_pFont = fontCreate("data/topaz.fnt");
s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight);

blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 21);
blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 21);

for(BYTE i = 0 ; i < 17 ; ++i){

  switch (i){
    case 0:
    sprintf(szScore2, "MISSION COMPLETE !");
    break;
    case 1:
    sprintf(szScore2, " ");
    break;
    case 2:
    sprintf(szScore2, "The ATARI tribe got serious hit.");
    break;
    case 3:
    sprintf(szScore2, "Now go get some rest.");
    break;
    case 4:
    sprintf(szScore2, " ");
    break;
    case 5:
    sprintf(szScore2, "You intercepted %d tons of Atarimen's coal.", excesscoal);
    break;
    case 6:
    sprintf(szScore2, " ");
    break;
    case 7:
    sprintf(szScore2, "You saved %d capacitors for our Amigas. ", capacitors);
    break;
    case 8:
    sprintf(szScore2, " ");
    break;
    case 9:
    sprintf(szScore2, " ");
    break;
    case 10:
    sprintf(szScore2, "%d tons of coal x 100 = %d pts.", excesscoal, excesscoal * 100);
    break;
    case 11:
    sprintf(szScore2, "%d sets of capacitors x 500 = %d pts.", capacitors, capacitors * 500);
    break;
    case 12:
    sprintf(szScore2, "Total score = %d pts.", (excesscoal * 100) + (capacitors * 500));
    break;
    case 13:
    sprintf(szScore2, " ");
    break;
    case 14:
    sprintf(szScore2, "... and all they accomplished in that amateur");
    break;
    case 15:
    sprintf(szScore2, "attack is awakening of our sleeping giant.");
    break;
    case 16:
    sprintf(szScore2, "We, the Amigans will crush them in...");
    break; 
    }
    
      fontFillTextBitMap(s_pFont, s_pBmText, szScore2);
      fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  10, (i * 9) + 10, 23, FONT_COOKIE);
      waitFrames(s_pVp, 50, 0);
  }
logWrite("vp height: %hu\n", s_pVp->uwHeight);
}


void stateScoreAmiLoop(void){
  
	joyProcess();
	keyProcess();

  if (joyUse(JOY1_FIRE) || keyUse(KEY_RETURN))
  {
    if (vampire2 == 0)
    {
      ++vampire2;
      vampirePage2();
    }
    else if (vampire2 == 1){
    
		clearTiles();
    cleanUp2();
    stateChange(g_pStateMachineGame, &g_sStateMenu);
		return;
  }
    
  } 
	viewProcessManagers(s_pView);
	copProcessBlocks();
	//vPortWaitForEnd(s_pVp);
   vPortWaitForPos(s_pVp, uwPosY, 0);
}

void stateScoreAmiDestroy(void){
	systemUse();
	joyClose();
	keyDestroy();
	viewDestroy(s_pView);
    bitmapDestroy(s_pVAM2);
	
}


tState g_sStateScoreAmi = {
  .cbCreate = stateScoreAmiCreate,
  .cbLoop = stateScoreAmiLoop,
  .cbDestroy = stateScoreAmiDestroy,
  .cbSuspend = 0,
  .cbResume = 0,
  .pPrev = 0
};
