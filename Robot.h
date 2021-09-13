#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "Variables.h"

typedef struct{
  char name;
  int id;
  int pdv;
  int missile_a_disposition;
  double xLocation;
  double yLocation;
  int mort;
  double vitesse;
  double angle;
  int *memoire[TAILLE_MEMOIRE];
} Robot;

#endif
