#ifndef __BURP__H__
#define __BURP__H__

  #include "Variables.h"
  #include "Missile.h"
  #include "Arena.h"
  #include "Window.h"
  #include "Robot.h"

  void yyerror(char *line, Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window, int *nextLine, int *delay, char *s);
  int egalite(int x, int y);
  int different(int x, int y);
  int  inferieur(int x, int y);
  int inferieurEgale(int x, int y);
  int  superieur(int x, int y);
  int superieurEgale(int x, int y);
  int ajouter(int x, int y);
  int multiplier(int x, int y);
  int difference(int x, int y);
  int division(int x, int y);
  int modulo(int x, int y);
  double angle(double x1, double y1, double x2, double y2);
  double targetx(double x1, double angle, double length);
  double targety(double y1, double angle, double length);
  double distance(double x1, double y1, double x2, double y2);
  void wait(int delay);
  void poke(int *adresse, int value);
  int peek(int *adresse);
  int ranD(int num);
  int state(Robot **robots, int num);
  int gpsx(Robot **robots, int num);
  int gpsy(Robot **robots, int num);
  
#endif
