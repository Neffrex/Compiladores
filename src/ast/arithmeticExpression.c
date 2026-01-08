#include "headers/arithmeticExpression.h"

literal arithmeticExpressionLiteral(literal* value) {
  log_message(LOG_INFO, LOG_MSG_LITERAL_VALUE, literal2str(value), type2str(value->type));
  return *value;
}

void logArithmeticExpressionEntry(literal* loperand, op_type op, literal* roperand) 
{
  char* lvalue = literal2str(loperand);
  char* rvalue = literal2str(roperand);

  log_message(LOG_INFO, LOG_MSG_ARITHMETIC_EXPRESSION, lvalue, type2str(loperand->type), op2str(op), rvalue, type2str(roperand->type));
  free(lvalue); free(rvalue);  
}

void logArithmeticExpressionExit(literal result) 
{
  char* sresult = literal2str(&result);

  log_message(LOG_INFO, LOG_MSG_END_OF_ARITHMETIC_EXPRESSION, sresult);
  free(sresult); 
}

literal arithmeticExpression(literal* loperand, op_type op, literal* roperand)
{
  literal result = createEmptyLiteral();
  logArithmeticExpressionEntry(loperand, op, roperand);

  result = handleIntegerArithmetic(loperand, op, roperand);
  if(!isNullLiteral(&result)) {
    logArithmeticExpressionExit(result);
    return result;
  }

  result = handleFloatArithmetic(loperand, op, roperand);
  if(!isNullLiteral(&result)) {
    logArithmeticExpressionExit(result);
    return result;
  }

  halt(ERR_MSG_INVALID_OPERATION, type2str(loperand->type), op2str(op), type2str(roperand->type));
  return result;
}

literal handleFloatArithmetic(literal* loperand, op_type op, literal* roperand)
{
	literal result = createEmptyLiteral();
  if(!isNullLiteral(loperand) && !isNullLiteral(roperand))
  { 
		double lvalue = isInteger(loperand)? (double)loperand->ivalue: (double)loperand->fvalue;
		double rvalue = isInteger(roperand)? (double)roperand->ivalue: (double)roperand->fvalue;
		result.type = TYPE_FLOAT;

		switch(op)
		{
			case OP_PLUS:    result.fvalue = (float)(lvalue + rvalue); break;
			case OP_MINUS:   result.fvalue = (float)(lvalue - rvalue); break;
			case OP_TIMES:   result.fvalue = (float)(lvalue * rvalue); break;
			case OP_DIVIDE:  result.fvalue = (float)(lvalue / rvalue); break;
			case OP_POW:     result.fvalue = (float)(pow(lvalue, rvalue)); break;
			default:
				halt(ERR_MSG_INVALID_OPERATION, type2str(loperand->type), op2str(op), type2str(roperand->type));
		}
	}
  return result;
}

literal handleIntegerArithmetic(literal* loperand, op_type op, literal* roperand)
{
  literal result = createEmptyLiteral();
	if(isInteger(loperand) && isInteger(roperand))
  {
		result.type = TYPE_INTEGER;
		switch(op)
		{
			case OP_PLUS:    result.ivalue = loperand->ivalue + roperand->ivalue; break;
			case OP_MINUS:   result.ivalue = loperand->ivalue - roperand->ivalue; break;
			case OP_TIMES:   result.ivalue = loperand->ivalue * roperand->ivalue; break;
			case OP_DIVIDE:  result.ivalue = loperand->ivalue / roperand->ivalue; break;
			case OP_MOD:     result.ivalue = loperand->ivalue % roperand->ivalue; break;
			case OP_POW:     result.ivalue = (int)(pow(loperand->ivalue, roperand->ivalue)+0.5); break;
			default:
				halt(ERR_MSG_INVALID_OPERATION, type2str(loperand->type), op2str(op), type2str(roperand->type));
		}
	}

  return result;
}

literal arithmeticExpressionPlus(literal* loperand, literal* roperand)
{ return arithmeticExpression(loperand, OP_PLUS, roperand); }

literal arithmeticExpressionMinus(literal* loperand, literal* roperand)
{ return arithmeticExpression(loperand, OP_MINUS, roperand); }

literal arithmeticExpressionTimes(literal* loperand, literal* roperand)
{ return arithmeticExpression(loperand, OP_TIMES, roperand); }

literal arithmeticExpressionDivide(literal* loperand, literal* roperand)
{ return arithmeticExpression(loperand, OP_DIVIDE, roperand); }

literal arithmeticExpressionMod(literal* loperand, literal* roperand)
{ return arithmeticExpression(loperand, OP_MOD, roperand); }

literal arithmeticExpressionPow(literal* loperand, literal* roperand)
{ return arithmeticExpression(loperand, OP_POW, roperand); }

literal arithmeticExpressionNegate(literal* operand)
{ 
  literal result;
  if(isInteger(operand))
  { result = createIntegerLiteral(-operand->ivalue); }
  else if(isFloat(operand))
  { result = createFloatLiteral(-operand->fvalue); }
  else
  { // TYPE ERROR
    halt(ERR_MSG_INVALID_OPERAND_TYPE,
      op2str(OP_NEGATE), type2str(operand->type));
  }
  return result;
}