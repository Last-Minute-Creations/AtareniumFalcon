#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/system.h>
#include <ace/managers/game.h>
#include <ace/utils/palette.h>
#include <ace/utils/font.h>
#include <stdio.h>
#include <ace/managers/rand.h>
#include <ace/managers/state.h>
#include <ace/utils/custom.h>
#include <ace/managers/ptplayer.h>
#include <ace/utils/file.h>
#include "enum.h"
#include "structures.h"

//------------------------------------------------------- gdzie� przed funkcjami
// zmienne trzymaj�ce adresy do viewa, viewporta, simple buffer managera
static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;
static tBitMap *s_pTiles;
static tBitMap *s_pTilesMask;
static tBitMap *s_pBg;
static tBitMap *s_pBgWithTile;
static tBitMap *s_pBgPortalGlow;
static tBitMap *s_pHUD;
static tBitMap *s_pFalconBg;
static tBitMap *s_pAnimBg;
static tBitMap *s_pRobboHUD;
static tBitMap *s_pRobboAnim;
static tPtplayerSfx *s_pFalkonEngineSound;
static tPtplayerSfx *s_pLadujWegiel;
static tPtplayerSfx *s_pRobbo8000;
static tPtplayerSfx *s_pCapacitorSFX;
static tCopBlock *copBlockBeforeHud, *copBlockAfterHud;

static tPtplayerMod *s_pMod;
static tPtplayerMod *s_pModAmbient;

static tFont *s_pFont;
static tFont *s_pGotekFont;
static tTextBitMap *s_pBmText;

static UWORD s_pPalette[32];

extern tMusicState musicPlay = MUSIC_HEAVY;
extern tAmigaMode amigaMode = AMIGA_MODE_OFF;
extern tHudState;
extern tFlyingAnimState;

extern tStateManager *g_pStateMachineGame;
extern tState g_sStateMenu;
extern tState g_sStateGameOver;
extern tState g_sStateScore;
extern tState g_sStateGuruMastah;
extern tState g_sStateScoreAmi;
extern tState g_sStateLeakedGameOver;

struct coords coords;
struct hud hud;
struct robboMsg robboMsg;
struct collected collected;
struct anim anim;
struct doubleBuffering db;
struct animStateControls stateControls;
struct moveControls moveControls;

#define MAP_TILE_HEIGHT 7
#define MAP_TILE_WIDTH 10
#define FALCON_HEIGHT 32
#define ANIM_FRAME_COUNT 8

#define LAST_LEVEL_NUMBER 28
#define STARTING_COAL 10

UBYTE level = 1;
char levelFilePath[20];

BYTE kamyki[10][7];
BYTE collectiblesAnim[10][7];
UWORD pAnim[] = {0, 32, 64, 96, 128, 160, 192, 224};

extern UBYTE cheatmodeEnablerWhenEqual3;
extern UBYTE secondCheatEnablerWhenEqual3;
extern UBYTE thirdCheatEnablerWhenEqual3;

UBYTE doubleBufferFrameControl = 2;
UBYTE prepareEndingPointCoordinatesOnce = TRUE;

void waitFrames(tVPort *pVPort, UBYTE ubHowMany, UWORD uwPosY)
{
  for (UBYTE i = 0; i < ubHowMany; ++i)
  {
    viewProcessManagers(pVPort->pView);
    copProcessBlocks();
    vPortWaitForEnd(pVPort);
  }
}
void gameOnResume(void)
{
  viewLoad(s_pView);
}
void initialSetupDeclarationOfDataInStructures(void)
{
  robboMsg.sz1stLine = "ROBBO says:";
  robboMsg.szCollision1stLine = "Collision course detected, ESP enabled.";
  robboMsg.szCollision2ndLine = "1T of fuel used, danger avioded. Over.";
  collected.coal = STARTING_COAL;
  collected.excessCoal = 0;
  collected.capacitors = 0;
  collected.robbo = 0;
  db.portal = 0;
  db.blueCap = 0;
  db.redCap = 0;
  db.robbo = 0;
  db.flyingAnimFrame = 0;
  anim.portalFrame = 0;
  anim.portalTick = 0;
  anim.portalTempo = 8;
  anim.redCapacitorTick = 0;
  anim.redCapacitorTempo = 8;
  anim.redCapacitorTileCheck = 0;
  anim.blueCapacitorTick = 0;
  anim.blueCapacitorTempo = 16;
  anim.blueCapacitorTileCheck = 0;
  anim.robboFrame = 0;
  anim.robboTick = 0;
  anim.robboTempo = 24;
  anim.falconTick = 0;
  anim.falconTempo = 8;
  anim.falconFrame = 0;
  anim.flyingTick = 0;
  anim.flyingFrame = 0;
  anim.flyingTempo = 128;
  stateControls.falconIdle = TRUE;
  stateControls.falconFlyingAnim = 0;
  stateControls.stoneHitAnim = FALSE;
  moveControls.stoneHit = FALSE;
  moveControls.frameHit = FALSE;
  moveControls.kierunek = 0;
  moveControls.kierunekHold = 0;
}

