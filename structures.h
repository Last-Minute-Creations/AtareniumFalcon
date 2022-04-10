#ifndef STRUCTURES_H 
#define STRUCTURES_H 
 
struct collected { 
  UBYTE coal; 
  UBYTE excesscoal; 
  UBYTE capacitors; 
  UBYTE robboMsgCount; 
};

struct moveControls { 
  BOOL stoneHit; 
  BOOL frameHit; 
  UBYTE anotherHit; // sprawdzam czy po uderzeniu w kamien chce jeszcze raz, zeby sie HUD 2 razy nie rozwijal 
  UBYTE kierunek;  
  UBYTE kierunekHold; 
}; 

struct coordinates { // coordsy do rysowania falkona i kontrolowania zeby sie nie wypierdolil za ekran
  UBYTE falkonx;
  UBYTE falkony; // pozycja falkona na tablicy z tilesami 32x32
  UBYTE falkonFace; // kierunek dziobem
  BYTE krawedzx; // do sprawdzania czy chcemy wyleciec za ekran, BYTE - ze znakiem bo moze byc -1
  BYTE krawedzy;
};

struct animStateControls { 
  BOOL falkonIdleControl; 
  UBYTE flyingAnimControl; 
  BOOL stonehitAnimControl;
  BOOL hudScrollingControl; 
}; 

struct anim { 
  UBYTE robboFrame;
  UBYTE robboTick;
  UBYTE robboTempo;

  UBYTE portalGlowTick;
  UBYTE portalGlowFrame; 
  UBYTE portalGlowX;
  UBYTE portalGlowY;

  UBYTE redCapacitorsAnimTileCheck;
  UBYTE redCapacitorsAnimTick;
  UBYTE blueCapacitorsAnimTileCheck; 
  UBYTE blueCapacitorsAnimTick;
  UBYTE tickTempo;

  UBYTE stonehitAnimTick;
  UBYTE stonehitAnimFrame;

  UBYTE flyingTick;
  UBYTE flyingFrame;

  UBYTE falkonIdle;
  UBYTE idleFrame;
  UBYTE falkonIdleTempo;

  UBYTE hudScrollingTick; // scrolling up/down on Robbo & ESP tick

  UBYTE hudTickTempo;  // amiga mode hud animation variables
  UBYTE hudAnimTick;
  UBYTE hudTickFrame;
}; 

struct flyingAnim { 
  // bad naming, but these are variables for drawing flying animation
  // frame by frame moving per pixel:
  // drawing Background on previous position, then
  // drawing animation tile on the new position
  UWORD uwPosX;
  UWORD uwPosY;
  UWORD uwPreviousX;
  UWORD uwPreviousY;
  UWORD newPosX;
  UWORD newPosY;
  UWORD HitPosX;
  UWORD HitPosY;
  UBYTE tempX;   // remember to use meaningful names next time ! !
  UBYTE tempY;   // lesson learned ... 

};


 /*
struct coords { 
  UBYTE falconFace; 
  UBYTE falkonx; 
  UBYTE falkony; 
  BYTE krawedzx; 
  BYTE krawedzy; 
  UBYTE targetTileX; 
  UBYTE targetTileY; 
  UBYTE portalX; 
  UBYTE portalY; 
  UBYTE robboX; 
  UBYTE robboY; 
}; 
 

 

  
 
struct doubleBuffering { 
  UBYTE portal; 
  UBYTE blueCap; 
  UBYTE redCap; 
  UBYTE robbo; 
  UBYTE flyingAnimFrame; 
}; 
 
struct hud { 
  char fuelCoalDisplay[50];   
  char capacitorsDisplay[50];  
  char excessCoalDisplay[50]; 
  char robboCountDisplay[50]; 
  char levelNumber[50];   // test-only display level number on hud botom right corner 
  UBYTE fontColor;  // 23 
  UBYTE stateCtrl; 
}; 
 
 */
 
 
 
 
#endif // STRUCTURES_H 