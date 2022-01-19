#ifndef STRUCTURES_H
#define STRUCTURES_H

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

struct anim {
  UBYTE portalFrame;
  UBYTE portalTick;
  UBYTE portalTempo;
  UBYTE redCapacitorTick;
  UBYTE redCapacitorTempo;
  UBYTE redCapacitorTileCheck;
  UBYTE blueCapacitorTick;
  UBYTE blueCapacitorTempo;
  UBYTE blueCapacitorTileCheck;
  UBYTE robboFrame;
  UBYTE robboTick;
  UBYTE robboTempo;
  UBYTE falconTick;
  UBYTE falconTempo;
  UBYTE falconFrame;
  UBYTE flyingTick;
  UBYTE flyingFrame;
  UBYTE flyingTempo;
};

struct animStateControls {
  BOOL falconIdle;
  UBYTE falconFlyingAnim;
  UBYTE stoneHitAnim;
};

struct moveControls {
  BOOL stoneHit;
  BOOL frameHit;
  UBYTE kierunek; 
  UBYTE kierunekHold;
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
};

struct collected {
  UBYTE coal;
  UBYTE excessCoal;
  UBYTE capacitors;
  UBYTE robbo;
};



#endif // STRUCTURES_H
