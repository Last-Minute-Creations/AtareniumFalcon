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

struct animStateControls { 
  BOOL falkonIdleControl; 
  UBYTE falconFlyingAnim; 
  BOOL stonehitAnimControl; 
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

  UBYTE stonehitAnimTick;
  UBYTE stonehitAnimFrame;

  UBYTE flyingTick;
  UBYTE flyingFrame;

  UBYTE falkonIdle;
  UBYTE idleFrame;
  UBYTE falkonIdleTempo;
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