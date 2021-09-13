#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include <sys/wait.h>
#include <math.h>
#include "main.h"

#include "BURP.tab.h"

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

/*--------------------------------------------------------------------------------------*/

int endGame(Arena *arena){
  int i, j, countDead;

  countDead = 0;
  for(i = 1; i < ARENA_HEIGHT; i++){
    for(j = 1; j < ARENA_WIDTH; j++){
      if(arena -> arene[i][j] != NULL && arena -> arene[i][j] -> mort == 1){
        countDead += 1;
      }
    }
  }
  if (countDead >=3){
    return 1;
  }

  return 0;
}

/*--------------------------------------------------------------------------------------*/

char ** getRobotScript(char *robotScript, int *nbrLines){
  char *line, *lineWithoutBreak;
  char **script;
  int numberLines, i, index;
  size_t len;
  ssize_t read;
  FILE *file;

  if((file = fopen(robotScript, "r")) != NULL){
    len = 0;
    line = NULL;
    numberLines = 0;
    while((read = getline(&line, &len, file)) != -1){
      numberLines += 1;
    }
    if(line)
      free(line);
    fclose(file);

    *nbrLines = numberLines;

    script = malloc(sizeof(char *) * numberLines);

    if(script == NULL){
      perror("malloc");
      exit(EXIT_FAILURE);
    }

    if((file = fopen(robotScript, "r")) != NULL){
      len = 0;
      line = NULL;
      index = 0;
      while((read = getline(&line, &len, file)) != -1){
        lineWithoutBreak = malloc(sizeof(char) * strlen(line));

        if(lineWithoutBreak == NULL){
          perror("malloc");
          exit(EXIT_FAILURE);
        }

        for(i = 0; i < strlen(line) - 1; i++){
          *(lineWithoutBreak + i) = *(line + i);
        }
        *(lineWithoutBreak + i) = '\0';

        *(script + index) = malloc(sizeof(char) * strlen(lineWithoutBreak));

        if(*(script + index) == NULL){
          perror("malloc");
          exit(EXIT_FAILURE);
        }

        strcpy(*(script + index), lineWithoutBreak);

        index += 1;

        free(lineWithoutBreak);
      }
      if(line)
        free(line);
      fclose(file);
    }else{
      perror("error open file");
      exit(EXIT_FAILURE);
    }
  }else{
    perror("error open file");
    exit(EXIT_FAILURE);
  }

  return script;
}

/*--------------------------------------------------------------------------------------*/

void parse(char *line, Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window, int *nextLine, int *delay){
    YY_BUFFER_STATE parsing_buffer = yy_scan_string(line);
    int p = yyparse(line, robots, robot, missiles, arena, window, nextLine, delay);
    yy_delete_buffer(parsing_buffer);
}

/*--------------------------------------------------------------------------------------*/

void showTheWinner(Arena *arena){
  //get screen size
  int xMax, yMax, i, j;
  char *winnerText, *winner;
  getmaxyx(stdscr, yMax, xMax);

  winner = malloc(sizeof(char));

  if(winner == NULL){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  for(i = 0; i < ARENA_HEIGHT; i++){
    for(j = 0; j < ARENA_WIDTH; j++){
      if(arena -> arene[i][j] != NULL && arena -> arene[i][j] -> pdv < 100){
        if(arena -> arene[i][j] -> id == 0){
          strcpy(winner, "0");
        }else if(arena -> arene[i][j] -> id == 1){
          strcpy(winner, "1");
        }else if(arena -> arene[i][j] -> id == 2){
          strcpy(winner, "2");
        }else{
          strcpy(winner, "3");
        }
      }
    }
  }
  
  if(strcmp("0", winner) != 0 && strcmp("1", winner) != 0 && strcmp("2", winner) != 0 && strcmp("3", winner) != 0){
    mvprintw(yMax/2, xMax/2 - strlen("NOBODY WON")/2, "NOBODY WON");
    mvprintw(yMax/2, xMax/2 - strlen("NOBODY WON")/2, "press any key to exit...");
  }else{
    winnerText = malloc(sizeof(char) * (strlen("ROBOT ") + 1 + strlen(" WON") + 2));

    if(winnerText == NULL){
      perror("malloc");
      exit(EXIT_FAILURE);
    }

    strcpy(winnerText, "ROBOT ");
    strcat(strcat(winnerText, winner), " WON,\0");
    mvprintw(yMax/2, xMax/2 - strlen(winnerText)/2, winnerText);
    mvprintw(yMax/2 + 1, xMax/2 - strlen(winnerText)/2, "press any key to exit...");
    free(winnerText);
  }
  free(winner);
}

/*--------------------------------------------------------------------------------------*/

int main(int argc, char *argv[]){
  int *nextLine, i, j, *numberLinesScript, *delay, key;
  char ***scripts;
  WIN *window;
  Arena *arena;
  Robot **robots;
  Missile **missiles;

  if(argc != 5){
  	printf("%s\n", "The command must be of the form : ./burp script_robot_1 script_robot_2 script_robot_3 script_robot_4");
  	return 0;
  }

  initscr();

  window = init_WIN();
  arena = init_arena(window);
  robots = init_robots(arena, window);
  init_robotCharacteristics(window);
  init_cycle(window);
  refreshRobotCharacteristics(robots, window);
  missiles = init_missiles();
  scripts = malloc(sizeof(char **) * 4);
  numberLinesScript = malloc(sizeof(int) * 4);
  nextLine = malloc(sizeof(int) * 4);
  delay = malloc(sizeof(int) * 4);

  for(i = 0; i < 4; i++){
  	scripts[i] = getRobotScript(argv[i + 1], &numberLinesScript[i]);
  	nextLine[i] = 0;
  	delay[i] = 1;
  }
  
  i = 0;
  while(!endGame(arena)){
    //getch();
    usleep(70000);
    if(i % 4 == 0){
    	refreshCycle(window, i/4);
    }

  	if(nextLine[i%4] >= 0 && nextLine[i%4] < numberLinesScript[i%4] && delay[i%4] == 0){
  		parse(scripts[i%4][nextLine[i%4]], robots, robots[i%4], missiles, arena, window, &nextLine[i%4], &delay[i%4]);
  	}else if(delay[i%4] > 0){
  		delay[i%4] -= 1;
  	}else{
  		break;
  	}
  	i++;
  }

  cleanWindow(window);
  showTheWinner(arena);

  for(i = 0; i < 8; i++){
    if(i < 4){
      free(robots[i]);
      for(j = 0; j < numberLinesScript[i]; j++){
        free(scripts[i][j]);
      }
      free(scripts[i]);
    }
    free(missiles[i]);
  }
  free(numberLinesScript);
  free(nextLine);
  free(delay);
  free(scripts);
  free(robots);
  free(window);
  free(arena);
  free(missiles);
  getch();
  endwin();
  return 0;
}
