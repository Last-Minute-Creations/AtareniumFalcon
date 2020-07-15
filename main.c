#include <ace/generic/main.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/system.h>
#include <ace/managers/game.h>
#include <ace/utils/palette.h>
#include <ace/utils/font.h>
#include <stdio.h>

//------------------------------------------------------- gdzie� przed funkcjami
// zmienne trzymaj�ce adresy do viewa, viewporta, simple buffer managera
static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;
static tBitMap *s_pTiles;
static tBitMap *s_pTilesMask;
static tBitMap *s_pBg;
static tBitMap *s_pHUD;

static tFont *s_pFont;
static tTextBitMap *s_pBmText;

#define MAP_TILE_HEIGHT 7
#define MAP_TILE_WIDTH 10

char szMsg[50]; // do wyswietlania wegla na HUD
char szMsg2[50]; // do wyswietlania kondkow na HUD


BYTE kamyki[10][7];

// coordsy do rysowania falkona i kontrolowania zeby sie nie wypierdolil za ekran
BYTE falkonx = 0;
BYTE falkony = 0;
BYTE krawedzx = 0;
BYTE krawedzy = 0;
BYTE kierunek = 0;

BYTE stoneHit = 0;
BYTE coal = 10;
BYTE capacitors = 0;
BYTE level = 1;


