#include "headers/arithmeticExpression.h"

operand_t arithmeticExpressionLiteral(literal_t* value)
{ return createOperandFromLiteral(value); }

operand_t arithmeticExpressionIdentifier(identifier_t* id)
{ return createOperandFromIdentifier(id); }

operand_t arithmeticExpressionArrayIdentifier(identifier_t* id, literal_t* index)
{
	if(!isInteger(index)) {
		halt(ERR_MSG_INVALID_ARRAY_INDEX_TYPE, literal2str(index), type2str(index->type));
	}

	identifier_t member = createTemporalIdentifier(id->members_type);
	index->ivalue += 25;
	log_message(LOG_MSG_ARITHMETIC_EXPRESSION_ARRAY_IDENTIFIER, member.name, id->name, index->ivalue);
	cprint("%s := %s[%s]\n", member.name, id->name, literal2str(index));
	return createOperandFromIdentifier(&member);
}

void logArithmeticExpressionBinaryResolution(operand_t* loperand, op_type_t op, operand_t* roperand, literal_t* result)
{ char* sresult = literal2str(result);
	log_message(LOG_MSG_ARITHMETIC_EXPRESSION_BINARY_RESOLUTION, 
		operand2str(loperand), type2str(getOperandDataType(loperand)),
		op2str(op),
		operand2str(roperand), type2str(getOperandDataType(roperand)),
		sresult, type2str(result->type));
	free(sresult);
}

void logArithmeticExpressionUnaryResolution(op_type_t op, operand_t* operand, literal_t* result)
{ char* sresult = literal2str(result);
	log_message(LOG_MSG_ARITHMETIC_EXPRESSION_BINARY_RESOLUTION, 
		op2str(op),
		operand2str(operand), type2str(getOperandDataType(operand)),
		sresult, type2str(result->type)
	);
	free(sresult);
}

operand_t arithmeticExpressionBinary(operand_t* loperand, op_type_t op, operand_t* roperand)
{ 
	operand_t result;

	if(loperand->type == OPERAND_LITERAL && roperand->type == OPERAND_LITERAL)
	{
		literal_t computedLiteral;
		computedLiteral = computeArithmeticExpressionBinary(&loperand->literal, op, &roperand->literal);
		result = createOperandFromLiteral(&computedLiteral);
		logArithmeticExpressionBinaryResolution(loperand, op, roperand, &computedLiteral);
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
	switch (operand->type)
	{
		case OPERAND_LITERAL:
			literal_t computedLiteral;
			computedLiteral = computeArithmeticExpressionUnary(op, &operand->literal);
			result = createOperandFromLiteral(&computedLiteral);
			logArithmeticExpressionUnaryResolution(op, operand, &computedLiteral);
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
			case OP_NEGATE:
				result.ivalue = -operand->ivalue;
				result.type = TYPE_INTEGER;
				break;
			case OP_I2F:
				result.fvalue = (float) operand->ivalue;
				result.type = TYPE_FLOAT;
				break;
			default:
				halt(ERR_MSG_INVALID_OPERATION, op2str(op), literal2str(operand), "");
		}
		
	}
	else if(isFloat(operand))
	{	
		switch (op) {
			case OP_NEGATE:
				result.fvalue = -operand->fvalue;
				result.type = TYPE_FLOAT;
				break;
			case OP_F2I:
				result.ivalue = (int) operand->fvalue;
				result.type = TYPE_INTEGER;
				break;
			default:
				halt(ERR_MSG_INVALID_OPERATION, op2str(op), literal2str(operand), "");
		}
		
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
	

	identifier_t tmp;
	
	// POW has no direct translation
	if(op == OP_POW)
	{ tmp = generatePowC3aCode(loperand, roperand); }
	else
	{	tmp = createTemporalIdentifier(resultType);
		cprint("%s := %s %s%s %s\n", tmp.name, operand2str(&tmp_loperand), op2code(op), SUFIX(resultType), operand2str(&tmp_roperand));
	}

	log_message(LOG_MSG_ARITHMETIC_EXPRESSION_BINARY_TMP,
		tmp.name,
		operand2str(&tmp_loperand), type2str(getOperandDataType(&tmp_loperand)),
		op2str(op),
		operand2str(&tmp_roperand), type2str(getOperandDataType(&tmp_roperand))
	); 
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
	cprint("%s := %s%s %s\n",
		tmp.name,
		op2code(op),
		(resultType == TYPE_FLOAT) ? CODE_FLOAT_SUFIX : CODE_INTEGER_SUFIX,
		operand_str
	);

	log_message(LOG_MSG_ARITHMETIC_EXPRESSION_UNARY_TMP, tmp.name,	op2str(op),	operand2str(operand), type2str(getOperandDataType(operand))
	); 

	return createOperandFromIdentifier(&tmp);
}

identifier_t generatePowC3aCode(operand_t* loperand, operand_t* roperand)
{
	// Generate code for exponentiation using repeated multiplication
	// result = loperand ^ roperand
	char* tmp_result = createTemporalIdentifier(TYPE_INTEGER).name;
	char* tmp_counter = createTemporalIdentifier(TYPE_INTEGER).name;

	identifier_t result = createTemporalIdentifier(TYPE_INTEGER);	

	// result = 1
	cprint("%s := 1\n", tmp_result);
	log_message("[ Arithmetic Expression ]: %s := 1 (POW result)", tmp_result);
	
	// counter = 0
	cprint("%s := 0\n", tmp_counter);
	log_message("[ Arithmetic Expression ]: %s := 0 (POW counter)", tmp_counter);
	
	// while counter < exponent
	int loop_start = code_lineno;
	cprint("IF %s GEI %s GOTO %d\n", tmp_counter, operand2str(roperand), code_lineno + 4);
	
	// result = result * base
	cprint("%s := %s MULI %s\n", tmp_result, tmp_result, operand2str(loperand));
	
	// counter = counter + 1
	cprint("%s := %s ADDI 1\n", tmp_counter, tmp_counter);
	
	// goto loop_start
	cprint("GOTO %d\n", loop_start);
	
	// L: done
	// Assign final result to output identifier
	cprint("%s := %s\n", result.name, tmp_result);

	return result;
}
