%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
  #include <time.h>
  #include "BURP.h"
  #include "main.h"
  #include "Variables.h"
  #include "Missile.h"
  #include "Arena.h"
  #include "Window.h"
  #include "Robot.h"

  int yydebug = 0;
  int yylex();
%}

%parse-param {char * ligne} {Robot **robots} {Robot *robot} {Missile **missiles} {Arena *arena} {WIN *window} {int *nextLine} {int *delay}

%union {
  int nombre;
  int(*tuple)(int, int);
}

%token PARA_O
%token PARA_F
%token PEEK
%token RAND
%token CARDINAL
%token SELF
%token SPEED
%token STATE
%token GPSX
%token GPSY
%token ANGLE
%token TARGETX
%token TARGETY
%token DISTANCE
%token WAIT
%token POKE
%token ENGINE
%token SHOOT
%token GOTO
%token IF
%token THEN
%token<nombre> NOMBRE
%token<tuple> CMP
%token<tuple> OP

%type command
%type <nombre> condition
%type <nombre> expression

%start command

%%

command     : WAIT expression       {*delay += $2; *nextLine += 1;}
| IF condition THEN NOMBRE {if($2 && $2 > 0){*nextLine = $4; }else{ *nextLine += 1;};}
| GOTO NOMBRE                  {engine(robots,robot,missiles,arena,window,robot -> angle,robot -> vitesse); updateShoot(robots,robot,missiles,arena,window); refreshRobotCharacteristics(robots,window); *nextLine = $2;}
| POKE expression expression        {poke(&$2,$3); engine(robots,robot,missiles,arena,window,robot -> angle,robot -> vitesse); updateShoot(robots,robot,missiles,arena,window); refreshRobotCharacteristics(robots,window); *nextLine += 1;}
| SHOOT expression expression       {shoot(robots,robot,missiles,arena,window,$2,$3); engine(robots,robot,missiles,arena,window,robot -> angle,robot -> vitesse); refreshRobotCharacteristics(robots,window); *nextLine += 1;}
| ENGINE expression expression      {engine(robots,robot,missiles,arena,window,$2,$3); updateShoot(robots,robot,missiles,arena,window); refreshRobotCharacteristics(robots,window); *nextLine += 1;}
;


condition   : expression CMP expression {$$ = $2($1, $3);};

expression  : NOMBRE                                    {$$ = $1;}
| SELF                                                  {$$ = robot -> id;}
| SPEED                                                 {$$ = robot -> vitesse;}
| TARGETX expression expression expression              {$$ = targetx($2,$3,$4);}
| TARGETY expression expression expression              {$$ = targety($2,$3,$4);}
| STATE expression                                      {$$ = state(robots, $2);}
| GPSX expression                                       {$$ = gpsx(robots, $2);}
| GPSY expression                                       {$$ = gpsy(robots, $2);}
| PARA_O expression OP expression PARA_F                {$$ = $3($2, $4);}
| PEEK expression                                       {$$ = peek(&$2);}
| RAND expression                                       {$$ = ranD($2);}
| ANGLE expression expression expression expression     {$$ = angle($2,$3,$4,$5);}
| DISTANCE expression expression expression expression  {$$ = distance($2,$3,$4,$5);}
| CARDINAL                                              {$$ = 4;}
;

%%

void yyerror(char *line, Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window, int *nextLine, int *delay, char *s){
}

/*--------------------------------------------------------------------------------------*/

int egalite(int x, int y){
  return x == y;
}
int different(int x, int y){
  return x != y;
}
int  inferieur(int x, int y){
  return x  < y; }
int inferieurEgale(int x, int y){
  return x <= y;
}
int  superieur(int x, int y){
  return x  > y;
}
int superieurEgale(int x, int y){
  return x >= y;
}

int ajouter(int x, int y){
  return x + y;
}
int multiplier(int x, int y){
  return x * y;
}
int difference(int x, int y){
  return x - y;
}
int division(int x, int y){
  return x / y;
}
int modulo(int x, int y){
  return x % y;
}

/*--------------------------------------------------------------------------------------*/

double angle(double x1, double y1, double x2, double y2){
  double normeVecteur1, normVecteur2, numerateur, denominateur, angle;

  double x_vecteur = x2 - x1;
  double y_vecteur = y2 - y1;
  double x_vecteurUnitaire = 1;
  double y_vecteurUnitaire = 0;

  normeVecteur1 = sqrt(pow(x_vecteur,2) + pow(x_vecteurUnitaire,2));
  normVecteur2 = sqrt(pow(y_vecteur,2) + pow(y_vecteurUnitaire,2));

  numerateur = (x_vecteur * x_vecteurUnitaire) + (y_vecteur * y_vecteurUnitaire);
  denominateur = normeVecteur1 * normVecteur2;

  angle = (acos(numerateur / denominateur) * 180)/M_PI;

  if(y_vecteur < 0){
    return round(angle) * -1;
  }

  return round(angle);


}
/*--------------------------------------------------------------------------------------*/

double targetx(double x1, double angle, double length){
  return (length * cos(angle * M_PI/ 180)) + x1;
}

/*--------------------------------------------------------------------------------------*/

double targety(double y1, double angle, double length){
  return (length * sin(angle * M_PI/ 180)) + y1;
}

/*--------------------------------------------------------------------------------------*/

double distance(double x1, double y1, double x2, double y2){
  return round(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
}

/*--------------------------------------------------------------------------------------*/

void poke(int *adresse, int value){
  if(adresse == NULL)
    *adresse = value;
}

/*--------------------------------------------------------------------------------------*/

int peek(int *adresse){
  return *adresse;
}

/*--------------------------------------------------------------------------------------*/

int ranD(int num){
  srand(time(NULL));
  return rand() % num;
}

/*--------------------------------------------------------------------------------------*/

int state(Robot **robots, int num){
  return robots[num] -> pdv;
}

/*--------------------------------------------------------------------------------------*/

int gpsx(Robot **robots, int num){
  return robots[num] -> xLocation -1;
}

/*--------------------------------------------------------------------------------------*/

int gpsy(Robot **robots, int num){
  return robots[num] -> yLocation -1;
}
