#include <ace/generic/main.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/joy.h>
#include <ace/managers/system.h>
#include <ace/managers/game.h>

//------------------------------------------------------- gdzie˜ przed funkcjami
// zmienne trzymaj¥ce adresy do viewa, viewporta, simple buffer managera
static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;

BYTE kamyki[6][6];

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
    // tu bedzie funkcja sprawdzajaca czy sie chcemy wypierdolic za ekran
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
  BYTE pickSthX = 0;
  BYTE pickSthY = 0;
  
  
  pickSthX = falkonx;
  pickSthY = falkony;
  
  if(kamyki[pickSthX][pickSthY] == 4){
    coal = coal + 2;
  } 


  coal = coal - 1;
}

void falconMove(void){
    if(stoneHit == 1){
      stoneHit = 0;
      return;
    }

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
  if(coal == 0){
    gameExit();
  }
}


void genericCreate(void) {
  // Here goes your startup code
  //-------------------------------------------------------------- gdzie˜ w create
s_pView = viewCreate(0,
    TAG_VIEW_COPLIST_MODE, COPPER_MODE_BLOCK, // spos¢b u¾ywania coppera - ten jest najprostszy bo nic z nim nie musisz wi©cej robi†
    TAG_VIEW_GLOBAL_CLUT, 1, // globalna paleta dla wszystkich viewport¢w
    TAG_END // wi©cej argument¢w nie ma
);

s_pVp = vPortCreate(0,
    TAG_VPORT_VIEW, s_pView, // parent view
    TAG_VPORT_BPP, 5, // bits per pixel: 4bpp = 16col, 5pp = 32col, etc.
    TAG_END
);
// Jaka˜ paleta na pocz¥tek
s_pVp->pPalette[0] = 0x0000; // 0x0RGB, hex (0..F)
s_pVp->pPalette[1] = 0x0888;
s_pVp->pPalette[2] = 0x0800;
s_pVp->pPalette[3] = 0x0008;

// proste wy˜wietlanie bitmapy na viewporcie
s_pVpManager = simpleBufferCreate(0,
    TAG_SIMPLEBUFFER_VPORT, s_pVp, // parent viewport
    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED, // wst©pne czyszczenie bitmapy, przyspieszenie rysowania grafiki
    TAG_SIMPLEBUFFER_IS_DBLBUF, 0, // nie potrzebujemy double buffering
    TAG_END
);
joyOpen(); // b©dziemy u¾ywa† d¾oja w grze
// na koniec create:
systemUnuse(); // system w trakcie loop nie jest nam potrzebny
viewLoad(s_pView);

// narysujmy prostok¥t
blitRect(s_pVpManager->pBack, falkonx, falkony, 32, 32, 1);

// drugi jako kamyk
blitRect(s_pVpManager->pBack, 2 * 32, 4 * 32, 32, 32, 2);
kamyki[2][4] = 3;
blitRect(s_pVpManager->pBack, 3 * 32, 2 * 32, 32, 32, 2);
kamyki[3][2] = 3;

blitRect(s_pVpManager->pBack, 5 * 32, 5 * 32, 32, 32, 3);
kamyki[5][5] = 4;

}

void genericProcess(void) {
  // Here goes code done each game frame
 


 //--------------------------------------------------------------- gdzie˜ w loop:
joyProcess();
if(joyUse(JOY1_FIRE)) {
  gameExit();
}
if(joyUse(JOY1_RIGHT)) {
    kierunek = 1; 
    isThisStone();
    czyRamka();
    falconMove(); 
    coalAndCollect(); 
}  
if(joyUse(JOY1_LEFT)) {
    kierunek = 2; 
    isThisStone();
    czyRamka();
    falconMove(); 
    coalAndCollect();
}
if(joyUse(JOY1_UP)) {
    kierunek = 3; 
    isThisStone();
    czyRamka();
    falconMove(); 
    coalAndCollect();    
}
if(joyUse(JOY1_DOWN)) {
    kierunek = 4; 
    isThisStone();
    czyRamka();
    falconMove(); 
    coalAndCollect();   
}

noCoalLeft();


viewProcessManagers(s_pView); // obliczenia niezb©dne do poprawnego dziaˆania viewport¢w
copProcessBlocks(); // obliczenia niezb©dne do poprawnego dziaˆania coppera
vPortWaitForEnd(s_pVp); // r¢wnowa¾ne amosowemu wait vbl
}

void genericDestroy(void) {
  // Here goes your cleanup code
  //------------------------------------------------------------ gdzie˜ w destroy:
// na pocz¥tku destroy:
systemUse(); // wˆ¥cz grzecznie system

viewDestroy(s_pView); // zwolnij z pami©ci view, wszystkie doˆ¥czone do niego viewporty i wszystkie doˆ¥czone do nich mened¾ery
joyClose();
}