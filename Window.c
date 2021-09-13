#include <stdlib.h>
#include <stdio.h>
#include "Window.h"
#include "Robot.h"

WIN * init_WIN(){

  WIN *window = malloc(sizeof(WIN));

  if(window == NULL){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  return window;
}

/*--------------------------------------------------------------------------------------*/

void init_robotCharacteristics(WIN *window){

  //get screen size
  int xMax, yMax, i, j;
  getmaxyx(stdscr, yMax, xMax);

  //create window for robots characteristics
  window -> robotsCharacteristics = newwin(ROBOTS_CHARACTERISTICS_HEIGHT, ROBOTS_CHARACTERISTICS_WIDTH, ARENA_HEIGHT, (xMax/2) - (ARENA_WIDTH/2));
  refresh();

  //create box for robots charateristics
  box(window -> robotsCharacteristics, 0, 0);
}

/*--------------------------------------------------------------------------------------*/

void init_cycle(WIN *window){
  //get screen size
  int xMax, yMax, i, j;
  getmaxyx(stdscr, yMax, xMax);

  //create window for robots characteristics
  window -> cycle = newwin(3, ROBOTS_CHARACTERISTICS_WIDTH, ARENA_HEIGHT + ROBOTS_CHARACTERISTICS_HEIGHT, (xMax/2) - (ARENA_WIDTH/2));
  refresh();

  //create box for robots charateristics
  box(window -> cycle, 0, 0);
  wrefresh(window -> cycle);
}
/*--------------------------------------------------------------------------------------*/

void refreshRobotCharacteristics(Robot **robots, WIN *window){
  int position, i;

  for(i = 0; i < 4; i++){
    if(robots[i] -> id == 0){
      position = 1;
    }else if(robots[i] -> id == 1){
      position = ROBOTS_CHARACTERISTICS_WIDTH / 4;
    }else if(robots[i] -> id == 2){
      position = (ROBOTS_CHARACTERISTICS_WIDTH / 4)*2;
    }else{
      position = (ROBOTS_CHARACTERISTICS_WIDTH / 4)*3;
    }
    if(position == 1){
      mvwprintw(window -> robotsCharacteristics, 1, position, "ROBOT %d", position);
    }else{
      mvwprintw(window -> robotsCharacteristics, 1, position, "ROBOT %d", (position / (ROBOTS_CHARACTERISTICS_WIDTH / 4)) + 1);
    }
    mvwprintw(window -> robotsCharacteristics, 3, position, "NAME : %c", robots[i] -> name);
    mvwprintw(window -> robotsCharacteristics, 4, position, "LIFE POINT : %d", robots[i] -> pdv);
    mvwprintw(window -> robotsCharacteristics, 5, position, "VITESSE : %d", (int)(robots[i] -> vitesse));
    mvwprintw(window -> robotsCharacteristics, 6, position, "X LOCATION : %d", (int)(robots[i] -> xLocation));
    mvwprintw(window -> robotsCharacteristics, 7, position, "Y LOCATION : %d", (int)(robots[i] -> yLocation));
  }
  wrefresh(window -> robotsCharacteristics);
}

/*--------------------------------------------------------------------------------------*/

void refreshCycle(WIN *window, int cycle){
  mvwprintw(window -> cycle, 1, 1, "CYCLE %d", cycle);
  wrefresh(window -> cycle);
}

/*--------------------------------------------------------------------------------------*/
void cleanWindow(WIN *window){
  //get screen size
  int xMax, yMax, i, j;
  getmaxyx(stdscr, yMax, xMax);

  for(i = 0; i < yMax; i++){
    for(j = 0; j < xMax; j++){
      mvprintw(i, j, " ");
    }
  }
}