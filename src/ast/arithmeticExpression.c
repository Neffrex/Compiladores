#include "headers/arithmeticExpression.h"

operand_t arithmeticExpressionLiteral(literal_t* value)
{
  log_message(LOG_INFO, LOG_MSG_LITERAL_VALUE, literal2str(value), type2str(value->type));
  return createOperandFromLiteral(value);
}

operand_t arithmeticExpressionIdentifier(identifier_t* id)
{
	log_message(LOG_INFO, LOG_MSG_IDENTIFIER_VALUE, id->name, type2str(id->type));
	return createOperandFromIdentifier(id);
}

void logArithmeticExpressionUnaryEntry(op_type_t op, operand_t* operand)
{}

void logArithmeticExpressionBinaryEntry(operand_t* loperand, op_type_t op, operand_t* roperand)
{}

void logArithmeticExpressionResolution(literal_t* result) 
{}

operand_t arithmeticExpressionBinary(operand_t* loperand, op_type_t op, operand_t* roperand)
{ 
	operand_t result;
	logArithmeticExpressionBinaryEntry(loperand, op, roperand);

	if(loperand->type == OPERAND_LITERAL && roperand->type == OPERAND_LITERAL)
	{
		literal_t computedLiteral;
		computedLiteral = computeArithmeticExpressionBinary(&loperand->literal, op, &roperand->literal);
		result = createOperandFromLiteral(&computedLiteral);
		logArithmeticExpressionResolution(&computedLiteral);
	}
	else
	{
		result = generateBinaryOperationCode(loperand, op, roperand);
	}

	return result;
}

literal_t computeArithmeticExpressionBinary(literal_t* loperand, op_type_t op, literal_t* roperand)
{
		literal_t result = createEmptyLiteral();
	if(isInteger(loperand) && isInteger(roperand))
	{	
		switch(op) {
			case OP_PLUS:    result.ivalue = loperand->ivalue + roperand->ivalue; break;
			case OP_MINUS:   result.ivalue = loperand->ivalue - roperand->ivalue; break;
			case OP_TIMES:   result.ivalue = loperand->ivalue * roperand->ivalue; break;
			case OP_DIVIDE:  result.ivalue = loperand->ivalue / roperand->ivalue; break;
			case OP_MOD:     result.ivalue = loperand->ivalue % roperand->ivalue; break;
			case OP_POW:     result.ivalue = (int)(pow(loperand->ivalue, roperand->ivalue)+0.5); break;
			default:
				halt(ERR_MSG_INVALID_OPERATION, literal2str(loperand), op2str(op), literal2str(roperand));
		}
		result.type = TYPE_INTEGER;
	}
	else if((isInteger(loperand) || isFloat(loperand)) && (isInteger(roperand) || isFloat(roperand)))
	{	
		float lvalue = isInteger(loperand)? (float)loperand->ivalue: loperand->fvalue;
		float rvalue = isInteger(roperand)? (float)roperand->ivalue: roperand->fvalue;
		switch (op) {
			case OP_PLUS:    result.fvalue = lvalue + rvalue; break;
			case OP_MINUS:   result.fvalue = lvalue - rvalue; break;
			case OP_TIMES:   result.fvalue = lvalue * rvalue; break;
			case OP_DIVIDE:  result.fvalue = lvalue / rvalue; break;
			case OP_POW:     result.fvalue = (float)(pow(lvalue, rvalue)); break;
			default:
				halt(ERR_MSG_INVALID_OPERATION, literal2str(loperand), op2str(op), literal2str(roperand));
		}
		result.type = TYPE_FLOAT;
	}
	else
	{ halt(ERR_MSG_INVALID_OPERATION, literal2str(loperand), S_PLUS, literal2str(roperand)); }

	return result;
}

operand_t arithmeticExpressionUnary(op_type_t op, operand_t* operand)
{
	operand_t result;
	logArithmeticExpressionUnaryEntry(op, operand);
	switch (operand->type)
	{
		case OPERAND_LITERAL:
			literal_t computedLiteral;
			computedLiteral = computeArithmeticExpressionUnary(op, &operand->literal);
			result = createOperandFromLiteral(&computedLiteral);
			logArithmeticExpressionResolution(&computedLiteral);
			break;
		case OPERAND_IDENTIFIER:
			result = generateUnaryOperationCode(op, operand);
			break;
		default:	halt(ERR_MSG_INVALID_OPERATION, op2str(op), operand2str(operand), "");
	}
	return result;
}

literal_t computeArithmeticExpressionUnary(op_type_t op, literal_t* operand)
{
	literal_t result = createEmptyLiteral();

	if(isInteger(operand))
	{	
		switch(op) {
			case OP_NEGATE:    result.ivalue = -operand->ivalue; break;
			default:
				halt(ERR_MSG_INVALID_OPERATION, op2str(op), literal2str(operand), "");
		}
		result.type = TYPE_INTEGER;
	}
	else if(isFloat(operand))
	{	
		switch (op) {
			case OP_NEGATE:    result.fvalue = -operand->fvalue; break;
			default:
				halt(ERR_MSG_INVALID_OPERATION, op2str(op), literal2str(operand), "");
		}
		result.type = TYPE_FLOAT;
	}
	else
	{ halt(ERR_MSG_INVALID_OPERATION, op2str(op), literal2str(operand), ""); }

	return result;
}