void setFalconCoordinates(void)
{
  for (UBYTE i = 0; i < MAP_TILE_WIDTH; ++i)
  {
    for (UBYTE j = 0; j < MAP_TILE_HEIGHT; ++j)
    {
      if (kamyki[i][j] == 1)
      {
        coords.falkonx = i;
        coords.falkony = j;
        coords.krawedzx = i;
        coords.krawedzy = j;
        coords.uwPosX = coords.falkonx * 32;
        coords.uwPosY = coords.falkony * 32;
        coords.targetTileX = coords.falkonx;
        coords.targetTileY = coords.falkony;
      }
    }
  }
}
void setPortalCoordinates(void)
{
  for (UBYTE i = 0; i < MAP_TILE_WIDTH; ++i)
  {
    for (UBYTE j = 0; j < MAP_TILE_HEIGHT; ++j)
    {
      if (kamyki[i][j] == 10)
      {
        coords.portalX = i;
        coords.portalY = j;
      }
    }
  }
}
void setRobboCoordinates(void)
{
  for (UBYTE i = 0; i < MAP_TILE_WIDTH; ++i)
  {
    for (UBYTE j = 0; j < MAP_TILE_HEIGHT; ++j)
    {
      if (kamyki[i][j] == 11)
      {
        coords.robboX = i;
        coords.robboY = j;
      }
    }
  }
}

void printOnHUD(void)
{
  if (amigaMode != AMIGA_MODE_OFF)
  {
    return;
  }

  blitCopy(s_pHUD, 32, 0, s_pVpManager->pBack, 32, 224, 32, 32, MINTERM_COOKIE);
  sprintf(hud.fuelCoalDisplay, "%d", collected.coal);
  fontFillTextBitMap(s_pFont, s_pBmText, hud.fuelCoalDisplay);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 42, 232, hud.fontColor, FONT_COOKIE);
  blitCopy(s_pHUD, 188, 0, s_pVpManager->pBack, 188, 224, 32, 32, MINTERM_COOKIE);
  sprintf(hud.capacitorsDisplay, "%d", collected.capacitors);
  fontFillTextBitMap(s_pFont, s_pBmText, hud.capacitorsDisplay);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 190, 236, hud.fontColor, FONT_COOKIE);
  blitCopy(s_pHUD, 128, 0, s_pVpManager->pBack, 128, 224, 32, 32, MINTERM_COOKIE);
  sprintf(hud.excessCoalDisplay, "%d", collected.excessCoal);
  fontFillTextBitMap(s_pFont, s_pBmText, hud.excessCoalDisplay);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 130, 232, hud.fontColor, FONT_COOKIE);
  blitCopy(s_pHUD, 248, 0, s_pVpManager->pBack, 248, 224, 32, 32, MINTERM_COOKIE);
  sprintf(hud.robboCountDisplay, "%d", collected.robbo);
  fontFillTextBitMap(s_pFont, s_pBmText, hud.robboCountDisplay);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 250, 236, hud.fontColor, FONT_COOKIE);
  blitCopy(s_pHUD, 288, 0, s_pVpManager->pBack, 288, 224, 32, 32, MINTERM_COOKIE);
  sprintf(hud.levelNumber, "%d", level);
  fontFillTextBitMap(s_pFont, s_pBmText, hud.levelNumber);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 290, 236, hud.fontColor, FONT_COOKIE);
}

