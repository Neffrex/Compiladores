#include "headers/base.h"

int code_lineno = 1;

literal_t createEmptyLiteral()
{
  literal_t result;
  result.type = TYPE_UNDEFINED;
  result.ivalue = 0;
  result.fvalue = 0.0f;
  return result;
}

literal_t createIntegerLiteral(int ivalue)
{
  literal_t result;
  result.type = TYPE_INTEGER;
  result.ivalue = ivalue;
  return result;
}

literal_t createFloatLiteral(float fvalue)
{
  literal_t result;
  result.type = TYPE_FLOAT;
  result.fvalue = fvalue;
  return result;
}

operand_t createOperandFromLiteral(literal_t* value)
{
	operand_t operand;
	operand.type = OPERAND_LITERAL;
	operand.literal = *value;
	return operand;
}

operand_t createOperandFromIdentifier(identifier_t* id)
{
	operand_t operand;
	operand.type = OPERAND_IDENTIFIER;
	operand.identifier = *id;
	return operand;
}

void endOfProgram(void)
{
	fprintf(yyout, "%d: %s\n", code_lineno, CODE_HALT);
	log_message(LOG_MSG_END_OF_PROGRAM, yylineno);
}