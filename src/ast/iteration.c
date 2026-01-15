#include "headers/iterarion.h"

identifier_t iterationRepeatStart()
{
	identifier_t tmp_counter = createTemporalIdentifier(TYPE_INTEGER);
	tmp_counter.value = createIntegerLiteral(0);
	fprintf(yyout, "%d: %s := %d\n", code_lineno++, tmp_counter.name, 0);
	return tmp_counter;
}

void iterationRepeatEnd(operand_t* tmp_condition, identifier_t* tmp_counter)
{
	fprintf(yyout, "%d: %s := %s ADDI %d\n", code_lineno++, tmp_counter->name, tmp_counter->name, 1);
	fprintf(yyout, "%d: IF %s LTI %s GOTO %d\n", code_lineno++, operand2str(tmp_condition), tmp_counter->name, tmp_counter->declaration_lineno+1);
}