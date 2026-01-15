/*
 * Scanner for a basic calculator compiler
 * author: Jose Luis Pueyo Viltres
 * e-mail: joseluis.pueyo@estudiants.urv.cat
 */

%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/calc.tab.h"
#include "../ast/headers/compiler.h"

#define TOKEN(X) log_message(LOG_INFO, LOG_MSG_TOKEN_VALUE, #X); return X

void yyerror(const char *s);

%}

%option noyywrap
%x COMMENT

/* DEFINITIONS */
DIGIT [0-9]
INTEGER {DIGIT}+
FLOAT {DIGIT}*\.{DIGIT}*([eE][-+]?{DIGIT}+)?
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*

TYPE_INTEGER "int"
TYPE_FLOAT "float"

REPEAT "repeat"
DO "do"
DONE "done"

WHITESPACE [ \t]
SINGLE_LINE_COMMENT "#"[^\n]*|"//"[^\n]*
EMPTY_LINE ^[ \t]*[\n]
EOL [\n]

%%

"("          { TOKEN(LPAREN); }
")"          { TOKEN(RPAREN); }

"["          { TOKEN(LBRACKET); }
"]"          { TOKEN(RBRACKET); }

":="         { TOKEN(ASSIGN); }
"+"          { TOKEN(PLUS); }
"-"          { TOKEN(MINUS); }
"*"          { TOKEN(TIMES); }
"/"          { TOKEN(DIVIDE); }
"%"          { TOKEN(MOD); }
"**"         { TOKEN(POW); }
","          { TOKEN(COMMA); }

{REPEAT}     { TOKEN(REPEAT); }
{DO}         { TOKEN(DO); }
{DONE}       { TOKEN(DONE); }

{TYPE_INTEGER} {
	yylval.type = TYPE_INTEGER;
	TOKEN(TYPE);
}
{TYPE_FLOAT}   { 
	yylval.type = TYPE_FLOAT;
	TOKEN(TYPE); 
}

{INTEGER} {
  yylval.literal.type = TYPE_INTEGER;
  yylval.literal.ivalue = atoi(yytext);
  TOKEN(INTEGER);
}
{FLOAT} {
  yylval.literal.type = TYPE_FLOAT;
  yylval.literal.fvalue = atof(yytext);
  TOKEN(FLOAT);
}

{IDENTIFIER} {
  identifier_t id = getIdentifier(yytext);
  id.name = strndup(yytext, yyleng); 
  yylval.identifier = id;

  if(id.type != TYPE_UNDEFINED)
  { TOKEN(IDENTIFIER); }
  else
  {	id.isArray = false; // Will be assigned while parsing
		id.declaration_lineno = yylineno-1;
    TOKEN(UNDECLARED_IDENTIFIER);
  }
}

{SINGLE_LINE_COMMENT} { }
{SINGLE_LINE_COMMENT}{EOL} { yylineno++; }
"/*"                  { BEGIN(COMMENT); }
<COMMENT>"*/\n"       { yylineno++; BEGIN(INITIAL); }
<COMMENT>"*/"         { BEGIN(INITIAL); }
<COMMENT>.            { }
<COMMENT>{EOL}        { yylineno++; }

{EMPTY_LINE} { yylineno++; }

{EOL} {
  yylineno++;
  TOKEN(EOL);
}
{WHITESPACE}  { }
<<EOF>>       { return 0; }
.             { halt(ERR_MSG_UNDEFINED_SEQUENCE, yytext); }

%%

void yyerror(const char *msg) {
	fprintf(stderr, "\033[31mERROR:%d:\033[0m %s\n", yylineno-1, msg);
  exit(EXIT_FAILURE);
}

