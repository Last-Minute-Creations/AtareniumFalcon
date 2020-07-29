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

BYTE page = 0;
char szIntro[80];

const char *IntroContinue = "(Fire or Return to continue)";

const char *line1 = "Captain Tramiel, our planet was attacked by";
const char *line2 = "treacherous Great Miner and his race of";
const char *line3 = "Amigans. Almost all coal supplies for our";
const char *line4 = "Atari are gone.";
const char *line5 = "Renton - Minister of Mining is reporting";
const char *line6 = "that natural coal deposits are almost";
const char *line7 = "depleted and most of our miners fell to a";
const char *line8 = "strange disease and must remain qurantined.";
const char *line9 = "There is nobody left to work in mines.";
const char *line10 = "If we run out of coal for our Ataris we";
const char *line11 = "face the apocalypse of punch out tickets.";
const char *line12 = "Scouting ROBBOts we sent out into space";
const char *line13 = "did not return. ";
const char *line14 = "We need your help Captain Tramiel.";
const char *line15 = "You have to go to Amigans Space Sector";
const char *line16 = "(ASS), retrieve any surviving ROBBOts ";
const char *line17 = "and reclaim our coal supplies. ";
const char *line18 = "Beware of their secret weapon";
const char *line19 = "- 16bit processing power, 32-color graphics";
const char *line20 = "and 4-channel sound.";
const char *line21 = "Don't let them foul you.";

const char *line22 = "We are giving you masterpiece of our space";
const char *line23 = "engineery. Our newest spacecraft named";
const char *line24 = "ATARENIUM FALCON. ";
const char *line25 = "Fully automated. The core of the craft is";
const char *line26 = "Atari Falcon running on highly efficient ";
const char *line27 = "eco pea-coal. It is also equipped with";
const char *line28 = "external automated arm.";
const char *line29 = "This is prototype experimental. Now GO!";
const char *line30 = "One more thing Captain... While you traverse";
const char *line31 = "enemy territory try to collect as many";
const char *line32 = "capacitors as you can so the Amigans can't";
const char *line33 = "replace leaking ones in their Amigas.";
const char *line34 = "Let electrolyte eat out their PCBs.";
const char *line35 = "HAR! HAR! HAR!";

void intro2ndPage(void)
{
  blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 21);
  blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 21);

  for (BYTE i = 0; i < 14; ++i)
  {

    switch (i)
    {
    case 0:
      sprintf(szIntro, line22);
      break;
    case 1:
      sprintf(szIntro, line23);
      break;
    case 2:
      sprintf(szIntro, line24);
      break;
    case 3:
      sprintf(szIntro, line25);
      break;
    case 4:
      sprintf(szIntro, line26);
      break;
    case 5:
      sprintf(szIntro, line27);
      break;
    case 6:
      sprintf(szIntro, line28);
      break;
    case 7:
      sprintf(szIntro, line29);
      break;
    case 8:
      sprintf(szIntro, line30);
      break;
    case 9:
      sprintf(szIntro, line31);
      break;
    case 10:
      sprintf(szIntro, line32);
      break;
    case 11:
      sprintf(szIntro, line33);
      break;
    case 12:
      sprintf(szIntro, line34);
      break;
    case 13:
      sprintf(szIntro, line35);
      break;
    }

    fontFillTextBitMap(s_pFont, s_pBmText, szIntro);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, (i * 9) + 10, 23, FONT_COOKIE);
    for (BYTE k = 0; k < 50; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }
  }
  sprintf(szIntro, IntroContinue);
  fontFillTextBitMap(s_pFont, s_pBmText, szIntro);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 5, 200, 23, FONT_COOKIE);
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

    switch (i)
    {
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
    }

    fontFillTextBitMap(s_pFont, s_pBmText, szIntro);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, (i * 9) + 10, 23, FONT_COOKIE);
    for (BYTE k = 0; k < 50; ++k)
    {
      vPortWaitForEnd(s_pVp);
    }
  }
  sprintf(szIntro, IntroContinue);
  fontFillTextBitMap(s_pFont, s_pBmText, szIntro);
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