int typePromotion(operand_t* result, operand_t* loperand, operand_t* roperand)
{
	if (isOperandInteger(loperand) && isOperandFloat(roperand))
	{	*result = arithmeticExpressionUnary(OP_I2F, loperand);
		return PROMOTED_LEFT;
	}
	else if (isOperandFloat(loperand) && isOperandInteger(roperand))
	{	*result = arithmeticExpressionUnary(OP_I2F, roperand);
		return PROMOTED_RIGHT;
	}
	return NO_PROMOTION;
}

operand_t generateBinaryOperationCode(operand_t* loperand, op_type_t op, operand_t* roperand)
{
	const data_type_t resultType = isOperandFloat(loperand) || isOperandFloat(roperand)? TYPE_FLOAT : TYPE_INTEGER;

	// POW and MOD can only operate over integers
	if ((op == OP_POW || op == OP_MOD) && resultType != TYPE_INTEGER)
	{ halt(ERR_MSG_INVALID_OPERATION, operand2str(loperand), op2str(OP_POW), operand2str(roperand)); }

	operand_t promotedTmp;
	int promotionCmp = typePromotion(&promotedTmp, loperand, roperand);
	operand_t tmp_loperand = (promotionCmp == PROMOTED_LEFT) ? promotedTmp : *loperand;
	operand_t tmp_roperand = (promotionCmp == PROMOTED_RIGHT) ? promotedTmp : *roperand;
	

	identifier_t tmp = createTemporalIdentifier(resultType);
	
	// POW has no direct translation
	if(op == OP_POW)
	{ generatePowC3aCode(&tmp, loperand, roperand); }
	else
	{	fprintf(yyout, "%d: %s := %s %s%s %s\n", code_lineno++, tmp.name, operand2str(&tmp_loperand), op2code(op), SUFIX(resultType), operand2str(&tmp_roperand)); }

	return createOperandFromIdentifier(&tmp);
}

operand_t generateUnaryOperationCode(op_type_t op, operand_t* operand)
{
	data_type_t resultType;
	switch(op)
	{
		case OP_I2F:
			if(getOperandDataType(operand) != TYPE_INTEGER)
			{ /*TODO: Halt msg for invalid type on type casting */ halt(""); }
			resultType = TYPE_FLOAT;
			break;
		case OP_F2I:
			if(getOperandDataType(operand) != TYPE_FLOAT)
			{ /*TODO: Halt msg for invalid type on type casting */ halt(""); }
			resultType = TYPE_INTEGER;
			break;
		default:
			resultType = getOperandDataType(operand);
	}
	
	const char* operand_str = operand2str(operand);
	identifier_t tmp = createTemporalIdentifier(resultType);
	fprintf(yyout, "%d: %s := %s%s %s\n",
		code_lineno++,
		tmp.name,
		op2code(op),
		(resultType == TYPE_FLOAT) ? CODE_FLOAT_SUFIX : CODE_INTEGER_SUFIX,
		operand_str
	);

	return createOperandFromIdentifier(&tmp);
}

void generatePowC3aCode(identifier_t* result, operand_t* loperand, operand_t* roperand)
{
	// Generate code for exponentiation using repeated multiplication
	// result = loperand ^ roperand
	char* tmp_result = createTemporalIdentifier(TYPE_INTEGER).name;
	char* tmp_counter = createTemporalIdentifier(TYPE_INTEGER).name;
	
	// result = 1
	fprintf(yyout, "%d: %s := 1\n", code_lineno++, tmp_result);
	
	// counter = 0
	fprintf(yyout, "%d: %s := 0\n", code_lineno++, tmp_counter);
	
	// while counter < exponent
	int loop_start = code_lineno;
	fprintf(yyout, "%d: IF %s GEI %s GOTO %d\n", code_lineno, tmp_counter, operand2str(roperand), code_lineno + 3);
	code_lineno++; // To avoid the warning Wsequence-point
	
	// result = result * base
	fprintf(yyout, "%d: %s := %s MULI %s\n", code_lineno++, tmp_result, tmp_result, operand2str(loperand));
	
	// counter = counter + 1
	fprintf(yyout, "%d: %s := %s ADDI 1\n", code_lineno++, tmp_counter, tmp_counter);
	
	// goto loop_start
	fprintf(yyout, "%d: GOTO %d\n", code_lineno++, loop_start);
	
	// L: done
	// Assign final result to output identifier
	fprintf(yyout, "%d: %s := %s\n", code_lineno++, result->name, tmp_result);
}
