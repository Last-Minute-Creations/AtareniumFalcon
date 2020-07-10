#include <ace/generic/main.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/system.h>
#include <ace/managers/game.h>
#include <ace/utils/palette.h>

//------------------------------------------------------- gdzie� przed funkcjami
// zmienne trzymaj�ce adresy do viewa, viewporta, simple buffer managera
static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;
static tBitMap *s_pTiles;


BYTE kamyki[9][6];

// coordsy do rysowania falkona i kontrolowania zeby sie nie wypierdolil za ekran
BYTE falkonx = 0;
BYTE falkony = 0;
BYTE krawedzx = 0;
BYTE krawedzy = 0;
BYTE kierunek = 0;

BYTE stoneHit = 0;
BYTE coal = 10;
BYTE capacitors = 0;


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
  }
  if(kamyki[pickSthX][pickSthY] == 9){
    capacitors = capacitors + 2;
  }
  if(kamyki[pickSthX][pickSthY] == 10){
    gameExit();
  }

  coal = coal - 1;
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
      blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 0);
      falkonx = falkonx + 1;
      blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 1);
        break;
      case 2:
      blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 0);
      falkonx = falkonx - 1;
      blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 1);
        break;
      case 3:
      blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 0);
      falkony = falkony - 1;
      blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 1);
        break;
      case 4:
      blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 0);
      falkony = falkony + 1;
      blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 1);
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

// proste wy�wietlanie bitmapy na viewporcie
s_pVpManager = simpleBufferCreate(0,
    TAG_SIMPLEBUFFER_VPORT, s_pVp, // parent viewport
    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED, // wst�pne czyszczenie bitmapy, przyspieszenie rysowania grafiki
    TAG_SIMPLEBUFFER_IS_DBLBUF, 0, // nie potrzebujemy double buffering
    TAG_END
);

// po zrobieniu simpleBufferCreate()
bitmapLoadFromFile(s_pVpManager->pBack, "data/tlo1.bm", 0, 0); // wczytaj zawarto�� bg1.bm bezpo�rednio do bitmapy bufora ekranu, zaczynaj�c od pozycji 0,0

joyOpen(); // b�dziemy u�ywa� d�oja w grze
keyCreate();
// na koniec create:
systemUnuse(); // system w trakcie loop nie jest nam potrzebny
viewLoad(s_pView);


// narysujmy prostok�t
blitRect(s_pVpManager->pBack, falkonx, falkony, 32, 32, 1);

// stawiam recznie kamyki i znajdzki, uzupelniam tablice do oznaczenia rodzaju tajla
blitRect(s_pVpManager->pBack, 1 * 32, 0 * 32, 32, 32, 4);
kamyki[1][0] = 4;
blitRect(s_pVpManager->pBack, 6 * 32, 1 * 32, 32, 32, 5);
kamyki[6][1] = 5;
blitRect(s_pVpManager->pBack, 1 * 32, 2 * 32, 32, 32, 5);
kamyki[1][2] = 5;
blitRect(s_pVpManager->pBack, 7 * 32, 2 * 32, 32, 32, 4);
kamyki[7][2] = 4;
blitRect(s_pVpManager->pBack, 1 * 32, 3 * 32, 32, 32, 11);
kamyki[1][3] = 11;
blitRect(s_pVpManager->pBack, 8 * 32, 3 * 32, 32, 32, 5);
kamyki[8][3] = 5;
blitRect(s_pVpManager->pBack, 2 * 32, 4 * 32, 32, 32, 6);
kamyki[2][4] = 6;
blitRect(s_pVpManager->pBack, 5 * 32, 5 * 32, 32, 32, 6);
kamyki[5][5] = 6;
blitRect(s_pVpManager->pBack, 8 * 32, 6 * 32, 32, 32, 4);
kamyki[8][6] = 4;
blitRect(s_pVpManager->pBack, 9 * 32, 6 * 32, 32, 32, 10);
kamyki[9][6] = 10;



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

	viewDestroy(s_pView); // zwolnij z pami�ci view, wszystkie do��czone do niego viewporty i wszystkie do��czone do nich mened�ery
	joyClose();
	keyDestroy();
}
