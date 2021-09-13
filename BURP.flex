%{

#define YY_NO_UNPUT
#define YY_NO_INPUT

#include "main.h"
#include "BURP.tab.h"
#include "BURP.h"

%}

blank   [ \t\r\f]

%%

-?[0-9]+    { yylval.nombre = atoi(yytext); return NOMBRE;}

"("             { return PARA_O;}
")"             { return PARA_F;}
"PEEK"          { return PEEK;}
"RAND"          { return RAND;}
"CARDINAL"      { return CARDINAL;}
"SELF"          { return SELF;}
"SPEED"         { return SPEED;}
"STATE"         { return STATE;}
"GPSX"          { return GPSX;}
"GPSY"          { return GPSY;}
"ANGLE"         { return ANGLE;}
"TARGETX"       { return TARGETX;}
"TARGETY"       { return TARGETY;}
"DISTANCE"      { return DISTANCE;}
"WAIT"          { return WAIT;}
"POKE"          { return POKE;}
"ENGINE"        { return ENGINE;}
"SHOOT"         { return SHOOT;}
"IF"            { return IF;}
"THEN"          { return THEN;}
"GOTO"          { return GOTO;}

"%"             { yylval.tuple = modulo; return OP;}
"+"             { yylval.tuple = ajouter;    return OP;}
"*"             { yylval.tuple = multiplier;   return OP;}
"/"             { yylval.tuple = division; return OP;}
"-"             { yylval.tuple = difference;  return OP;}
"<"             { yylval.tuple = inferieur;  return CMP;}
">="            { yylval.tuple = superieurEgale; return CMP;}
">"             { yylval.tuple = superieur;  return CMP;}
"<>"            { yylval.tuple = different; return CMP;}
"="             { yylval.tuple = egalite;  return CMP;}
"<="            { yylval.tuple = inferieurEgale; return CMP;}

blank           ;

[.]             { perror("error");}

%%

int yywrap(){
  return 1;
}
