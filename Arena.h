#ifndef __ARENA__H__
#define __ARENA__H__

#include "Robot.h"
#include "Missile.h"
#include "Variables.h"

typedef struct{
  Robot *arene[ARENA_HEIGHT][ARENA_WIDTH];
  Missile *areneMissiles[ARENA_HEIGHT][ARENA_WIDTH];
} Arena;

#endif
