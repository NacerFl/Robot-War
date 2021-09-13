#ifndef __WINDOW__H__
#define __WINDOW__H__

#include <ncurses.h>

typedef struct{
  WINDOW *arena;
  WINDOW *robotsCharacteristics;
  WINDOW *cycle;
} WIN;

#endif
