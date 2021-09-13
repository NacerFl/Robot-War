#ifndef __MAIN__H__
#define __MAIN__H__

#include "Window.h"
#include "Robot.h"
#include "Arena.h"
#include "Missile.h"


/* PROTOTYPES */

WIN * init_WIN();
void cleanWindow(WIN *window);
void init_cycle(WIN *window);
void refreshCycle(WIN *window, int cycle);
Arena * init_arena(WIN *window);
Robot ** init_robots(Arena *arena, WIN *window);
void initializeMemory(Robot **robots);
int exist(int *tab, int n);
void updatePositions(int positions, Robot *robot);
void engine(Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window, double angle, double speed);
void collisionMissile(double x, double y, Arena *arena, WIN *window);
Missile ** init_missiles();
void shoot(Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window, double angle, double distance);
void updateShoot(Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window);
void explode(double x, double y, Arena *arena, WIN *window);
void refreshRobotCharacteristics(Robot **robots, WIN *window);
int endGame(Arena *arena);
void init_robotCharacteristics(WIN *window);
char ** getRobotScript(char *robotScript, int * nbrLines);
void parse(char *line, Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window, int *nextLine, int *delay);

#endif
