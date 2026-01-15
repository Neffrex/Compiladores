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
  literal_t literal;
  identifier_t identifier;
  identifier_node_t* p_identifier_node;
  op_type_t operator;
	operand_t operand;
	data_type_t type;
  void* no_type;

}

%token <no_type> EOL YYEOF LPAREN RPAREN ASSIGN COMMA
%token <no_type> REPEAT DO DONE
%token <type> TYPE

%token <literal> INTEGER FLOAT
%token <operator> PLUS MINUS TIMES DIVIDE MOD POW

%token <identifier> IDENTIFIER UNTYPED_IDENTIFIER
%type <no_type> statement statementList // iteration


%type <no_type> assignment
%type <no_type> declaration
%type <no_type> expression
%type <p_identifier_node> identifierList


%type <operand> arithmeticExpression
%type <operand> arithmeticExpressionM
%type <operand> arithmeticExpressionP
%type <operand> arithmeticExpressionX
%type <literal> arithmeticExpressionLiteral

%define parse.error verbose

%start program

%%

program: 
  statementList YYEOF
  { log_message(LOG_INFO, LOG_MSG_END_OF_PROGRAM, yylineno); }
;

statementList:
  %empty
| statementList statement[s] EOL
{ log_message(LOG_INFO, LOG_MSG_END_OF_STATEMENT, yylineno-1);}
;

statement:
  declaration[id]
  | assignment[e]
  | expression[e]
  // | iteration[e]
;

// iteration:
//   REPEAT arithmeticExpression[e] DO statementList DONE
//   { $$ = $e; }
// ;

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
	{ assign(&$l, &$r); }
;

expression:
  arithmeticExpression[e]
	{ expression(&$e); }
;


arithmeticExpression:
  arithmeticExpressionM
  | arithmeticExpression[l] PLUS arithmeticExpressionM[r]
  { $$ = arithmeticExpressionBinary(&$l, OP_PLUS, &$r); }
  | arithmeticExpression[l] MINUS arithmeticExpressionM[r]
  { $$ = arithmeticExpressionBinary(&$l, OP_MINUS, &$r); }
  | MINUS arithmeticExpressionM[r]
  { $$ = arithmeticExpressionUnary(OP_NEGATE, &$r); }
  | PLUS arithmeticExpressionM[r]
  { $$ = $r; }
;

arithmeticExpressionM:
  arithmeticExpressionP
  | arithmeticExpressionM[l] MOD arithmeticExpressionP[r]
  { $$ = arithmeticExpressionBinary(&$l, OP_MOD, &$r); }
  | arithmeticExpressionM[l] TIMES arithmeticExpressionP[r]
  { $$ = arithmeticExpressionBinary(&$l, OP_TIMES, &$r); }
  | arithmeticExpressionM[l] DIVIDE arithmeticExpressionP[r]
  { $$ = arithmeticExpressionBinary(&$l, OP_DIVIDE, &$r); }
;

arithmeticExpressionP:
  arithmeticExpressionX
  | arithmeticExpressionP[l] POW arithmeticExpressionX[r]
  { $$ = arithmeticExpressionBinary(&$l, OP_POW, &$r); }
;

arithmeticExpressionX:
  arithmeticExpressionLiteral[x]
  { $$ = arithmeticExpressionLiteral(&$x); }
  | IDENTIFIER[x]
  { $$ = arithmeticExpressionIdentifier(&$x); }
  | LPAREN arithmeticExpression[x] RPAREN
  { $$ = $x; }
;

arithmeticExpressionLiteral:
	INTEGER | FLOAT
;

%%

int main(int argc, char** argv) {
  init_log("debug.log");
  yyparse();
  close_log();
  return 0;
}


