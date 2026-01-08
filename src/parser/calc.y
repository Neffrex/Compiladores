%{
/* Dependencies */
#include <stdio.h>
#include "calc.tab.h"


extern FILE *yyout;
extern char *yytext;
extern int yylineno;
extern int yylex(void);
extern void yyerror(const char *s);

%}

%code requires {
  #include "../ast/headers/compiler.h"
}

%union {
  bool boolean;
  literal literal;
  identifier identifier;
  identifier_node* p_identifier_node;
  op_type operator;
	type_t type;
  void* no_type;
}

%token <no_type> EOL YYEOF LPAREN RPAREN ASSIGN COMMA
%token <no_type> SIN COS TAN LEN SUBSTR
%token <type> TYPE

%token <literal> INTEGER FLOAT STRING CONSTANT
%token <operator> PLUS MINUS TIMES DIVIDE MOD POW

%token <identifier> IDENTIFIER UNTYPED_IDENTIFIER
%type <no_type> statement


%type <identifier> assignment
%type <p_identifier_node> declaration
%type <literal> expression
%type <p_identifier_node> identifierList

%type <literal> arithmeticExpression
%type <literal> arithmeticExpressionA
%type <literal> arithmeticExpressionM
%type <literal> arithmeticExpressionP
%type <literal> arithmeticExpressionX
%type <literal> arithmeticFunction

%define parse.error verbose

%start program

%%

program: 
  statementList YYEOF
  { log_message(LOG_INFO, LOG_MSG_END_OF_PROGRAM, yylineno); }
;

statementList:
  %empty
| statementList statement EOL
{ log_message(LOG_INFO, LOG_MSG_END_OF_STATEMENT, yylineno-1);}
;

statement:
  declaration[id]
	{ for (identifier_node* current = $id; current != NULL; current = current->next) 
    { cprint(yyout, "%s:%s%s", current->id.name, type2str(current->id.type), current->next != NULL? ", ": "\n"); }
  }
  | assignment[id]
  { cprint(yyout, "%s:%s = <%v>:%s\n", $id.name, type2str($id.type), &($id.value), type2str($id.value.type)); }
  | expression[e]
  { cprint(yyout, "<%v>:%s\n", &$e, type2str($e.type)); }
;

declaration:
	TYPE[t] identifierList[node]
	{ $$ = declare($node, $t); }
;

identifierList:
  UNTYPED_IDENTIFIER[id]
  { $$ = createIdentifierNode(NULL, &$id, TYPE_UNDEFINED); }
  | identifierList COMMA UNTYPED_IDENTIFIER[id]
  { $$ = createIdentifierNode($1, &$id, TYPE_UNDEFINED); }
;

assignment:
  IDENTIFIER[l] ASSIGN arithmeticExpression[r]
  { $$ = assign(&$l, $r); }
;

expression:
  arithmeticExpression

arithmeticExpression:
  arithmeticExpressionA 
;

arithmeticExpressionA:
  arithmeticExpressionM
  | arithmeticExpressionA[l] PLUS arithmeticExpressionM[r]
  { $$ = arithmeticExpressionPlus(&$l, &$r); }
  | arithmeticExpressionA[l] MINUS arithmeticExpressionM[r]
  { $$ = arithmeticExpressionMinus(&$l, &$r); }
  | MINUS arithmeticExpressionM[r]
  { $$ = arithmeticExpressionNegate(&$r); }
  | PLUS arithmeticExpressionM[r]
  { $$ = $r; }
;

arithmeticExpressionM:
  arithmeticExpressionP
  | arithmeticExpressionM[l] MOD arithmeticExpressionP[r]
  { $$ = arithmeticExpressionMod(&$l, &$r); }
  | arithmeticExpressionM[l] TIMES arithmeticExpressionP[r]
  { $$ = arithmeticExpressionTimes(&$l, &$r); }
  | arithmeticExpressionM[l] DIVIDE arithmeticExpressionP[r]
  { $$ = arithmeticExpressionDivide(&$l, &$r); }
;

arithmeticExpressionP:
  arithmeticExpressionX
  | arithmeticExpressionP[l] POW arithmeticExpressionX[r]
  { $$ = arithmeticExpressionPow(&$l, &$r); }
;

arithmeticExpressionX:
  INTEGER[x]
  { $$ = arithmeticExpressionLiteral(&$x); }
  | FLOAT[x]
  { $$ = arithmeticExpressionLiteral(&$x); }
  | IDENTIFIER[x]
  { $$ = $x.value; }
  | LPAREN arithmeticExpression[x] RPAREN
  { $$ = $x; }
;

%%

int main(int argc, char** argv) {
  init_log("debug.log");
  yyparse();
  close_log();
  return 0;
}


