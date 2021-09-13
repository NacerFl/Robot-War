#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <math.h>
#include <time.h>
#include "Robot.h"
#include "Arena.h"
#include "Window.h"

void initializeMemory(Robot **robots){
  int i, j;

  for(i = 0; i < 4; i++){
    for(j = 0; j < TAILLE_MEMOIRE; j++){
      robots[i] -> memoire[j] = NULL;
    }
  }
}

/*--------------------------------------------------------------------------------------*/

int exist(int *tab, int n){
  int i;

  for(i = 0; i < 5; i++){
    if(tab[i] == n){
      return 1;
    }
  }
  return 0;
}

/*--------------------------------------------------------------------------------------*/

void updatePositions(int positions, Robot *robot){
  if(positions == 0){
    robot -> xLocation = 1;
    robot -> yLocation = 1;
  }else if(positions == 1){
    robot -> xLocation = ARENA_WIDTH - 2;
    robot -> yLocation = 1;
  }else if(positions == 2){
    robot -> xLocation = ARENA_WIDTH - 2;
    robot -> yLocation = ARENA_HEIGHT - 2;
  }else{
    robot -> xLocation = 1;
    robot -> yLocation = ARENA_HEIGHT - 2;
  }
}

/*--------------------------------------------------------------------------------------*/