void clearTiles(void) // czyszczenie planszy z tile'ow na koniec kazdego etapu zeby nie zostaly smieci
{
  blitCopy(s_pBg, 0, 0, s_pBgWithTile, 0, 0, 320, 128, MINTERM_COPY);    // podlozenie defaultowego tla do zmiennej
  blitCopy(s_pBg, 0, 128, s_pBgWithTile, 0, 128, 320, 96, MINTERM_COPY); // gdzie wklejam potem tile  -NIE KASOWAC KURWA !1
  for (UBYTE y = 0; y < MAP_TILE_HEIGHT; ++y)                            // w kazdym kolejnym rzedzie poziomym od gory
  {
    for (UBYTE x = 0; x < MAP_TILE_WIDTH; ++x) // na kazdej kolejnej pozycji od lewej do prawej
    {
      kamyki[x][y] = 0;           // wyczysc (wyzeruj) miejsce w tablicy trzymajace dana pozycje
      collectiblesAnim[x][y] = 0; // J.W. miejsce w tablicy kontrolujacej czy i jaki element jest animowany
    }
  }
}
void drawTiles(void)
{

  sprintf(levelFilePath, "data/%d.txt", level);
  systemUse();
  tFile *levelFile = fileOpen(levelFilePath, "r");
  UBYTE ubZmienna = 0;
  UBYTE ubStoneImg;
  BYTE x = 0;
  BYTE y = 0;

  for (UBYTE i = 0; i < 82; ++i)
  {
    fileRead(levelFile, &ubZmienna, 1);

    if (ubZmienna == 0x30)
    {
      kamyki[x][y] = 0;
      blitCopy(s_pBg, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBg, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
    }

    else if (ubZmienna == 0x33)
    {
      kamyki[x][y] = 3;
      ubStoneImg = ulRandMinMax(0, 2); // drawing one of 3 possible stone images
      blitCopy(s_pBg, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBg, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopyMask(s_pTiles, ubStoneImg * 32, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopyMask(s_pTiles, ubStoneImg * 32, 0, s_pVpManager->pFront, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
    else if (ubZmienna == 0x34)
    {
      kamyki[x][y] = 4;
      blitCopyMask(s_pTiles, 96, 0, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopyMask(s_pTiles, 96, 0, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);

      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
    }

    else if (ubZmienna == 0x35)
    {
      kamyki[x][y] = 5;
      blitCopyMask(s_pTiles, 128, 0, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopyMask(s_pTiles, 128, 0, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);

      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
    }

    else if (ubZmienna == 0x36)
    {
      kamyki[x][y] = 6;
      blitCopyMask(s_pTiles, 160, 0, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopyMask(s_pTiles, 160, 0, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);

      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
    }
    else if (ubZmienna == 0x37)
    {
      kamyki[x][y] = 7;
      blitCopyMask(s_pTiles, 192, 0, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopyMask(s_pTiles, 192, 0, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);

      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
    }
    else if (ubZmienna == 0x38)
    {
      kamyki[x][y] = 8;
      collectiblesAnim[x][y] = 8;
      blitCopyMask(s_pTiles, 0, 256, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopyMask(s_pTiles, 0, 256, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);

      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
    }
    else if (ubZmienna == 0x39)
    {
      kamyki[x][y] = 9;
      collectiblesAnim[x][y] = 9;
      blitCopyMask(s_pTiles, 0, 288, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopyMask(s_pTiles, 0, 288, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
    }
    else if (ubZmienna == 0x45) // PORTAL
    {
      kamyki[x][y] = 10;
      blitCopyMask(s_pTiles, 0, 352, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopyMask(s_pTiles, 0, 352, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);

      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
    }
    else if (ubZmienna == 0x52) //ROBBO
    {
      kamyki[x][y] = 11;
      blitCopyMask(s_pTiles, 0, 32, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopyMask(s_pTiles, 0, 32, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);

      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
    }
    else if (ubZmienna == 0x42) // BROKEN CAPAC
    {
      kamyki[x][y] = 12;
      blitCopyMask(s_pTiles, 224, 0, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      blitCopyMask(s_pTiles, 224, 0, s_pBgWithTile, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);

      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, MINTERM_COPY);
      blitCopy(s_pBgWithTile, x * 32, y * 32, s_pVpManager->pFront, x * 32, y * 32, 32, 32, MINTERM_COPY);
    }
    else if (ubZmienna == 0x31) // FALCON
    {
      kamyki[x][y] = 1;
    }

    ++x;
    if (ubZmienna == 0x0a)
    {
      x = 0;
      ++y;
    }
  }
  fileClose(levelFile);
  systemUnuse();
  setFalconCoordinates();
  setPortalCoordinates();
  setRobboCoordinates();
  paletteDim(s_pPalette, s_pVp->pPalette, 32, 15);
  viewUpdateCLUT(s_pView);
}

void setDestionationCoordsToEndFlying()
{
switch (moveControls.kierunekHold)
    {
    case 1:
      coords.falkonx = coords.falkonx + 1;
      break;
    case 2:
      coords.falkonx = coords.falkonx - 1;
      break;
    case 3:
      coords.falkony = coords.falkony - 1;
      break;
    case 4:
      coords.falkony = coords.falkony + 1;
      break;
    }
}

void ifCollectibleTileBgSetup(void)
{
  switch (moveControls.kierunekHold)
  {
  case 1:
    coords.targetTileX = coords.falkonx + 1;
    break;
  case 2:
    coords.targetTileX = coords.falkonx - 1;
    break;
  case 3:
    coords.targetTileY = coords.falkony - 1;
    break;
  case 4:
    coords.targetTileY = coords.falkony + 1;
    break;
  }
}

void falconIdleAnimation(void)
{
  if (stateControls.falconIdle == FALSE)
  {
    return;
  }

  if (anim.falconTick == anim.falconTempo * 1)
  {
    anim.falconFrame = 0;
  }
  else if (anim.falconTick == anim.falconTempo * 2)
  {
    anim.falconFrame = 1;
  }
  else if (anim.falconTick == anim.falconTempo * 3)
  {
    anim.falconFrame = 2;
  }
  else if (anim.falconTick == anim.falconTempo * 4)
  {
    anim.falconFrame = 3;
  }
  else if (anim.falconTick == anim.falconTempo * 5)
  {
    anim.falconFrame = 4;
  }
  else if (anim.falconTick == anim.falconTempo * 6)
  {
    anim.falconFrame = 5;
  }
  else if (anim.falconTick == anim.falconTempo * 7)
  {
    anim.falconFrame = 6;
  }
  else if (anim.falconTick >= anim.falconTempo * 8)
  {
    anim.falconFrame = 7;
    anim.falconTick = 0;
    if (stateControls.falconIdle == HOLD)
    {
      stateControls.falconIdle = FALSE;
      stateControls.falconFlyingAnim = BEGIN;
      /*
      if (stateControls.stoneHitAnim == HOLD){
        stateControls.stoneHitAnim = TRUE;
      }
      if (stateControls.falconFlyingAnim == HOLD){
        stateControls.falconFlyingAnim = BEGIN;
      } */
    }
  }
  UWORD uwPrevPosX = coords.uwPosX;
  UWORD uwPrevPosY = coords.uwPosY;
  switch (moveControls.kierunekHold)
  {
  case 1:
    if (uwPrevPosX >= 2)
    {
      --uwPrevPosX;
      --uwPrevPosX;
    }
    break;
  case 2:
    if (uwPrevPosX <= 318)
    {
      ++uwPrevPosX;
      ++uwPrevPosX;
    }
    break;
  case 3:
    if (uwPrevPosY <= 190)
    {
      ++uwPrevPosY;
      ++uwPrevPosY;
    }
    break;
  case 4:
    if (uwPrevPosY >= 2)
    {
      --uwPrevPosY;
      --uwPrevPosY;
    }
    break;
  }
  blitCopy(s_pBg, uwPrevPosX, uwPrevPosY, s_pVpManager->pBack, uwPrevPosX, uwPrevPosY, 32, 32, MINTERM_COOKIE);
  //blitCopy(s_pBg, uwPrevPosX, uwPrevPosY, s_pVpManager->pFront, uwPrevPosX, uwPrevPosY, 32, 32, MINTERM_COOKIE);
  blitCopy(s_pBg, coords.uwPosX, coords.uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);
  blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, coords.uwPosX, coords.uwPosY, 32, 32, MINTERM_COOKIE);
  blitCopyMask(s_pTiles, anim.falconFrame * 32, 192 + coords.falconFace, s_pVpManager->pBack, coords.falkonx * 32, coords.falkony * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
}

void blitBackground(void) // TO BEDZIE BLIT ANIM FRAME
{
  blitCopy(s_pBg, coords.uwPosX, coords.uwPosY, s_pVpManager->pBack, coords.uwPosX, coords.uwPosY, 32, 32, MINTERM_COOKIE);
}   

void falconFlyingAnimation(void)
{
  if (stateControls.falconFlyingAnim == FALSE){
    return;
  }
  if (stateControls.falconFlyingAnim == BEGIN){
  blitBackground();
  db.flyingAnimFrame = 1;
  stateControls.falconFlyingAnim = END;
  }

  else if (stateControls.falconFlyingAnim == END){
    setDestionationCoordsToEndFlying();
    coords.uwPosX = coords.falkonx * 32; 
    coords.uwPosY = coords.falkony * 32; 
    stateControls.falconIdle = TRUE; 
    stateControls.falconFlyingAnim = FALSE;

  }
  
  
}

void portalAnimBlit(void) // animacja portalu na planszy
{
  blitCopy(s_pBg, coords.portalX * 32, coords.portalY * 32, s_pBgPortalGlow, 0, 0, 32, 32, MINTERM_COOKIE);             // wytnij tlo w miejscu gdzie jest portal do s_pBgPortalGlow
  blitCopyMask(s_pTiles, anim.portalFrame * 32, 352, s_pBgPortalGlow, 0, 0, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);  // wytnij klatke tile'a i doklej do s_pBgPortalGlow z zachowaniem transparentnosci - maska
  blitCopy(s_pBgPortalGlow, 0, 0, s_pVpManager->pBack, coords.portalX * 32, coords.portalY * 32, 32, 32, MINTERM_COPY); // wrzuc gotowe s_pBgPortalGlow na ekran pBack
}
void robboAnimBlit(void) // animacja portalu na planszy
{
  blitCopy(s_pBg, coords.robboX * 32, coords.robboY * 32, s_pRobboAnim, 0, 0, 32, 32, MINTERM_COOKIE);
  blitCopyMask(s_pTiles, anim.robboFrame * 32, 32, s_pRobboAnim, 0, 0, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
  blitCopy(s_pRobboAnim, 0, 0, s_pVpManager->pBack, coords.robboX * 32, coords.robboY * 32, 32, 32, MINTERM_COPY);
}

void portalAnimFrameCounter(void)
{
  if (anim.portalTick > anim.portalTempo)
  {
    db.portal = 1;
    ++anim.portalFrame;
    anim.portalTick = 0;
    if (anim.portalFrame == 7)
    {
      anim.portalFrame = 0;
    }
    portalAnimBlit();
  }
}

void robboAnimCounter(void)
{
  if (anim.robboTick > anim.robboTempo)
  {
    db.robbo = 1;
    ++anim.robboFrame;
    anim.robboTick = 0;
    if (anim.robboFrame == 8)
    {
      anim.robboFrame = 0;
    }
    robboAnimBlit();
  }
}

void capacitorsAnimLooper(void)
{
  if (anim.redCapacitorTick > anim.redCapacitorTempo)
  {
    anim.redCapacitorTick = 0;
  }
  if (anim.blueCapacitorTick > anim.blueCapacitorTempo)
  {
    anim.blueCapacitorTick = 0;
  }
}

void redCapacitorsAnimation(void)
{
  UBYTE i = 0, k = 0;

  if (anim.redCapacitorTick == anim.redCapacitorTempo)
  {
    db.redCap = 1;
    for (i = 0; i < 10; ++i)
    {
      for (k = 0; k < 7; ++k)
      {
        if (collectiblesAnim[i][k] == 9)
        {
          blitCopy(s_pBg, i * 32, k * 32, s_pAnimBg, 0, 0, 32, 32, MINTERM_COOKIE);
          blitCopy(s_pAnimBg, 0, 0, s_pVpManager->pBack, i * 32, k * 32, 32, 32, MINTERM_COOKIE);
          blitCopyMask(s_pTiles, anim.redCapacitorTileCheck * 32, 288, s_pVpManager->pBack, i * 32, k * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
        }
      }
    }
    ++anim.redCapacitorTileCheck;
    if (anim.redCapacitorTileCheck == 7)
    {
      anim.redCapacitorTileCheck = 0;
    }
  }
}
void blueCapacitorsAnimation(void)
{

  if (anim.blueCapacitorTick == anim.blueCapacitorTempo)
  {
    db.blueCap = 1;
    for (UBYTE i = 0; i < 10; ++i)
    {
      for (UBYTE k = 0; k < 7; ++k)
      {
        if (collectiblesAnim[i][k] == 8)
        {
          blitCopy(s_pBg, i * 32, k * 32, s_pAnimBg, 0, 0, 32, 32, MINTERM_COOKIE);
          blitCopy(s_pAnimBg, 0, 0, s_pVpManager->pBack, i * 32, k * 32, 32, 32, MINTERM_COOKIE);
          blitCopyMask(s_pTiles, anim.blueCapacitorTileCheck * 32, 256, s_pVpManager->pBack, i * 32, k * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
        }
      }
    }
    ++anim.blueCapacitorTileCheck;
    if (anim.blueCapacitorTileCheck == 7)
    {
      anim.blueCapacitorTileCheck = 0;
    }
  }
}

void doubleBufferingHandler(void)
{
  if (db.flyingAnimFrame == 1){
    blitBackground();
    db.flyingAnimFrame = 0;
  }
  if (db.portal == 1)
  {
    portalAnimBlit();
    db.portal = 0;
  }
  if (db.blueCap == 1)
  {
    blueCapacitorsAnimation();
    db.blueCap = 0;
  }
  if (db.redCap == 1)
  {
    redCapacitorsAnimation();
    db.redCap = 0;
  }
  if (db.robbo == 1)
  {
    robboAnimBlit();
    db.robbo = 0;
  }

  if (doubleBufferFrameControl > 0)
  {
    if (hud.stateCtrl == HUD_PRINT)
    {
      printOnHUD();
    }
    else if (hud.stateCtrl == HUD_UP)
    {
      // robboScrollUp();
    }
    else if (hud.stateCtrl == HUD_DOWN)
    {
      //robboScrollDown();
    }

    if (hud.stateCtrl == HUD_ROBBO_SAYS)
    {
      // robboSays();
    }

    --doubleBufferFrameControl;
  }
}

void isThisStone(void)
{
  // funkcja sprawdzajaca przed wykonaniem ruchu czy chcemy wleciec w kamien

  BYTE stoneX = 0; // tu utrzymamy wspolrzedne docelowe
  BYTE stoneY = 0; // i porownamy czy jest na nich kamien

  switch (moveControls.kierunek)
  {
  case 1:                                    // gdy w prawo
    stoneX = coords.falkonx + 1;             // przypisz do stoneX docelowa wspolrzedna
    if (kamyki[stoneX][coords.falkony] == 3) // jesli pole docelowe to 3 (kamien)
    {
      moveControls.stoneHit = TRUE; // oznacz ze chciales walnac w kamyk dla dalszego procesowania
    }
    break;
  case 2: // i tak dalej dla reszty kierunkow
    stoneX = coords.falkonx - 1;
    if (kamyki[stoneX][coords.falkony] == 3)
    {
      moveControls.stoneHit = TRUE;
    }
    break;
  case 3:
    stoneY = coords.falkony - 1;
    if (kamyki[coords.falkonx][stoneY] == 3)
    {
      moveControls.stoneHit = TRUE;
    }
    break;
  case 4:
    stoneY = coords.falkony + 1;
    if (kamyki[coords.falkonx][stoneY] == 3)
    {
      moveControls.stoneHit = TRUE;
    }
    break;
  }
}
void isThisFrame(void)
{
  // tu jest funkcja sprawdzajaca czy sie chcemy wypierdolic za ekran i nie pozwalajaca na to
  switch (moveControls.kierunek)
  {
  case 1:                                  // gdy w prawo
    coords.krawedzx = coords.krawedzx + 1; // pole docelowe
    if (coords.krawedzx == 10)             // jesli za ekranem (bo fruwamy od 0 do 9)
    {
      coords.krawedzx = 9;          // ustaw znow na 9
      coords.falkonx = 9;           // zatrzymaj falkona tez na 9
      moveControls.frameHit = TRUE; // oznacz ze chciales walnac w ramke dla dalszego procesowania, animka i tak dalej
    }
    break;
  case 2: // JW w lewo
    coords.krawedzx = coords.krawedzx - 1;
    if (coords.krawedzx == -1)
    {
      coords.krawedzx = 0;
      coords.falkonx = 0;
      moveControls.frameHit = TRUE;
    }
    break;
  case 3: // JW w gore
    coords.krawedzy = coords.krawedzy - 1;
    if (coords.krawedzy == -1)
    {
      coords.krawedzy = 0;
      coords.falkony = 0;
      moveControls.frameHit = TRUE;
    }
    break;
  case 4: // JW w dol
    coords.krawedzy = coords.krawedzy + 1;
    if (coords.krawedzy == 7)
    {
      coords.krawedzy = 6;
      coords.falkony = 6;
      moveControls.frameHit = TRUE;
    }
    break;
  }
}

void stateGameCreate(void)
{
  // Here goes your startup code
  //-------------------------------------------------------------- gdzie� w create

  initialSetupDeclarationOfDataInStructures();

  s_pView = viewCreate(0,
                       TAG_VIEW_COPLIST_MODE, COPPER_MODE_BLOCK, // spos�b u�ywania coppera - ten jest najprostszy bo nic z nim nie musisz wi�cej robi�
                       TAG_VIEW_GLOBAL_CLUT, 1,                  // globalna paleta dla wszystkich viewport�w
                       TAG_END                                   // wi�cej argument�w nie ma
  );

  s_pVp = vPortCreate(0,
                      TAG_VPORT_VIEW, s_pView, // parent view
                      TAG_VPORT_BPP, 5,        // bits per pixel: 4bpp = 16col, 5pp = 32col, etc.
                      TAG_END);

  // kolory do zmiany nowy hud
  // 102  85  51 Index 6    0x653
  //  119 102  68 Index 7   0x764
  //  136 119  85 Index 8   0x875
  //  170 153 119 Index 9   0xA97
  //  187 170 136 Index 10  0xBA8

  copBlockBeforeHud = copBlockCreate(s_pView->pCopList, 5, 0xE2, s_pView->ubPosY + 224 - 1);
  copBlockAfterHud = copBlockCreate(s_pView->pCopList, 5, 0xE2, s_pView->ubPosY + 256 - 1);

  copMove(s_pView->pCopList, copBlockBeforeHud, &g_pCustom->color[6], 0x653);
  copMove(s_pView->pCopList, copBlockBeforeHud, &g_pCustom->color[7], 0x764);
  copMove(s_pView->pCopList, copBlockBeforeHud, &g_pCustom->color[8], 0x875);
  copMove(s_pView->pCopList, copBlockBeforeHud, &g_pCustom->color[9], 0xA97);
  copMove(s_pView->pCopList, copBlockBeforeHud, &g_pCustom->color[10], 0xBA8);

  // kolory do zmiany bazowe
  //  153  34   0 Index 6   0x920
  //  187  51   0 Index 7   0xB30
  //  204  68  17 Index 8   0xC41
  //  238 102  34 Index 9   0xE62
  //  255 119  51 Index 10  0xF73

  copMove(s_pView->pCopList, copBlockAfterHud, &g_pCustom->color[6], 0x920);
  copMove(s_pView->pCopList, copBlockAfterHud, &g_pCustom->color[7], 0xB30);
  copMove(s_pView->pCopList, copBlockAfterHud, &g_pCustom->color[8], 0xC41);
  copMove(s_pView->pCopList, copBlockAfterHud, &g_pCustom->color[9], 0xE62);
  copMove(s_pView->pCopList, copBlockAfterHud, &g_pCustom->color[10], 0xF73);

  copBlockDisable(s_pView->pCopList, copBlockBeforeHud);
  copBlockDisable(s_pView->pCopList, copBlockAfterHud);

  ptplayerCreate(1);
  s_pMod = ptplayerModCreate("data/mod.falkon");
  s_pModAmbient = ptplayerModCreate("data/mod.falkon-ambient2");
  //s_pModAmbient = ptplayerModCreate("data/mod.falkon-ambient");
  ptplayerLoadMod(s_pMod, 0, 0);

  s_pFalkonEngineSound = ptplayerSfxCreateFromFile("data/falkonEngine.sfx");
  s_pLadujWegiel = ptplayerSfxCreateFromFile("data/LadujWegiel.sfx");
  s_pRobbo8000 = ptplayerSfxCreateFromFile("data/robbo8000.sfx");
  s_pCapacitorSFX = ptplayerSfxCreateFromFile("data/cap11025.sfx");

  // Paleta z falkona
  paletteLoad("data/falkon.plt", s_pVp->pPalette, 32);
  paletteLoad("data/falkon.plt", s_pPalette, 32);

  g_pCustom->color[0] = 0x0FFF; // zmie� kolor zero aktualnie u�ywanej palety na 15,15,15

  if (thirdCheatEnablerWhenEqual3 != 3)
  {
    s_pTiles = bitmapCreateFromFile("data/tileset.bm", 0);          // z pliku tileset.bm, nie lokuj bitmapy w pami�ci FAST
    s_pTilesMask = bitmapCreateFromFile("data/tileset_mask.bm", 0); // z pliku tileset_mask.bm, nie lokuj bitmapy w pami�ci FAST
    s_pHUD = bitmapCreateFromFile("data/HUD.bm", 0);
    hud.fontColor = 23;
    copProcessBlocks();
  }
  else if (thirdCheatEnablerWhenEqual3 == 3)
  {
    amigaMode = AMIGA_MODE_CHECK;
    hud.fontColor = 5;
    s_pTiles = bitmapCreateFromFile("data/tileset2.bm", 0);
    s_pTilesMask = bitmapCreateFromFile("data/tileset_mask2.bm", 0);
    s_pHUD = bitmapCreateFromFile("data/amiHUD.bm", 0);
    copBlockEnable(s_pView->pCopList, copBlockBeforeHud);
    copBlockEnable(s_pView->pCopList, copBlockAfterHud);
  }

  s_pBg = bitmapCreateFromFile("data/tlo1.bm", 0);
  s_pBgWithTile = bitmapCreateFromFile("data/tlo1.bm", 0); // fragmenty tla do podstawiania po ruchu
  s_pRobboHUD = bitmapCreateFromFile("data/falkon_robbo.bm", 0);
  s_pFalconBg = bitmapCreate(48, 32, 5, BMF_INTERLEAVED);
  s_pAnimBg = bitmapCreate(48, 32, 5, BMF_INTERLEAVED);
  s_pBgPortalGlow = bitmapCreate(48, 32, 5, BMF_INTERLEAVED);
  s_pRobboAnim = bitmapCreate(48, 32, 5, BMF_INTERLEAVED);
  s_pFont = fontCreate("data/topaz.fnt");
  s_pGotekFont = fontCreate("data/gotekFont.fnt");
  s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight); // bitmapa robocza długa na 200px, wysoka na jedną linię tekstu

  // proste wy�wietlanie bitmapy na viewporcie
  s_pVpManager = simpleBufferCreate(0,
                                    TAG_SIMPLEBUFFER_VPORT, s_pVp,                              // parent viewport
                                    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED, // wst�pne czyszczenie bitmapy, przyspieszenie rysowania grafiki
                                    TAG_SIMPLEBUFFER_IS_DBLBUF, 1,                              // nie potrzebujemy double buffering
                                    TAG_SIMPLEBUFFER_BOUND_WIDTH, 320 + 16,
                                    TAG_END);

  cameraSetCoord(s_pVpManager->pCamera, 0, 0);

  // po zrobieniu simpleBufferCreate()
  bitmapLoadFromFile(s_pVpManager->pBack, "data/tlo1.bm", 0, 0); // wczytaj zawarto�� bg1.bm bezpo�rednio do bitmapy bufora ekranu, zaczynaj�c od pozycji 0,0
  bitmapLoadFromFile(s_pVpManager->pFront, "data/tlo1.bm", 0, 0);
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 32, MINTERM_COOKIE);
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pFront, 0, 224, 320, 32, MINTERM_COOKIE);
  joyOpen(); // b�dziemy u�ywa� d�oja w grze
  keyCreate();
  // na koniec create:

  randInit(1337);
  printOnHUD();
  doubleBufferFrameControl = 2;

  viewProcessManagers(s_pView);
  viewLoad(s_pView);

  blitCopy(s_pBg, 0, 0, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE);
  blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, 0, 0, 33, 32, MINTERM_COOKIE);

  drawTiles();
  setFalconCoordinates();
  ptplayerEnableMusic(1);
  systemUnuse(); // system w trakcie loop nie jest nam potrzebny
}

void stateGameLoop(void)
{
  doubleBufferingHandler();

  ++anim.robboTick;
  robboAnimCounter();

  ++anim.portalTick;
  portalAnimFrameCounter();

  ++anim.redCapacitorTick;
  ++anim.blueCapacitorTick;
  capacitorsAnimLooper();

  if (stateControls.falconIdle == TRUE || stateControls.falconIdle == HOLD)
  {
    ++anim.falconTick;
  }

  if (stateControls.falconFlyingAnim == TRUE)
  {
    ++anim.flyingTick;
  }

  //falkonHittingStone();

  //robboScrollUp();
  //robboScrollDown();
  //portalAnim();

  falconIdleAnimation();
  falconFlyingAnimation();
  redCapacitorsAnimation();
  blueCapacitorsAnimation();

  moveControls.kierunek = 0;

  joyProcess();
  keyProcess();

  if (joyUse(JOY1_RIGHT) || keyUse(KEY_D) || keyUse(KEY_RIGHT))
  {
    moveControls.kierunek = 1;
    coords.falconFace = 0;
  }
  else if (joyUse(JOY1_LEFT) || keyUse(KEY_A) || keyUse(KEY_LEFT))
  {
    moveControls.kierunek = 2;
    coords.falconFace = 32;
  }
  else if (joyUse(JOY1_UP) || keyUse(KEY_W) || keyUse(KEY_UP))
  {
    moveControls.kierunek = 3;
  }
  else if (joyUse(JOY1_DOWN) || keyUse(KEY_S) || keyUse(KEY_DOWN))
  {
    moveControls.kierunek = 4;
  }

  if (moveControls.kierunek != 0)
  {
    if (stateControls.falconFlyingAnim != FLYING_ANIM_OFF)
    {
      return;
    }
    moveControls.kierunekHold = moveControls.kierunek;
    stateControls.falconIdle = HOLD;
    prepareEndingPointCoordinatesOnce = TRUE;
    /*
      if (musicPlay == MUSIC_AMBIENT_SFX)
      {
        ptplayerSfxPlay(s_pFalkonEngineSound, 3, 64, 100);
      }
       */
    isThisStone();
    isThisFrame();
  }

  viewProcessManagers(s_pView); // obliczenia niezb�dne do poprawnego dzia�ania viewport�w
  copProcessBlocks();           // obliczenia niezb�dne do poprawnego dzia�ania coppera
  vPortWaitForEnd(s_pVp);       // r�wnowa�ne amosowemu wait vbl
}

void stateGameDestroy(void)
{
  // Here goes your cleanup code
  systemUse(); // w��cz grzecznie system
  ptplayerModDestroy(s_pMod);
  ptplayerModDestroy(s_pModAmbient);
  ptplayerSfxDestroy(s_pFalkonEngineSound);
  ptplayerSfxDestroy(s_pLadujWegiel);
  ptplayerSfxDestroy(s_pRobbo8000);
  ptplayerSfxDestroy(s_pCapacitorSFX);
  ptplayerDestroy();
  bitmapDestroy(s_pTiles);
  bitmapDestroy(s_pTilesMask);
  bitmapDestroy(s_pBg);
  bitmapDestroy(s_pBgWithTile);
  bitmapDestroy(s_pHUD);
  bitmapDestroy(s_pFalconBg);
  bitmapDestroy(s_pAnimBg);
  bitmapDestroy(s_pBgPortalGlow);
  bitmapDestroy(s_pRobboHUD);
  bitmapDestroy(s_pRobboAnim);

  fontDestroy(s_pFont);
  fontDestroy(s_pGotekFont);
  fontDestroyTextBitMap(s_pBmText);

  viewDestroy(s_pView); // zwolnij z pami�ci view, wszystkie do��czone do niego viewporty i wszystkie do��czone do nich mened�ery
  joyClose();
  keyDestroy();
}

// pod funkcjami bo kompilator czyta od g�ry do do�u i musi najpierw napotka� funkcje by wiedzie� �e istniej�
tState g_sStateGame = {
    .cbCreate = stateGameCreate,
    .cbLoop = stateGameLoop,
    .cbDestroy = stateGameDestroy,
    .cbSuspend = 0,
    .cbResume = gameOnResume,
    .pPrev = 0};
