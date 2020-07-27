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


char szWungiel[50];

const char *w1 = "READY.";
char run[] = "RUN ATARENIUM FALCON";
const char *w3 = "";
const char *w4 = "";
const char *w5 = "";
const char *w6 = "";
const char *w7 = "";
const char *w8 = "";
const char *w9 = "";
const char *w10 = "";
const char *w11 = "";
const char *w12 = "";
const char *w13 = "";
const char *w14 = "";
const char *w15 = "";



void stateWungielCreate(void)
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

  systemUnuse();
  joyOpen();
  keyCreate();
  viewLoad(s_pView);

  s_pFont = fontCreate("data/topaz.fnt");
  s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight);

  blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 21);
  blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 21);

    sprintf(szWungiel, w1);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 10, 23, FONT_COOKIE);

    for (BYTE k = 0; k < 50; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }

    for(BYTE i = 0; i < 20 ; ++i){
     sprintf(szWungiel, run[i]);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 9) + 10, 20, 23, FONT_COOKIE);   
        for (BYTE k = 0; k < 15; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }
    }
  }
  


void stateWungielLoop(void)
{
  joyProcess();
  keyProcess();

  if (joyUse(JOY1_FIRE) || keyUse(KEY_RETURN))
  {
      stateChange(g_pStateMachineGame, &g_sStateMenu);
      return;
  }

  viewProcessManagers(s_pView);
  copProcessBlocks();
  vPortWaitForEnd(s_pVp);
}

void stateWungielDestroy(void)
{
  systemUse();
  joyClose();
  keyDestroy();
  viewDestroy(s_pView);
}

tState g_sStateWungiel = {
    .cbCreate = stateWungielCreate,
    .cbLoop = stateWungielLoop,
    .cbDestroy = stateWungielDestroy,
    .cbSuspend = 0,
    .cbResume = 0,
    .pPrev = 0};
