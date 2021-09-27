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
#include <ace/managers/rand.h>

static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;

//extern tState g_sStateIntro;
extern tStateManager *g_pStateMachineGame;
extern tState g_sStateMenu;

void waitFrames();

static tFont *s_pFont;
static tTextBitMap *s_pBmText;

static tPtplayerSfx *s_pAtari;

UBYTE typingRandomizer = 0;


char szWungiel[50];

const char *w1 = "READY";
const char load1[] = "LOAD ATARENIUM FALCON";
const char *w2 = "ERROR - INSERT WINCY WUNGLA TO DF0:";
const char dots[] = "...";
const char *w3 = "WUNGIEL DETECTED";
const char *w4 = "DONE";
const char run[] = "RUN";

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

    randInit(476);
    

    ptplayerCreate(1);
    s_pAtari = ptplayerSfxCreateFromFile("data/Atari.sfx");

    s_pFont = fontCreate("data/topaz.fnt");
    s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight);

    blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 21);
    blitRect(s_pVpManager->pBack, 0, 128, 320, 128, 21);

    sprintf(szWungiel, w1);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 10, 23, FONT_COOKIE);

    waitFrames(s_pVp, 50, 200);

    UWORD uwX = 10;
    for (UBYTE i = 0; i < strlen(load1); ++i) {
        sprintf(szWungiel, "%c", load1[i]);
        fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
        fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, uwX, 20, 23, FONT_COOKIE);
        uwX += fontGlyphWidth(s_pFont, load1[i]) + 1;
        ptplayerSfxPlay(s_pAtari, 3, 64, 100);
        typingRandomizer = ulRandMinMax(10, 20);
        waitFrames(s_pVp, typingRandomizer, 200);
    }


    waitFrames(s_pVp, 50, 200);

    sprintf(szWungiel, w2);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 30, 23, FONT_COOKIE);

    waitFrames(s_pVp, 50, 200);

    for (BYTE i = 0; i < 3; ++i) {
        sprintf(szWungiel, "%c", dots[i]);
        fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
        fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 10, 40, 23, FONT_COOKIE);
        waitFrames(s_pVp, 50, 200);
    }
    sprintf(szWungiel, w3);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 50, 23, FONT_COOKIE);

    waitFrames(s_pVp, 50, 200);

    uwX = 10;
    for (UBYTE i = 0; i < strlen(load1); ++i) {
        sprintf(szWungiel, "%c", load1[i]);
        fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
        fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, uwX, 60, 23, FONT_COOKIE);
        uwX += fontGlyphWidth(s_pFont, load1[i]) + 1;
        ptplayerSfxPlay(s_pAtari, 3, 64, 100);
        typingRandomizer = ulRandMinMax(10, 20);
       waitFrames(s_pVp, typingRandomizer, 200);
    }


    for (BYTE i = 0; i < 3; ++i) {
        sprintf(szWungiel, "%c", dots[i]);
        fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
        fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 10, 70, 23, FONT_COOKIE);
        waitFrames(s_pVp, 50, 200);
    }

    sprintf(szWungiel, w4);
    fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 10, 80, 23, FONT_COOKIE);

    waitFrames(s_pVp, 50, 200);

    for (BYTE i = 0; i < 3; ++i) {
        sprintf(szWungiel, "%c", run[i]);
        fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
        fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 10, 90, 23, FONT_COOKIE);
        ptplayerSfxPlay(s_pAtari, 3, 64, 100);
        typingRandomizer = ulRandMinMax(10, 20);
        waitFrames(s_pVp, typingRandomizer, 200);
    }

    for (BYTE i = 0; i < 3; ++i) {
        sprintf(szWungiel, "%c", dots[i]);
        fontFillTextBitMap(s_pFont, s_pBmText, szWungiel);
        fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, (i * 7) + 10, 100, 23, FONT_COOKIE);
        waitFrames(s_pVp, 50, 200);
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
    ptplayerSfxDestroy(s_pAtari);
    ptplayerDestroy();
    fontDestroy(s_pFont);
    fontDestroyTextBitMap(s_pBmText);
    joyClose();
    keyDestroy();
    viewDestroy(s_pView);
}

tState g_sStateWungiel ={
    .cbCreate = stateWungielCreate,
    .cbLoop = stateWungielLoop,
    .cbDestroy = stateWungielDestroy,
    .cbSuspend = 0,
    .cbResume = 0,
    .pPrev = 0 };
