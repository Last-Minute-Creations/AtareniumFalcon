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

static tFont *s_pFont;
static tTextBitMap *s_pBmText;

char szIntro[80];
const char *line1 = "Captain Tramiel, our planet was attacked by trecherous";
const char *line2 = "Great Miner and his race of Amigans.";
const char *line3 = "Almost all coal supplies for our Atari are gone.";
const char *line4 = "Renton - Minister of Mining is reporting that natural coal";
const char *line5 = "deposits are almost depleted and most of our miners fell";
const char *line6 = "to a strange disease and must remain qurantined. There is";
const char *line7 = "nobody left to work in mines. If we run out of coal for";
const char *line8 = "our Ataris we face the apocalypse of punch out tickets.";
const char *line9 = "Scouting ROBBOts we sent out into space did not return.";
const char *line10 = "We need your help Captain Tramiel. You have to go to";
const char *line11 = "Amigans Space Sector (ASS), retrieve any surviving ROBBOts";
const char *line12 = "and reclaim our coal supplies. Beware of their secret weapon -";
const char *line13 = "6bit processing power, 32-color graphics and 4-channel sound.";
const char *line14 = "Don't let them foul you.";
const char *line15 = "We are giving you masterpiece of our space engineery.";
const char *line16 = "Our newest spacecraft named ATARENIUM FALCON. Fully automated.";
const char *line17 = "The core of the craft is Atari Falcon running on highly";
const char *line18 = "efficient eco pea-coal. It is also equipped with external";
const char *line19 = "automated arm. This is prototype experimental. Now GO!";
const char *line20 = "One more thing Captain... While you traverse enemy territory";
const char *line21 = "try to collect as many capacitors as you can so the Amigans";
const char *line22 = "can't replaceleaking ones in their Amigas. Let electrolyte";
const char *line23 = "eat out their PCBs. HAR! HAR! HAR!";

void stateIntroCreate(void){

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

for(BYTE i = 0 ; i < 23 ; ++i){

  switch (i){
    case 0:
    sprintf(szIntro, line1);
    break;
    case 1:
    sprintf(szIntro, line2);
    break;
    case 2:
    sprintf(szIntro, line3);
    break;
    case 3:
    sprintf(szIntro, line4);
    break;
    case 4:
    sprintf(szIntro, line5);
    break;
    case 5:
    sprintf(szIntro, line6);
    break;
    case 6:
    sprintf(szIntro, line7);
    break;
    case 7:
    sprintf(szIntro, line8);
    break;
    case 8:
    sprintf(szIntro, line9);
    break;
    case 9:
    sprintf(szIntro, line10);
    break;
    case 10:
    sprintf(szIntro, line11);
    break;
    case 11:
    sprintf(szIntro, line12);
    break;
    case 12:
    sprintf(szIntro, line13);
    break;
    case 13:
    sprintf(szIntro, line14);
    break;
    case 14:
    sprintf(szIntro, line15);
    break;
    case 15:
    sprintf(szIntro, line16);
    break;
    case 16:
    sprintf(szIntro, line17);
    break;
    case 17:
    sprintf(szIntro, line18);
    break;
    case 18:
    sprintf(szIntro, line19);
    break;
    case 19:
    sprintf(szIntro, line20);
    break;
    case 20:
    sprintf(szIntro, line21);
    break;
    case 21:
    sprintf(szIntro, line22);
    break;
    case 22:
    sprintf(szIntro, line23);
    break;     
  }
    
      fontFillTextBitMap(s_pFont, s_pBmText, szIntro);
      fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  1, i * 9, 5, FONT_COOKIE);
      for(BYTE k = 0 ; k < 10 ; ++k){
      vPortWaitForEnd(s_pVp);
      }
}

}


void stateIntroLoop(void){
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

void stateIntroDestroy(void){
	systemUse();
	joyClose();
	keyDestroy();
	viewDestroy(s_pView);
	
}


tState g_sStateIntro = {
  .cbCreate = stateIntroCreate,
  .cbLoop = stateIntroLoop,
  .cbDestroy = stateIntroDestroy,
  .cbSuspend = 0,
  .cbResume = 0,
  .pPrev = 0
};
