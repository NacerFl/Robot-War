#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <math.h>
#include "Robot.h"
#include "Missile.h"
#include "Arena.h"
#include "Window.h"

Missile ** init_missiles(){
  Missile **missiles;
  int i;

  missiles = malloc(sizeof(Missile *) * 8);

  if(missiles == NULL){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  for(i = 0; i < 8; i++){
    missiles[i] = NULL;
  }

  return missiles;
}

/*--------------------------------------------------------------------------------------*/

void explode(double x, double y, Arena *arena, WIN *window){
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

void shoot(Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window, double angle, double distance){
  int x, y, i, j;

  if(robot -> mort == 0 && robot -> missile_a_disposition > 0){
    //x = robot -> xLocation + round(((10 / 100) * 100 * cos(angle * -1 * M_PI/180)));
    //y = robot -> yLocation + round(((10 / 100) * 100 * sin(angle * - 1 * M_PI/ 180)));
    x = round((robot -> xLocation + (1.0/100) * VITESSE_MAX_MISSILE * cos(angle * -1 * M_PI/180)));
    y = round((robot -> yLocation + (1.0/100) * VITESSE_MAX_MISSILE * sin(angle * -1 * M_PI/180)));
    if(arena -> arene[y][x] != NULL || arena -> areneMissiles[y][x] != NULL || x > ARENA_WIDTH - 2 || x < 1 || y > ARENA_HEIGHT - 2 || y < 1){ // bumps into a robot or a wall or a missile
    	explode(x, y, arena, window);
    	if(arena -> areneMissiles[y][x] != NULL){
    		for(i = 0; i < 4; i++){
	      		if(robots[i] -> id == arena -> areneMissiles[y][x] -> id){
	      			robots[i] -> missile_a_disposition += 1;
	      			for(i = 0; i < 8; i++){
	      				if((int)(missiles[i] -> xLocation) == x && (int)(missiles[i] -> yLocation) == y){
	      					free(missiles[i]);
	      					missiles[i] = NULL;
	      					mvwprintw(window -> arena, y, x, " ");
			      			wrefresh(window -> arena);
			      			arena -> areneMissiles[y][x] = NULL;
	      					return;
	      				}
	      			}
	      		}
	      	}
	    }
    }else{ // nothing in front of him
      for(i = 0; i < 8; i++){
        if(missiles[i] == NULL){
          missiles[i] = malloc(sizeof(Missile));

          if(missiles[i] == NULL){
            perror("malloc");
            exit(EXIT_FAILURE);
          }

          missiles[i] -> id = robot -> id;
          missiles[i] -> xLocation = (double)x;
          missiles[i] -> yLocation = (double)y;
          missiles[i] -> angle = angle;
          missiles[i] -> distance = distance - 1;

          robot -> missile_a_disposition -= 1;
          arena -> areneMissiles[y][x] = missiles[i];
	      mvwprintw(window -> arena, y, x, "X");
	      if((int)distance <= 1){
	        explode(x, y, arena, window);
	        mvwprintw(window -> arena, y, x, " ");
	        free(missiles[i]);
	        missiles[i] = NULL;
	        robot -> missile_a_disposition += 1;
	        arena -> areneMissiles[y][x] = NULL;
	      }
	      wrefresh(window -> arena);
          return;
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------------------*/

void updateShoot(Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window){
  int x, y, i, j, pos_x_missile, pos_y_missile;

  for(i = 0; i < 8; i++){
    if(missiles[i] != NULL){
      x = round((missiles[i] -> xLocation + (1.0/100) * VITESSE_MAX_MISSILE * cos(missiles[i] -> angle * -1 * M_PI/180)));
      y = round((missiles[i] -> yLocation + (1.0/100) * VITESSE_MAX_MISSILE * sin(missiles[i] -> angle * -1 * M_PI/180)));
      if(arena -> arene[y][x] != NULL || arena -> areneMissiles[y][x] != NULL || x > ARENA_WIDTH - 2 || x < 1 || y > ARENA_HEIGHT - 2 || y < 1){ // bumps into a robot or a wall or a missile
        explode(x, y, arena, window);
        pos_x_missile = missiles[i] -> xLocation;
        pos_y_missile = missiles[i] -> yLocation;
        arena -> areneMissiles[pos_y_missile][pos_x_missile] = NULL;
        mvwprintw(window -> arena, pos_y_missile, pos_x_missile, " ");
        wrefresh(window -> arena);
        for(j = 0; j < 4; j++){
          if(robots[j] -> id == missiles[i] -> id){
            robots[j] -> missile_a_disposition += 1;
          }
        }
        if(arena -> areneMissiles[y][x] != NULL){
    		for(j = 0; j < 4; j++){
	      		if(robots[j] -> id == arena -> areneMissiles[y][x] -> id){
	      			robots[j] -> missile_a_disposition += 1;
	      			for(j = 0; j < 8; j++){
	      				if(missiles[j] != NULL && (int)(missiles[j] -> xLocation) == x && (int)(missiles[j] -> yLocation) == y){
	      					free(missiles[j]);
	      					missiles[j] = NULL;
	      					arena -> areneMissiles[y][x] = NULL;
			      			mvwprintw(window -> arena, y, x, " ");
			      			wrefresh(window -> arena);
	      					return;
	      				}
	      			}
	      		}
	      	}
	    }else{
	    	free(missiles[i]);
        	missiles[i] = NULL;
	    }
      }else{ // nothing in front of him
        mvwprintw(window -> arena, y, x, "X");
        pos_x_missile = missiles[i] -> xLocation;
        pos_y_missile = missiles[i] -> yLocation;
        mvwprintw(window -> arena, pos_y_missile, pos_x_missile, " ");
        missiles[i] -> xLocation = (double)x;
        missiles[i] -> yLocation = (double)y;
        missiles[i] -> distance -= 1;
        arena -> areneMissiles[pos_y_missile][pos_x_missile] = NULL;
        arena -> areneMissiles[y][x] = missiles[i];
        if(missiles[i] -> distance <= 0){
          mvwprintw(window -> arena, y, x, " ");
          explode(x, y, arena, window);
          for(j = 0; j < 4; j++){
            if(robots[j] -> id == missiles[i] -> id){
              robots[j] -> missile_a_disposition += 1;
            }
          }
          free(missiles[i]);
          missiles[i] = NULL;
          arena -> areneMissiles[y][x] = NULL;
        }
        wrefresh(window -> arena);
      }
    }
  }
}