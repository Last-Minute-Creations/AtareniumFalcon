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

// coordsy do rysowania falkona i kontrolowania zeby sie nie wypierdolil za ekran
int falkonx = 0;
int falkony = 0;
int krawedzx = 0;
int krawedzy = 0;
int kierunek = 0;



void czyramka(void) {
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
        if(krawedzy == 8){
          krawedzy =7;
          falkony = 6;
        }  



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
    czyramka();
    falkonx = falkonx + 1;
 blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 1);   
}
if(joyUse(JOY1_LEFT)) {
    kierunek = 2;
    czyramka();
    falkonx = falkonx - 1;
 blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 1);   
}
if(joyUse(JOY1_UP)) {
    kierunek = 3;
    czyramka();
    falkony = falkony - 1;
 blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 1);   
}
if(joyUse(JOY1_DOWN)) {
    kierunek = 4;
    czyramka();
    falkony = falkony + 1;
 blitRect(s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, 1);   
}




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