void collisionMissile(double x, double y, Arena *arena, WIN *window){
  int i, j;

  for(i = (int)y - 15; i < (int)y + 15; i++){
    for(j = (int)x - 15; j < (int)x + 15; j++){
      if(j <= ARENA_WIDTH - 2 && j >= 1 && i <= ARENA_HEIGHT - 2 && i >= 1){
        if(arena -> arene[i][j] != NULL){
          if(((int)fabs(i - y) + (int)fabs(j - x)) <= 1){ // missile exploded nearby
            if(arena -> arene[i][j] -> pdv < 100){
              if((arena -> arene[i][j] -> pdv + 10) >= 100){
                arena -> arene[i][j] -> vitesse = 0;
                arena -> arene[i][j] -> mort = 1;
                arena -> arene[i][j] -> pdv = 100;
              }else{
                arena -> arene[i][j] -> vitesse = 0;
                arena -> arene[i][j] -> pdv += 10;
              }
            }
          }else if(((int)fabs(i - y) + (int)fabs(j - x)) <= 4){ // missile exploded between 2 and 4 boxes
            if(arena -> arene[i][j] -> pdv < 100){
              if((arena -> arene[i][j] -> pdv + 5) >= 100){
                arena -> arene[i][j] -> vitesse = 0;
                arena -> arene[i][j] -> mort = 1;
                arena -> arene[i][j] -> pdv = 100;
              }else{
                arena -> arene[i][j] -> vitesse = 0;
                arena -> arene[i][j] -> pdv += 5;
              }
            }
          }else{
            if(((int)fabs(i - y) + (int)fabs(j - x)) <= 15){ // missile exploded between 5 and 15 boxes
              if(arena -> arene[i][j] -> pdv < 100){
                if((arena -> arene[i][j] -> pdv + 3) >= 100){
                  arena -> arene[i][j] -> vitesse = 0;
                  arena -> arene[i][j] -> mort = 1;
                  arena -> arene[i][j] -> pdv = 100;
                }else{
                  arena -> arene[i][j] -> vitesse = 0;
                  arena -> arene[i][j] -> pdv += 3;
                }
              }
            }
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------------------*/
Robot ** init_robots(Arena *arena, WIN *window){
  int i, j, x, y, random, position[4];
  Robot **robots;

  srand(time(NULL));
  i = 0;
  position[0] = 5;
  position[1] = 5;
  position[2] = 5;
  position[3] = 5;
  while(i != 4){
    while(position[i] == 5){
      random = rand() % 4;
      if(exist(position, random) != 1){
        position[i] = random;
      }else{
        random = rand() % 4;
      }
    }
    i++;
  }

  robots = malloc(sizeof(Robot *) * 4);

  if(robots == NULL){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  for(i = 0; i < 4; i++){
    robots[i] = malloc(sizeof(Robot));

    if(robots[i] == NULL){
      perror("malloc");
      exit(EXIT_FAILURE);
    }

    robots[i] -> id = i;
    robots[i] -> pdv = 0;
    robots[i] -> vitesse = 0;
    robots[i] -> angle = 0;
    robots[i] -> mort = 0;
    robots[i] -> missile_a_disposition = MISSILE_MAX;
    if(i == 0){
      robots[i] -> name = 'A';
      updatePositions(position[0], robots[i]);
    }else if(i == 1){
      robots[i] -> name = 'B';
      updatePositions(position[1], robots[i]);
    }else if(i == 2){
      robots[i] -> name = 'C';
      updatePositions(position[2], robots[i]);
    }else{
      robots[i] -> name = 'D';
      updatePositions(position[3], robots[i]);
    }
    x = robots[i] -> xLocation;
    y = robots[i] -> yLocation;
    arena -> arene[y][x] = robots[i];
    mvwprintw(window -> arena, y, x, "%c", robots[i] -> name);
  }
  wrefresh(window -> arena);
  initializeMemory(robots);

  return robots;
}

/*--------------------------------------------------------------------------------------*/

void engine(Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window, double angle, double speed){
  int x, y, i;

  if(robot -> mort == 0 && speed > 0){
    //x = robot -> xLocation + round(((speed / 100) * VITESSE_MAX_ROBOT * cos(angle * -1 * M_PI/180)));
    //y = robot -> yLocation + round(((speed / 100) * VITESSE_MAX_ROBOT * sin(angle * - 1 * M_PI/ 180)));
    x = round((robot -> xLocation + (speed/100) * VITESSE_MAX_ROBOT * cos(angle * -1 * M_PI/180)));
    y = round((robot -> yLocation + (speed/100) * VITESSE_MAX_ROBOT * sin(angle * -1 * M_PI/180)));
    if(x > ARENA_WIDTH - 2 || arena -> areneMissiles[y][x] != NULL || x < 1 || y > ARENA_HEIGHT - 2 || y < 1){ // bumps into a wall
      if(arena -> areneMissiles[y][x] != NULL){
        for(i = 0; i < 4; i++){
          if(robots[i] -> id == arena -> areneMissiles[y][x] -> id){
            robots[i] -> missile_a_disposition += 1;
            for(i = 0; i < 8; i++){
              if(missiles[i] != NULL && (int)(missiles[i] -> xLocation) == x && (int)(missiles[i] -> yLocation) == y){
                free(missiles[i]);
                missiles[i] = NULL;
                mvwprintw(window -> arena, y, x, " ");
	            wrefresh(window -> arena);
	            arena -> areneMissiles[y][x] = NULL;
	            collisionMissile(x, y, arena, window);
                return;
              }
            }
          }
        }
      }else{
        if((robot -> pdv + 2) >= 100){
          robot -> vitesse = 0;
          robot -> mort = 1;
          robot -> pdv = 100;
        }else{
          robot -> pdv += 2;
          robot -> vitesse = 0;
        }
      }
    }else if(arena -> arene[y][x] != NULL){ // bumps into a robot
      if((robot -> pdv + 2) >= 100){
        robot -> vitesse = 0;
        robot -> mort = 1;
        robot -> pdv = 100;
      }else{
        robot -> vitesse = 0;
        robot -> pdv += 2;
      }
      if(arena -> arene[y][x] -> pdv < 100){
        if((arena -> arene[y][x] -> pdv + 2) >= 100){
          arena -> arene[y][x] -> vitesse = 0;
          arena -> arene[y][x] -> mort = 1;
          arena -> arene[y][x] -> pdv = 100;
        }else{
          arena -> arene[y][x] -> pdv += 2;
        }
      }
    }else{ // nothing in front of him
      arena -> arene[y][x] = robot;
      int pos_x_robot = robot -> xLocation;
      int pos_y_robot = robot -> yLocation;
      arena -> arene[pos_y_robot][pos_x_robot] = NULL;
      mvwprintw(window -> arena, y, x, "%c", robot -> name);
      mvwprintw(window -> arena, pos_y_robot, pos_x_robot, " ");
      wrefresh(window -> arena);
      robot -> xLocation = (double)x;
      robot -> yLocation = (double)y;
      robot -> vitesse = speed;
      robot -> angle = angle;
    }
  }
}

/*--------------------------------------------------------------------------------------*/
/*
void updateEngine(Robot **robots, Arena *arena, WIN *window){
  int x, y, i;

  for(i = 0; i < 4; i++){
    if(robots[i] -> mort == 0){
      //x = robots[i] -> xLocation + round(((robots[i] -> vitesse / 100) * VITESSE_MAX_ROBOT * cos(robots[i] -> angle * -1 * M_PI/180)));
      //y = robots[i] -> yLocation + round(((robots[i] -> vitesse / 100) * VITESSE_MAX_ROBOT * sin(robots[i] -> angle * - 1 * M_PI/ 180)));
      x = round((robots[i] -> xLocation + (robots[i] -> vitesse/100) * VITESSE_MAX_ROBOT * cos(robots[i] -> angle * -1 * M_PI/180)));
      y = round((robots[i] -> yLocation + (robots[i] -> vitesse/100) * VITESSE_MAX_ROBOT * sin(robots[i] -> angle * -1 * M_PI/180)));
      if(x > ARENA_WIDTH - 2 || x < 1 || y > ARENA_HEIGHT - 2 || y < 1){ // bumps into a wall
        robots[i] -> pdv += 2;
        if(robots[i] -> pdv >= 100){
          robots[i] -> vitesse = 0;
          robots[i] -> mort = 1;
        }else{
          robots[i] -> vitesse = 0;
        }
      }else if(arena -> arene[y][x] != NULL){ // bumps into a robot
        robots[i] -> pdv += 2;
        if(robots[i] -> pdv >= 100){
          robots[i] -> vitesse = 0;
          robots[i] -> mort = 1;
        }else{
          robots[i] -> vitesse = 0;
        }
        arena -> arene[y][x] -> pdv += 2;
        if(arena -> arene[y][x] -> pdv >= 100){
          arena -> arene[y][x] -> vitesse = 0;
          arena -> arene[y][x] -> mort = 1;
        }
      }else{ // nothing in front of him
        arena -> arene[y][x] = robots[i];
        int pos_x = robots[i] -> xLocation;
        int pos_y = robots[i] -> yLocation;
        arena -> arene[y][x] = NULL;
        mvwprintw(window -> arena, y, x, "%c", robots[i] -> name);
        mvwprintw(window -> arena, robots[i] -> yLocation, robots[i] -> xLocation, " ");
        wrefresh(window -> arena);
        robots[i] -> xLocation = (double)x;
        robots[i]-> yLocation = (double)y;
      }
    }
  }
}*/