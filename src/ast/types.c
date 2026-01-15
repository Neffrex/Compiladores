#include "headers/types.h"

bool isNullLiteral(literal_t* literal)
{ return literal->type == TYPE_UNDEFINED; }

bool isInteger(literal_t* literal)
{ return literal->type == TYPE_INTEGER; }

bool isFloat(literal_t* literal)
{ return literal->type == TYPE_FLOAT; }

bool isOperandInteger(operand_t* operand)
{
	return operand->type == OPERAND_IDENTIFIER? operand->identifier.type == TYPE_INTEGER : operand->literal.type == TYPE_INTEGER;
}

bool isOperandFloat(operand_t* operand)
{
	return operand->type == OPERAND_IDENTIFIER? operand->identifier.type == TYPE_FLOAT : operand->literal.type == TYPE_FLOAT;
}

char* literal2str(literal_t* literal)
{
  char* buffer = malloc(STR_MAX_LENGTH);
  size_t size = STR_MAX_LENGTH;

  switch(literal->type)
  {	case TYPE_INTEGER:   snprintf(buffer, size, "%d", literal->ivalue); break;
    case TYPE_FLOAT:     snprintf(buffer, size, "%.3f", literal->fvalue); break;
    default: snprintf(buffer, size, S_UNDEFINED);
  }

  return buffer;
}

const char* type2str(data_type_t type)
{
  switch(type)
  {	case TYPE_INTEGER:   return S_INTEGER;
    case TYPE_FLOAT:     return S_FLOAT;
    default: return S_UNDEFINED;
  }
}

const char* op2str(op_type_t op)
{
  switch(op) 
  { case OP_NEGATE:          return S_NEGATE;
    case OP_PLUS:            return S_PLUS;
    case OP_MINUS:           return S_MINUS;
    case OP_TIMES:           return S_TIMES;
    case OP_DIVIDE:          return S_DIVIDE;
    case OP_MOD:             return S_MOD;
    case OP_POW:             return S_POW;
    default: return S_NULL;
  }
}

const data_type_t getOperandDataType(operand_t* operand)
{
	switch(operand->type)
	{	case OPERAND_LITERAL:		return operand->literal.type;
		case OPERAND_IDENTIFIER:	return operand->identifier.type;
		default:	return TYPE_UNDEFINED;
	}
}

const char* operand2str(operand_t* operand)
{
	switch(operand->type)
	{	case OPERAND_LITERAL:	return literal2str(&operand->literal);
		case OPERAND_IDENTIFIER:	return operand->identifier.name;
		default:	return S_NULL;
	}
}

const char* op2code(op_type_t op)
{
	switch(op)
	{
		case OP_NEGATE: return CODE_NEGATE;
		case OP_PLUS: return CODE_PLUS;
		case OP_MINUS: return CODE_MINUS;
		case OP_TIMES: return CODE_TIMES;
		case OP_DIVIDE: return CODE_DIVIDE;
		case OP_MOD: return CODE_MOD;
		case OP_POW: return CODE_POW;
		case OP_F2I: return CODE_F2I;
		case OP_I2F: return CODE_I2F;
		default: return CODE_UNDEFINED;
	}
}