void drawTiles(void) {
for(UBYTE y = 0; y < MAP_TILE_HEIGHT; ++y) {
  for(UBYTE x = 0; x < MAP_TILE_WIDTH; ++x) {
    // i już masz zmienne x,y które się ruszają
    switch (kamyki[x][y])    {
    case 1:
    blitCopyMask(s_pTiles, 0, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break;
    case 2:
    blitCopyMask(s_pTiles, 32, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break;
    case 3:
    blitCopyMask(s_pTiles, 64, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break;          
    case 4:
      blitCopyMask(s_pTiles, 96, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break;
    case 5:
      blitCopyMask(s_pTiles, 128, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break;
    case 6:
      blitCopyMask(s_pTiles, 160, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break;
    case 7:
      blitCopyMask(s_pTiles, 192, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break; 
    case 8:
      blitCopyMask(s_pTiles, 0, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break; 
    case 9:
      blitCopyMask(s_pTiles, 32, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break; 
    case 10:
      blitCopyMask(s_pTiles, 64, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break;
    case 11:
      blitCopyMask(s_pTiles, 96, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break;      
    case 12:
      blitCopyMask(s_pTiles, 128, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
      break;
    }
  }
}
}

void clearTiles(void) {
for(UBYTE y = 0; y < MAP_TILE_HEIGHT; ++y) {
  for(UBYTE x = 0; x < MAP_TILE_WIDTH; ++x) {
    kamyki[x][y] = 0;  
    } 
  } 
} 

void printOnHUD(void) {
  sprintf(szMsg, "%d", coal);
  fontFillTextBitMap(s_pFont, s_pBmText, szMsg);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  48, 229, 5, FONT_COOKIE);

  sprintf(szMsg2, "%d", capacitors);
  fontFillTextBitMap(s_pFont, s_pBmText, szMsg2);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  112, 229, 5, FONT_COOKIE);
}

void nextLevel(void) {
  clearTiles();
  blitCopy(s_pBg, 0, 0, s_pVpManager->pBack, 0, 0, 320, 128,MINTERM_COOKIE, 0xFF);
  blitCopy(s_pBg, 0, 0, s_pVpManager->pBack, 0, 128, 320, 128,MINTERM_COOKIE, 0xFF);
  blitCopy(s_pHUD, 0, 224, s_pVpManager->pBack, 0, 224, 320,32,MINTERM_COOKIE, 0xFF);
  printOnHUD();
  switch(level){
    case 2:
    falkonx = 0;
    falkony = 0;
    krawedzx = 0;
    krawedzy = 0;
    kierunek = 0;

    kamyki[0][0] = 12;
    kamyki[0][2] = 3; 
    kamyki[1][0] = 7; 
    kamyki[2][3] = 8;
    kamyki[1][1] = 6;
    kamyki[8][3] = 5;
    kamyki[4][4] = 5;
    kamyki[6][5] = 11;
    kamyki[7][5] = 7;
    kamyki[9][6] = 10;
    
    drawTiles();
      break;
    case 3:
    falkonx = 0;
    falkony = 0;
    krawedzx = 0;
    krawedzy = 0;
    kierunek = 0;
    
    kamyki[0][0] = 12;
    kamyki[1][1] = 4;
    kamyki[3][1] = 4;
    kamyki[6][1] = 9;
    kamyki[7][1] = 7;
    kamyki[6][2] = 4; 
    kamyki[8][5] = 6; 
    kamyki[1][3] = 4;  
    kamyki[4][3] = 5; 
    kamyki[5][3] = 11; 
    kamyki[7][5] = 2; 
    kamyki[9][6] = 10;

    drawTiles();
      break;
    case 4:
    falkonx = 0;
    falkony = 0;
    krawedzx = 0;
    krawedzy = 0;
    kierunek = 0;

    kamyki[0][0] = 12;
    kamyki[1][1] = 3;
    kamyki[0][1] = 4;
    kamyki[1][0] = 5;
    kamyki[1][2] = 11;
    kamyki[2][2] = 7;
    kamyki[3][4] = 3;
    kamyki[3][5] = 3;
    kamyki[3][6] = 7;
    kamyki[5][6] = 3;
    kamyki[6][6] = 4;
    kamyki[6][4] = 6;
    kamyki[9][6] = 10;
    
    drawTiles();
      break;
    
    case 5:
    falkonx = 0;
    falkony = 0;
    krawedzx = 0;
    krawedzy = 0;
    kierunek = 0;

    kamyki[0][0] = 12;
    kamyki[1][0] = 3;
    kamyki[4][0] = 4;
    kamyki[7][0] = 3;
    kamyki[9][0] = 9;
    kamyki[0][1] = 6;
    kamyki[1][1] = 3;
    kamyki[2][1] = 5;
    kamyki[3][1] = 3;
    kamyki[4][1] = 8;
    kamyki[6][1] = 5;
    kamyki[7][1] = 3;
    kamyki[9][1] = 3;
    kamyki[3][2] = 3;
    kamyki[4][2] = 3;
    kamyki[5][2] = 3;
    kamyki[7][2] = 3;
    kamyki[9][2] = 7;
    kamyki[1][3] = 3;
    kamyki[2][3] = 5;  
    kamyki[4][3] = 6;
    kamyki[6][3] = 4;
    kamyki[9][3] = 6;
    kamyki[1][4] = 3;
    kamyki[2][4] = 3;
    kamyki[4][4] = 11; 
    kamyki[5][4] = 3;
    kamyki[8][4] = 4;
    kamyki[0][5] = 7;
    kamyki[2][5] = 3;
    kamyki[3][5] = 3;
    kamyki[4][5] = 3;
    kamyki[5][5] = 3;
    kamyki[6][5] = 3;
    kamyki[7][5] = 3;
    kamyki[0][5] = 8;
    kamyki[3][5] = 6;
    kamyki[4][5] = 7;
    kamyki[6][6] = 7;
    kamyki[8][6] = 3;
    kamyki[9][6] = 10;

    drawTiles();
      break;

    case 6:
    falkonx = 5;
    falkony = 4;
    krawedzx = 5;
    krawedzy = 4;
    kierunek = 0;

    kamyki[0][0] = 9;
    kamyki[1][0] = 3;
    kamyki[2][0] = 8;
    kamyki[3][0] = 3;
    kamyki[4][0] = 3;
    kamyki[7][0] = 4;
    kamyki[9][0] = 8;
    kamyki[1][1] = 3;
    kamyki[2][1] = 8;
    kamyki[3][1] = 3;
    kamyki[4][1] = 9;
    kamyki[5][1] = 3;
    kamyki[6][1] = 3;
    kamyki[7][1] = 7;
    kamyki[8][1] = 3;
    kamyki[9][1] = 3;
    kamyki[0][2] = 6;
    kamyki[2][2] = 5;
    kamyki[6][2] = 7;
    kamyki[9][2] = 6;
    kamyki[1][3] = 3;
    kamyki[3][3] = 3;
    kamyki[4][3] = 3;
    kamyki[5][3] = 3;
    kamyki[6][3] = 3;
    kamyki[7][3] = 3;
    kamyki[8][3] = 4;
    kamyki[9][3] = 3;
    kamyki[0][4] = 4;
    kamyki[1][4] = 3; 
    kamyki[3][4] = 10;
    kamyki[4][4] = 3;
    kamyki[5][4] = 12;
    kamyki[6][4] = 3;
    kamyki[9][4] = 9;
    kamyki[2][5] = 3;
    kamyki[3][5] = 3;
    kamyki[4][5] = 3;
    kamyki[5][5] = 7;
    kamyki[6][5] = 3;
    kamyki[7][5] = 3;
    kamyki[8][5] = 8;
    kamyki[9][5] = 3;
    kamyki[0][6] = 3;
    kamyki[1][6] = 5; 
    kamyki[4][6] = 8; 
    kamyki[8][6] = 4; 
    kamyki[9][6] = 3;

    drawTiles();
      break;

    case 7: // ta ma byc ostatnia 
    falkonx = 0;
    falkony = 3;
    krawedzx = 0;
    krawedzy = 3;
    kierunek = 0;

    kamyki[0][0] = 3;
    kamyki[1][0] = 3;
    kamyki[2][0] = 3;
    kamyki[3][0] = 3;
    kamyki[4][0] = 3;
    kamyki[5][0] = 3;
    kamyki[6][0] = 3;
    kamyki[7][0] = 3;
    kamyki[8][0] = 3;
    kamyki[9][0] = 3;
    kamyki[0][1] = 3;
    kamyki[2][1] = 7;
    kamyki[3][1] = 6;
    kamyki[4][1] = 9; 
    kamyki[5][1] = 7;
    kamyki[6][1] = 7; 
    kamyki[7][1] = 7; 
    kamyki[8][1] = 6;
    kamyki[9][1] = 3;
    kamyki[0][2] = 3;
    kamyki[2][2] = 5; 
    kamyki[6][2] = 7; 
    kamyki[9][2] = 3;
    kamyki[0][3] = 12;
    kamyki[1][3] = 11;
    kamyki[2][3] = 7;
    kamyki[3][3] = 7; 
    kamyki[4][3] = 7; 
    kamyki[5][3] = 6;
    kamyki[6][3] = 7;
    kamyki[7][3] = 6;
    kamyki[8][3] = 4; 
    kamyki[9][3] = 3;
    kamyki[0][4] = 3;
    kamyki[1][4] = 7;
    kamyki[3][4] = 7;
    kamyki[4][4] = 6;
    kamyki[5][4] = 7;
    kamyki[6][4] = 5;
    kamyki[9][4] = 3;
    kamyki[0][5] = 3;
    kamyki[2][5] = 7;
    kamyki[3][5] = 9;
    kamyki[4][5] = 6; 
    kamyki[5][5] = 7;
    kamyki[6][5] = 7;
    kamyki[7][5] = 6;
    kamyki[8][5] = 10;
    kamyki[9][5] = 3;

    kamyki[0][6] = 3;
    kamyki[1][6] = 3; 
    kamyki[2][6] = 3;
    kamyki[3][6] = 3;
    kamyki[4][6] = 3; 
    kamyki[5][6] = 3;
    kamyki[6][6] = 3;
    kamyki[7][6] = 3; 
    kamyki[8][6] = 3;
    kamyki[9][6] = 3;

    drawTiles();
      break;
    
  }
}

void czyRamka(void) {
    // tu jest funkcja sprawdzajaca czy sie chcemy wypierdolic za ekran i nie pozwalajaca na to
    switch (kierunek){
      case 1:
      krawedzx = krawedzx + 1;
        if(krawedzx == 10){
          krawedzx = 9;
          falkonx = 8;
        }
        break;
      case 2:
      krawedzx = krawedzx - 1;
        if(krawedzx == -1){
          krawedzx = 0;
          falkonx = 1;
        }
        break;
      case 3:
      krawedzy = krawedzy - 1;
        if(krawedzy == -1){
          krawedzy = 0;
          falkony = 1;
        }
        break;
      case 4:
      krawedzy = krawedzy + 1;
        if(krawedzy == 7){
          krawedzy = 6;
          falkony = 5;
        }
        break;



    }
}

void isThisStone(void){
      // funkcja sprawdzajaca przed wykonaniem ruchu czy chcemy wleciec w kamien

      BYTE stoneX = 0;
      BYTE stoneY = 0;

       switch (kierunek){
      case 1:
      stoneX = falkonx + 1;
        if(kamyki[stoneX][falkony] == 3){
          stoneHit = 1;
        }
      break;
      case 2:
      stoneX = falkonx - 1;
        if(kamyki[stoneX][falkony] == 3){
          stoneHit = 1;
        }
      break;
      case 3:
      stoneY = falkony - 1;
        if(kamyki[falkonx][stoneY] == 3){
          stoneHit = 1;
        }
      break;
      case 4:
      stoneY = falkony + 1;
        if(kamyki[falkonx][stoneY] == 3){
          stoneHit = 1;
        }
        break;



    }
}

void coalAndCollect(void) {
  //funkcja do zbierania zasobu jesli jest na danym tajlu

  BYTE pickSthX = 0;
  BYTE pickSthY = 0;


  pickSthX = falkonx;
  pickSthY = falkony;

  if(kamyki[pickSthX][pickSthY] == 4){
    coal = coal + 2;
  }
  if(kamyki[pickSthX][pickSthY] == 5){
    coal = coal + 3;
  }
  if(kamyki[pickSthX][pickSthY] == 6){
    coal = coal + 4;
  }
  if(kamyki[pickSthX][pickSthY] == 7){
    coal = coal + 5;
  }
  if(kamyki[pickSthX][pickSthY] == 8){
    capacitors = capacitors + 2;
    blitCopy(s_pHUD, 96, 224, s_pVpManager->pBack, 96, 224, 32, 32,MINTERM_COOKIE, 0xFF);
    sprintf(szMsg2, "%d", capacitors);
    fontFillTextBitMap(s_pFont, s_pBmText, szMsg2);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  112, 229, 5, FONT_COOKIE);
  }
  if(kamyki[pickSthX][pickSthY] == 9){
    capacitors = capacitors + 4;
    blitCopy(s_pHUD, 96, 224, s_pVpManager->pBack, 96, 224, 32, 32,MINTERM_COOKIE, 0xFF);
    sprintf(szMsg2, "%d", capacitors);
    fontFillTextBitMap(s_pFont, s_pBmText, szMsg2);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  112, 229, 5, FONT_COOKIE);
  }
  if(kamyki[pickSthX][pickSthY] == 10){
    ++level;
    nextLevel();
  }
  
  coal = coal - 1;
  blitCopy(s_pHUD, 32, 224, s_pVpManager->pBack, 32, 224, 32, 32,MINTERM_COOKIE, 0xFF);
  sprintf(szMsg, "%d", coal);
  fontFillTextBitMap(s_pFont, s_pBmText, szMsg);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText,  48, 229, 5, FONT_COOKIE);
}

void falconMove(void){


        // jesli byl kamien to brak ruchu
    if(stoneHit == 1){
      stoneHit = 0;
      return;
    }
        // ruch falkonem na razie skokowo

   switch (kierunek){
      case 1:
      blitCopy(s_pBg, falkonx * 32, falkony * 32, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32,MINTERM_COOKIE, 0xFF);
      falkonx = falkonx + 1;
      blitCopyMask(s_pTiles, 128, 32, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
        break;
      case 2:
      blitCopy(s_pBg, falkonx * 32, falkony * 32, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32,MINTERM_COOKIE, 0xFF);
      falkonx = falkonx - 1;
      blitCopyMask(s_pTiles, 128, 32, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
        break;
      case 3:
      blitCopy(s_pBg, falkonx * 32, falkony * 32, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32,MINTERM_COOKIE, 0xFF);
      falkony = falkony - 1;
      blitCopyMask(s_pTiles, 128, 32, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
        break;
      case 4:
      blitCopy(s_pBg, falkonx * 32, falkony * 32, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32,MINTERM_COOKIE, 0xFF);
      falkony = falkony + 1;
      blitCopyMask(s_pTiles, 128, 32, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
        break;
   }
}





void noCoalLeft(void) {
  // sprawdzenie warunku na game over

  if(coal == 0){
    gameExit();
  }
}





void genericCreate(void) {
  // Here goes your startup code
  //-------------------------------------------------------------- gdzie� w create
s_pView = viewCreate(0,
    TAG_VIEW_COPLIST_MODE, COPPER_MODE_BLOCK, // spos�b u�ywania coppera - ten jest najprostszy bo nic z nim nie musisz wi�cej robi�
    TAG_VIEW_GLOBAL_CLUT, 1, // globalna paleta dla wszystkich viewport�w
    TAG_END // wi�cej argument�w nie ma
);

s_pVp = vPortCreate(0,
    TAG_VPORT_VIEW, s_pView, // parent view
    TAG_VPORT_BPP, 5, // bits per pixel: 4bpp = 16col, 5pp = 32col, etc.
    TAG_END
);
// Paleta z falkona
paletteLoad("data/falkon.plt", s_pVp->pPalette, 32);

s_pTiles = bitmapCreateFromFile("data/tileset.bm", 0); // z pliku tileset.bm, nie lokuj bitmapy w pami�ci FAST
s_pTilesMask = bitmapCreateFromFile("data/tileset.bm", 0); // z pliku tileset.bm, nie lokuj bitmapy w pami�ci FAST
s_pBg = bitmapCreateFromFile("data/tlo1.bm", 0); // fragmenty tla do podstawiania po ruchu
s_pHUD = bitmapCreateFromFile("data/HUD.bm", 0);

s_pFont = fontCreate("data/uni54.fnt");
s_pBmText = fontCreateTextBitMap(200, s_pFont->uwHeight); // bitmapa robocza długa na 200px, wysoka na jedną linię tekstu


// proste wy�wietlanie bitmapy na viewporcie
s_pVpManager = simpleBufferCreate(0,
    TAG_SIMPLEBUFFER_VPORT, s_pVp, // parent viewport
    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED, // wst�pne czyszczenie bitmapy, przyspieszenie rysowania grafiki
    TAG_SIMPLEBUFFER_IS_DBLBUF, 0, // nie potrzebujemy double buffering
    TAG_END
);

// po zrobieniu simpleBufferCreate()
bitmapLoadFromFile(s_pVpManager->pBack, "data/tlo1.bm", 0, 0); // wczytaj zawarto�� bg1.bm bezpo�rednio do bitmapy bufora ekranu, zaczynaj�c od pozycji 0,0
blitCopy(s_pHUD, 0, 224, s_pVpManager->pBack, 0, 224, 320,32,MINTERM_COOKIE, 0xFF);
joyOpen(); // b�dziemy u�ywa� d�oja w grze
keyCreate();
// na koniec create:
systemUnuse(); // system w trakcie loop nie jest nam potrzebny

printOnHUD();



viewLoad(s_pView);


// narysujmy falkona
blitCopyMask(s_pTiles, 128, 32, s_pVpManager->pBack, falkonx, falkony, 32, 32,(UWORD*)s_pTilesMask->Planes[0]);
//blitRect(s_pVpManager->pBack, falkonx, falkony, 32, 32, 1);

// tablica trzyma wlasciwosci tile'a, petla stawia tajle na miejscach wyznaczanych przez tablice
// level 1

kamyki[1][0] = 4;
kamyki[6][1] = 5;
kamyki[1][2] = 5;
kamyki[7][2] = 4;
kamyki[1][3] = 11;
kamyki[8][3] = 5;
kamyki[2][4] = 6;
kamyki[5][5] = 6;
kamyki[8][6] = 4;
kamyki[9][6] = 10;





drawTiles();
}

void genericProcess(void) {
  // Here goes code done each game frame



 //--------------------------------------------------------------- gdzie� w loop:
	joyProcess();
	keyProcess();
	if(joyUse(JOY1_FIRE) || keyUse(KEY_ESCAPE)) {
		gameExit();
	}

	kierunek = 0;
	if(joyUse(JOY1_RIGHT) || keyUse(KEY_D)) {
		kierunek = 1;
	}
	else if(joyUse(JOY1_LEFT) || keyUse(KEY_A)) {
		kierunek = 2;
	}
	else if(joyUse(JOY1_UP) || keyUse(KEY_W)) {
		kierunek = 3;
	}
	else if(joyUse(JOY1_DOWN) || keyUse(KEY_S)) {
		kierunek = 4;
	}

	if(kierunek != 0) {
		isThisStone();
		czyRamka();
		falconMove();
		coalAndCollect();
	}

	// noCoalLeft();

	viewProcessManagers(s_pView); // obliczenia niezb�dne do poprawnego dzia�ania viewport�w
	copProcessBlocks(); // obliczenia niezb�dne do poprawnego dzia�ania coppera
	vPortWaitForEnd(s_pVp); // r�wnowa�ne amosowemu wait vbl
}

void genericDestroy(void) {
  // Here goes your cleanup code
	systemUse(); // w��cz grzecznie system

	bitmapDestroy(s_pTiles);
  bitmapDestroy(s_pTilesMask);

  fontDestroy(s_pFont);
  fontDestroyTextBitMap(s_pBmText);

	viewDestroy(s_pView); // zwolnij z pami�ci view, wszystkie do��czone do niego viewporty i wszystkie do��czone do nich mened�ery
	joyClose();
	keyDestroy();
}
