/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_BURP_TAB_H_INCLUDED
# define YY_YY_BURP_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PARA_O = 258,
    PARA_F = 259,
    PEEK = 260,
    RAND = 261,
    CARDINAL = 262,
    SELF = 263,
    SPEED = 264,
    STATE = 265,
    GPSX = 266,
    GPSY = 267,
    ANGLE = 268,
    TARGETX = 269,
    TARGETY = 270,
    DISTANCE = 271,
    WAIT = 272,
    POKE = 273,
    ENGINE = 274,
    SHOOT = 275,
    GOTO = 276,
    IF = 277,
    THEN = 278,
    NOMBRE = 279,
    CMP = 280,
    OP = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 20 "BURP.y" /* yacc.c:1921  */

  int nombre;
  int(*tuple)(int, int);

#line 90 "BURP.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (char * ligne, Robot **robots, Robot *robot, Missile **missiles, Arena *arena, WIN *window, int *nextLine, int *delay);

#endif /* !YY_YY_BURP_TAB_H_INCLUDED  */
