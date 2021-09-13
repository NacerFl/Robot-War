#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "Arena.h"
#include "Window.h"

Arena * init_arena(WIN *window){
  Arena *arena;

  //get screen size
  int xMax, yMax, i, j;
  getmaxyx(stdscr, yMax, xMax);

  //create window for arena
  window -> arena = newwin(ARENA_HEIGHT, ARENA_WIDTH, 0, (xMax/2) - (ARENA_WIDTH/2));
  refresh();

  //create box for arena
  box(window -> arena, 0, 0);

  //initialize arena
  arena = malloc(sizeof(Arena));

  if(arena == NULL){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  //set positions of robots
  for(i = 0; i < ARENA_HEIGHT; i++){
    for(j =0; j < ARENA_WIDTH; j++){
      arena -> arene[i][j] = NULL;
      arena -> areneMissiles[i][j] = NULL;
    }
  }

  return arena;
}
