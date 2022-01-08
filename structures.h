#ifndef STRUCTURES_H
#define STRUCTURES_H

struct coords {
  UBYTE falkonx;
  UBYTE falkony;
  UBYTE krawedzx;
  UBYTE krawedzy;
  UWORD uwPosX;
  UWORD uwPosY;
  UBYTE tempX;
  UBYTE tempY;
  UBYTE portalX;
  UBYTE portalY;
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

};

struct doubleBuffering {
  UBYTE portalDB;
  UBYTE blueCapDB;
  UBYTE redCapDB;
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
