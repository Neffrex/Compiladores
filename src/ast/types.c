#include "headers/types.h"

bool isNullLiteral(literal* literal)
{ return literal->type == TYPE_UNDEFINED; }

bool isInteger(literal* literal)
{ return literal->type == TYPE_INTEGER; }

bool isFloat(literal* literal)
{ return literal->type == TYPE_FLOAT; }

char* literal2str(literal* literal)
{
  char* buffer = malloc(STR_MAX_LENGTH);
  size_t size = STR_MAX_LENGTH;

  switch(literal->type)
  {
    case TYPE_INTEGER:   snprintf(buffer, size, "%d", literal->ivalue); break;
    case TYPE_FLOAT:     snprintf(buffer, size, "%.3f", literal->fvalue); break;
    default: snprintf(buffer, size, S_UNDEFINED);
  }

  return buffer;
}

const char* type2str(type_t type) 
{
  switch(type)
  {
    case TYPE_INTEGER:   return S_INTEGER;
    case TYPE_FLOAT:     return S_FLOAT;
    default: return S_UNDEFINED;
  }
}

const char* op2str(op_type op)
{
  switch(op) 
  {
    case OP_NEGATE:          return S_NEGATE;
    case OP_PLUS:            return S_PLUS;
    case OP_MINUS:           return S_MINUS;
    case OP_TIMES:           return S_TIMES;
    case OP_DIVIDE:          return S_DIVIDE;
    case OP_MOD:             return S_MOD;
    case OP_POW:             return S_POW;
    default: return S_NULL;
  }
}