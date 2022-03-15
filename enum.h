typedef enum {
  MUSIC_OFF = 0,
  MUSIC_HEAVY = 1,
  MUSIC_AMBIENT_SFX = 2
} tMusicState;

typedef enum {
  AMIGA_MODE_OFF = 0,
  AMIGA_MODE_ON = 1,
  AMIGA_MODE_CHECK = 2,
} tAmigaMode;

typedef enum {
  LEVEL_SCORE_OFF = 0,
  LEVEL_SCORE_COUNT = 1,
  LEVEL_SCORE_PORTAL_OPEN = 2,
  LEVEL_SCORE_PORTAL_ANIM = 3,
  LEVEL_SCORE_PORTAL_CLOSE = 4,
  LEVEL_SCORE_END = 5,
  LEVEL_SCORE_NOCOAL = 6
} tEndLevelState;

typedef enum {
  METEORITE = '3',
  COAL_2 = '4',
  COAL_3 = '5',
  COAL_4 = '6',
  COAL_5 = '7',
  CAPACITOR_BLUE = '8',
  CAPACITOR_RED = '9',
  PORTAL = 'E',
  ROBBO = 'R',
  CAPACITOR_BROKEN = 'B',
  SABERMANS_GUMBOOT = 'S',
  FALCON_START_POSITION = '1',
  EMPTY_TILE = '0' 
} tDrawingTilesetElements;