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

#define INTRO_LINE_COUNT 35

static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;

extern tState g_sStateMenu;
extern tStateManager *g_pStateMachineGame;

static tFont *s_pFont;
static tTextBitMap *s_pBmText;

BYTE page = 0;


const char *IntroContinue = "(Fire or Return to continue)";

const char *s_pLines[INTRO_LINE_COUNT] = {
"Captain Tramiel, our planet was attacked by",
"treacherous Great Miner and his race of",
"Amigans. Almost all coal supplies for our",
"Atari are gone.",
"Renton - Minister of Mining is reporting",
"that natural coal deposits are almost",
"depleted and most of our miners fell to a",
"strange disease and must remain qurantined.",
"There is nobody left to work in mines.",
"If we run out of coal for our Ataris we",
"face the apocalypse of punch out tickets.",
"Scouting ROBBOts we sent out into space",
"did not return. ",
"We need your help Captain Tramiel.",
"You have to go to Amigans Space Sector",
"(ASS), retrieve any surviving ROBBOts ",
"and reclaim our coal supplies. ",
"Beware of their secret weapon",
"- 16bit processing power, 32-color graphics",
"and 4-channel sound.",
"Don't let them foul you.",

"We are giving you masterpiece of our space",
"engineery. Our newest spacecraft named",
"ATARENIUM FALCON. ",
"Fully automated. The core of the craft is",
"Atari Falcon running on highly efficient ",
"eco pea-coal. It is also equipped with",
"external automated arm.",
"This is prototype experimental. Now GO!",
"One more thing Captain... While you traverse",
"enemy territory try to collect as many",
"capacitors as you can so the Amigans can't",
"replace leaking ones in their Amigas.",
"Let electrolyte eat out their PCBs.",
"HAR! HAR! HAR!"
};

void intro2ndPage(void)
{
  blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 21);
  blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 21);

  for (BYTE i = 0; i < 14; ++i)
  {
    fontFillTextBitMap(s_pFont, s_pBmText, s_pLines[i+21]);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, (i * 9) + 10, 23, FONT_COOKIE);
    for (BYTE k = 0; k < 50; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }
  }
  fontFillTextBitMap(s_pFont, s_pBmText, IntroContinue);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 200, 23, FONT_COOKIE);
  for (BYTE k = 0; k < 50; ++k)
  {
    vPortWaitForEnd(s_pVp);
  }
}

void stateIntroCreate(void)
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

  for (BYTE i = 0; i < 21; ++i)
  {
    fontFillTextBitMap(s_pFont, s_pBmText, s_pLines[i]);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, (i * 9) + 10, 23, FONT_COOKIE);
    for (BYTE k = 0; k < 50; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }
  }
  
  fontFillTextBitMap(s_pFont, s_pBmText, IntroContinue);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 200, 23, FONT_COOKIE);
  for (BYTE k = 0; k < 50; ++k)
  {
    vPortWaitForEnd(s_pVp);
  }
}

void stateIntroLoop(void)
{
  joyProcess();
  keyProcess();

  if (joyUse(JOY1_FIRE) || keyUse(KEY_RETURN))
  {
    if (page == 0)
    {
      ++page;
      intro2ndPage();
    }
    else if (page == 1)
    {
      page = 0;
      stateChange(g_pStateMachineGame, &g_sStateMenu);
      return;
    }
  }

  viewProcessManagers(s_pView);
  copProcessBlocks();
  vPortWaitForEnd(s_pVp);
}

void stateIntroDestroy(void)
{
  systemUse();
	fontDestroy(s_pFont);
	fontDestroyTextBitMap(s_pBmText);
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
    .pPrev = 0};
