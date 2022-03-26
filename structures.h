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
   /*
  UBYTE portalTempo; 
  UBYTE redCapacitorTempo; 
  UBYTE blueCapacitorTempo; 
  UBYTE falconTick; 
  UBYTE falconTempo; 
  UBYTE falconFrame; 
   
   
  UBYTE flyingTempo; 
  */
}; 

 /*
struct coords { 
  UBYTE falconFace; 
  UBYTE falkonx; 
  UBYTE falkony; 
  BYTE krawedzx; 
  BYTE krawedzy; 
  UWORD uwPosX; 
  UWORD uwPosY; 
  UBYTE targetTileX; 
  UBYTE targetTileY; 
  UBYTE portalX; 
  UBYTE portalY; 
  UBYTE robboX; 
  UBYTE robboY; 
  UBYTE uwPreviousX; 
  UBYTE uwPreviousY; 
  UBYTE newPosX; 
  UBYTE newPosY; 
}; 
 

 
struct animStateControls { 
  BOOL falconIdle; 
  UBYTE falconFlyingAnim; 
  UBYTE stoneHitAnim; 
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
 
struct robboMsg { 
  char *sz1stLine; 
  char szText[80]; 
  char *szCollision1stLine; 
  char *szCollision2ndLine; 
}; */
 
 
 
 
#endif // STRUCTURES_H 