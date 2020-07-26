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

static tBitMap *s_pLMC;
static tBitMap *s_pACE;

extern tState g_sStateMenu;
extern tStateManager *g_pStateMachineGame;

static tFont *s_pFont;
static tTextBitMap *s_pBmText;

static UWORD s_pPalette[32];

char szCredits[80];
const char *c1 = "ATARENIUM FALCON";
const char *c2 = "v 0.9 Decrunch 2020 gamedev compo release.";
const char *c3 = "";
const char *c4 = "Last Minute Creations are:";
const char *c5 = "KaiN";
const char *c6 = "Softiron";
const char *c7 = "Proxy";
const char *c8 = "Luc3k";
const char *c9 = ".";
const char *c10 = "ENJOY THE PARTY !";

static void doFadeInOut(void) {
  for (BYTE bRatio = 0; bRatio < 16; ++bRatio)
  {
    paletteDim(s_pPalette, s_pVp->pPalette, 32, bRatio); // 0 - czarno, 15 - pe?na paleta
    viewUpdateCLUT(s_pView);                              // we? palet? z viewporta i wrzu? j? na ekran
    for (int k = 0; k < 10; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }
  }

  for (int k = 0; k < 100; ++k)
  {
    vPortWaitForEnd(s_pVp);
  }
  // ?ciemnianie

  for (BYTE bRatio = 15 ; bRatio >= 0; --bRatio)
  {
    paletteDim(s_pPalette, s_pVp->pPalette, 32, bRatio); // 0 - czarno, 15 - pe?na paleta
    viewUpdateCLUT(s_pView);                              // we? palet? z viewporta i wrzu? j? na ekran
    for (int k = 0; k < 10; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }
  }
}

void stateCreditsCreate(void)
{

  s_pView = viewCreate(0,
                       TAG_VIEW_COPLIST_MODE, COPPER_MODE_BLOCK,
                       TAG_VIEW_GLOBAL_CLUT, 1,
                       TAG_END);

  s_pVp = vPortCreate(0,
                      TAG_VPORT_VIEW, s_pView,
                      TAG_VPORT_BPP, 5,
                      TAG_END);

  s_pVpManager = simpleBufferCreate(0,
                                    TAG_SIMPLEBUFFER_VPORT, s_pVp,
                                    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED,
                                    TAG_SIMPLEBUFFER_IS_DBLBUF, 0,
                                    TAG_END);

  joyOpen();
  keyCreate();
  viewLoad(s_pView);

  s_pFont = fontCreate("data/topaz.fnt");
  s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight);
  paletteLoad("data/lmcpalette.plt", s_pPalette, 32);
  s_pLMC = bitmapCreateFromFile("data/LMC.bm", 0);
  s_pACE = bitmapCreateFromFile("data/ACE.bm", 0);

  paletteDim(s_pPalette, s_pVp->pPalette, 32, 0); // 0 - czarno, 15 - peˆna paleta
  viewUpdateCLUT(s_pView);

  blitCopy(s_pLMC, 0, 0, s_pVpManager->pBack, 0, 0, 320, 128, MINTERM_COOKIE, 0xFF);
  blitCopy(s_pLMC, 0, 128, s_pVpManager->pBack, 0, 128, 320, 128, MINTERM_COOKIE, 0xFF);

  doFadeInOut();

  blitCopy(s_pACE, 0, 0, s_pVpManager->pBack, 0, 0, 320, 128, MINTERM_COOKIE, 0xFF);
  blitCopy(s_pACE, 0, 128, s_pVpManager->pBack, 0, 128, 320, 128, MINTERM_COOKIE, 0xFF);

  doFadeInOut();

  paletteLoad("data/falkon.plt", s_pPalette, 32);
  BYTE ubRatio = 15;
  paletteDim(s_pPalette, s_pVp->pPalette, 32, ubRatio); // 0 - czarno, 15 - peˆna paleta
  viewUpdateCLUT(s_pView);
  blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 22);
  blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 22);

  for (BYTE i = 0; i < 10; ++i)
  {

    switch (i)
    {
    case 0:
      sprintf(szCredits, c1);
      break;
    case 1:
      sprintf(szCredits, c2);
      break;
    case 2:
      sprintf(szCredits, c3);
      break;
    case 3:
      sprintf(szCredits, c4);
      break;
    case 4:
      sprintf(szCredits, c5);
      break;
    case 5:
      sprintf(szCredits, c6);
      break;
    case 6:
      sprintf(szCredits, c7);
      break;
    case 7:
      sprintf(szCredits, c8);
      break;
    case 8:
      sprintf(szCredits, c9);
      break;
    case 9:
      sprintf(szCredits, c10);
      break;
    }

    fontFillTextBitMap(s_pFont, s_pBmText, szCredits);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 1, i * 9, 5, FONT_COOKIE);
    for (BYTE k = 0; k < 10; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }
  }
}

void stateCreditsLoop(void)
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

void stateCreditsDestroy(void)
{

  joyClose();
  keyDestroy();
  viewDestroy(s_pView);
}

tState g_sStateCredits = {
    .cbCreate = stateCreditsCreate,
    .cbLoop = stateCreditsLoop,
    .cbDestroy = stateCreditsDestroy,
    .cbSuspend = 0,
    .cbResume = 0,
    .pPrev = 0};
