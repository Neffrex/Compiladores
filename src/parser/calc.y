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
	extern char current_line[];
	extern void reset_line(void);
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
%token <no_type> REPEAT DO DONE LBRACKET RBRACKET
%token <type> TYPE

%token <literal> INTEGER FLOAT
%token <operator> PLUS MINUS TIMES DIVIDE MOD POW

%token <identifier> IDENTIFIER UNDECLARED_IDENTIFIER
%type <no_type> statement statementList iteration


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
  { endOfProgram(); }
;

statementList:
  %empty
| statementList statement[s] EOL
{ log_message(LOG_MSG_END_OF_STATEMENT, yylineno-1, current_line); reset_line(); }
;

statement:
  declaration[id]
	
  | assignment[e]
	
  | expression[e]
	
  | iteration[e]
	
;

iteration:
  REPEAT arithmeticExpression[e] DO EOL
	{ $<identifier>$ = iterationRepeatStart();
		log_message(LOG_MSG_END_OF_STATEMENT, yylineno-1, current_line);
		reset_line(); }
	statementList DONE
	{ iterationRepeatEnd(&$e, &$<identifier>5); }
;

declaration:
	TYPE[t] identifierList[node]
	{ $$ = declare($node, $t); }
;

identifierList:
	UNDECLARED_IDENTIFIER[id]
	{ identifier_t identifier = createIdentifier($id.name);
		$$ = createIdentifierNode(&identifier, NULL); }
  | UNDECLARED_IDENTIFIER[id] LBRACKET INTEGER[size] RBRACKET
	{	identifier_t array = createArrayIdentifier($id.name, $size.ivalue);
		$$ = createIdentifierNode(&array, NULL); }
  | UNDECLARED_IDENTIFIER[id] COMMA identifierList[next]
	{	identifier_t identifier = createIdentifier($id.name);
		$$ = createIdentifierNode(&identifier, $next); }
  | UNDECLARED_IDENTIFIER[id] LBRACKET INTEGER[size] RBRACKET COMMA identifierList[next]
	{	identifier_t array = createArrayIdentifier($id.name, $size.ivalue);
		$$ = createIdentifierNode(&array, $next); }
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


