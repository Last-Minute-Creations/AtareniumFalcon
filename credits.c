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
#include <ace/utils/ptplayer.h>

#define CREDITS_LINE_COUNT 13

enum StatesCredits
{
  STATE_LMC_FADE_IN,
  STATE_LMC_WAIT,
  STATE_LMC_FADE_OUT,
  STATE_ACE_FADE_IN,
  STATE_ACE_WAIT,
  STATE_ACE_FADE_OUT,
  STATE_CREDITS,
};

static enum StatesCredits s_eState;

static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;

static tBitMap *s_pLMC;
static tBitMap *s_pACE;

static tPtplayerSfx *s_pACEsfx;
static tPtplayerSfx *s_pLMCsfx;

extern tState g_sStateMenu;
extern tStateManager *g_pStateMachineGame;
extern tState g_sStateIntro;

static tFont *s_pFont;
static tTextBitMap *s_pBmText;

static UWORD s_pPalette[32];

extern UBYTE creditsControl;

UBYTE bRatio = 0;
UBYTE waitTime = 0;
UBYTE drawOnce = 0;

const char *s_pCreditsLines[CREDITS_LINE_COUNT] = {
    "ATARENIUM FALCON  v0.91 (Demo)",
    "Updated 30.12.2020, first release at",
    "Decrunch 2020 gamedev compo (2nd place).",
    "",
    "Luc3k: code & MSX",
    "Softiron: GFX",
    "KaiN & Proxy: AFK",
    "",
    "Special credit goes to Saberman - ",
    "The Great Atariman of the Galaxy.",
    "",
    "(Fire or Return to continue)"};

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

  systemUnuse();
  joyOpen();
  keyCreate();
  viewLoad(s_pView);
  ptplayerCreate(1);

  bRatio = 0;
  waitTime = 0;
  drawOnce = 0;

  s_eState = STATE_LMC_FADE_IN;

  s_pFont = fontCreate("data/topaz.fnt");
  s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight);
  paletteLoad("data/lmcpalette.plt", s_pPalette, 32);
  s_pLMC = bitmapCreateFromFile("data/LMC.bm", 0);
  s_pACE = bitmapCreateFromFile("data/ACE.bm", 0);

  s_pACEsfx = ptplayerSfxCreateFromFile("data/AceSample.sfx");
  s_pLMCsfx = ptplayerSfxCreateFromFile("data/Morse_LMC8000.sfx");
}

void stateCreditsLoop(void)
{
  joyProcess();
  keyProcess();

  switch (s_eState)
  {
  case STATE_LMC_FADE_IN:
    if (drawOnce == 0)
    {
      ++drawOnce;
      paletteDim(s_pPalette, s_pVp->pPalette, 32, 0); // 0 - czarno, 15 - pe�na paleta
      viewUpdateCLUT(s_pView);

      ptplayerSfxPlay(s_pLMCsfx, 3, 64, 100);
      blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 0);
      blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 0);
      blitCopy(s_pLMC, 0, 0, s_pVpManager->pBack, 104, 40, 112, 160, MINTERM_COOKIE);
    }

    paletteDim(s_pPalette, s_pVp->pPalette, 32, bRatio); // 0 - czarno, 15 - pe?na paleta
    viewUpdateCLUT(s_pView);                             // we? palet? z viewporta i wrzu? j? na ekran
    ++bRatio;
    if (bRatio == 15)
    {
      s_eState = STATE_LMC_WAIT;
    }
    break;

  case STATE_LMC_WAIT:
    ++waitTime;
    if (waitTime == 200)
    {
      waitTime = 0;
      s_eState = STATE_LMC_FADE_OUT;
    }
    break;

  case STATE_LMC_FADE_OUT:
    paletteDim(s_pPalette, s_pVp->pPalette, 32, bRatio); // 0 - czarno, 15 - pe?na paleta
    viewUpdateCLUT(s_pView);                             // we? palet? z viewporta i wrzu? j? na ekran
    --bRatio;
    if (bRatio == 0)
    {
      s_eState = STATE_ACE_FADE_IN;
    }
    break;

  case STATE_ACE_FADE_IN:
    if (drawOnce == 1)
    {
      ++drawOnce;
      paletteDim(s_pPalette, s_pVp->pPalette, 32, 0); // 0 - czarno, 15 - pe�na paleta
      viewUpdateCLUT(s_pView);

      ptplayerSfxPlay(s_pACEsfx, 3, 64, 100);
      blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 0);
      blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 0);
      blitCopy(s_pACE, 0, 0, s_pVpManager->pBack, 80, 95, 155, 70, MINTERM_COOKIE);
    }

    paletteDim(s_pPalette, s_pVp->pPalette, 32, bRatio); // 0 - czarno, 15 - pe?na paleta
    viewUpdateCLUT(s_pView);                             // we? palet? z viewporta i wrzu? j? na ekran
    ++bRatio;
    if (bRatio == 15)
    {
      s_eState = STATE_ACE_WAIT;
    }
    break;

  case STATE_ACE_WAIT:
    ++waitTime;
    if (waitTime == 200)
    {
      s_eState = STATE_ACE_FADE_OUT;
    }
    break;

  case STATE_ACE_FADE_OUT:
    paletteDim(s_pPalette, s_pVp->pPalette, 32, bRatio); // 0 - czarno, 15 - pe?na paleta
    viewUpdateCLUT(s_pView);                             // we? palet? z viewporta i wrzu? j? na ekran
    --bRatio;
    if (bRatio == 0)
    {
      s_eState = STATE_CREDITS;
    }
    break;

  case STATE_CREDITS:
    if (drawOnce == 2)
    {
      ++drawOnce;
      paletteLoad("data/falkon.plt", s_pPalette, 32);
      BYTE ubRatio = 15;
      paletteDim(s_pPalette, s_pVp->pPalette, 32, ubRatio); // 0 - czarno, 15 - pe�na paleta
      viewUpdateCLUT(s_pView);
      blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 21);
      blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 21);
    

    for (BYTE i = 0; i < CREDITS_LINE_COUNT; ++i)
    {
      fontFillTextBitMap(s_pFont, s_pBmText, s_pCreditsLines[i]);
      fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, (i * 9) + 10, 23, FONT_COOKIE);
    }
    }
  }

  if (joyUse(JOY1_FIRE) || keyUse(KEY_RETURN)){
    if (creditsControl == 0)
    {
      stateChange(g_pStateMachineGame, &g_sStateIntro);
      return;
    }
    else if (creditsControl == 1)
    {
      stateChange(g_pStateMachineGame, &g_sStateMenu);
      return;
    }
  }

  viewProcessManagers(s_pView);
  copProcessBlocks();
  vPortWaitForEnd(s_pVp);
}

void stateCreditsDestroy(void)
{
  systemUse();

  fontDestroy(s_pFont);
  fontDestroyTextBitMap(s_pBmText);
  bitmapDestroy(s_pLMC);
  bitmapDestroy(s_pACE);
  joyClose();
  keyDestroy();
  viewDestroy(s_pView);
  ptplayerSfxDestroy(s_pACEsfx);
  ptplayerSfxDestroy(s_pLMCsfx);

  ptplayerDestroy();
}

tState g_sStateCredits = {
    .cbCreate = stateCreditsCreate,
    .cbLoop = stateCreditsLoop,
    .cbDestroy = stateCreditsDestroy,
    .cbSuspend = 0,
    .cbResume = 0,
    .pPrev = 0};
