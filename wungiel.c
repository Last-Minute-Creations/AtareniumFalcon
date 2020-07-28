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
char load1[] = "LOAD ATAREN";
char load2[] = "UM FALCON";
const char *w2 = "ERROR - INSERT WINCY WUNGLA TO DF0:"; 
char dots[] = "...";
const char *w3 = "WUNGIEL DETECTED";
const char *w4 = "DONE.";
char run[] = "RUN";

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

  blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 22);
  blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 22);

    sprintf(szWungiel, w1);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 10, 23, FONT_COOKIE);

    for (BYTE k = 0; k < 50; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }

    for(BYTE i = 0; i < 12 ; ++i){
     sprintf(szWungiel, "%c", load1[i]);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 10, 20, 23, FONT_COOKIE);   
        for (BYTE k = 0; k < 15; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }
    }

    sprintf(szWungiel, "I");
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 88, 20, 23, FONT_COOKIE);
    for (BYTE k = 0; k < 10; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }

    for(BYTE i = 0; i < 10 ; ++i){
     sprintf(szWungiel, "%c", load2[i]);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 93, 20, 23, FONT_COOKIE);   
        for (BYTE k = 0; k < 15; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }
    }

    for (BYTE k = 0; k < 50; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }
    
    sprintf(szWungiel, w2);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 30, 23, FONT_COOKIE);

    for (BYTE k = 0; k < 50; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }

    for(BYTE i = 0; i < 3 ; ++i){
     sprintf(szWungiel, "%c", dots[i]);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 10, 40, 23, FONT_COOKIE);   
        for (BYTE k = 0; k < 50; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }
    }
    sprintf(szWungiel, w3);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 50, 23, FONT_COOKIE);

    for (BYTE k = 0; k < 50; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }

       for(BYTE i = 0; i < 12 ; ++i){
     sprintf(szWungiel, "%c", load1[i]);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 10, 60, 23, FONT_COOKIE);   
        for (BYTE k = 0; k < 15; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }
    }
    sprintf(szWungiel, "I");
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 88, 60, 23, FONT_COOKIE);
    for (BYTE k = 0; k < 10; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }

    for(BYTE i = 0; i < 10 ; ++i){
     sprintf(szWungiel, "%c", load2[i]);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 93, 60, 23, FONT_COOKIE);   
        for (BYTE k = 0; k < 15; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }
    }

    for(BYTE i = 0; i < 3 ; ++i){
     sprintf(szWungiel, "%c", dots[i]);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 10, 70, 23, FONT_COOKIE);   
        for (BYTE k = 0; k < 55; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }
    }

    sprintf(szWungiel, w4);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 80, 23, FONT_COOKIE);

    for (BYTE k = 0; k < 50; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }

    for(BYTE i = 0; i < 3 ; ++i){
     sprintf(szWungiel, "%c", run[i]);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 10, 90, 23, FONT_COOKIE);   
        for (BYTE k = 0; k < 15; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }
    }

    for(BYTE i = 0; i < 3 ; ++i){
     sprintf(szWungiel, "%c", dots[i]);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 10, 100, 23, FONT_COOKIE);   
        for (BYTE k = 0; k < 50; ++k)
        {
        vPortWaitForEnd(s_pVp);
        }
    }


  }
  


void stateWungielLoop(void)
{
 
      stateChange(g_pStateMachineGame, &g_sStateMenu);
      return;
  
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
