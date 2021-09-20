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
  LEVEL_SCORE_END = 5
} tEndLevelState;