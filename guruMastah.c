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
static tBitMap *s_pJMiner;

extern tState g_sStateGame;
extern tStateManager *g_pStateMachineGame;

static UWORD s_pPalette[32];
static tFont *s_pFont;
static tTextBitMap *s_pBmText;

#define MINER_LINE_COUNT 4

const char *s_pLines2[MINER_LINE_COUNT] = {
    "GREAT MINERs FACE GONNA BE HERE",
    "AND SOME BULLSHIT ABOUT AMIGA SPIRIT",
    "IN YOU, SIR. AND FANCY GFX.",
    "ATARIs? KILLem ALL. (PRESS FIRE)"};

void waitFrames();

void stateGuruMastahCreate(void)
{
  systemUse();

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

  // s_pJMiner = bitmapCreateFromFile("data/jminersmall1.bm", 0);

  s_pVpManager = simpleBufferCreate(0,
                                    TAG_SIMPLEBUFFER_VPORT, s_pVp,
                                    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED,
                                    TAG_SIMPLEBUFFER_IS_DBLBUF, 0,
                                    TAG_END);

  s_pFont = fontCreate("data/topaz.fnt");
  s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight);

  systemUnuse();
  // bitmapLoadFromFile(s_pVpManager->pBack, "data/jminersmall1.bm", 0, 0);

  blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 21);
  blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 21);

  for (UBYTE i = 0; i < 4; ++i)
  {
    fontFillTextBitMap(s_pFont, s_pBmText, s_pLines2[i]);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, (i * 9) + 10, 23, FONT_COOKIE);
  }

  joyOpen();
  keyCreate();
  viewLoad(s_pView);
}

void stateGuruMastahLoop(void)
{
  joyProcess();
  keyProcess();

  if (joyUse(JOY1_FIRE) || keyUse(KEY_RETURN))
  {
    statePop(g_pStateMachineGame);
    return;
  }

  viewProcessManagers(s_pView);
  copProcessBlocks();
  waitFrames(s_pVp, 1, 200);
}

void stateGuruMastahDestroy(void)
{
  systemUse();
  joyClose();
  keyDestroy();
  viewDestroy(s_pView);
}

tState g_sStateGuruMastah = {
    .cbCreate = stateGuruMastahCreate,
    .cbLoop = stateGuruMastahLoop,
    .cbDestroy = stateGuruMastahDestroy,
    .cbSuspend = 0,
    .cbResume = 0,
    .pPrev = 0};
