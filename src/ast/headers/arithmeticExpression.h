#pragma once
#include "base.h"

literal arithmeticExpressionLiteral(literal* value);

void logArithmeticExpressionEntry(literal*, op_type, literal*);
void logArithmeticExpressionExit(literal);
literal arithmeticExpression(literal*, op_type, literal*);

literal handleFloatArithmetic(literal*, op_type, literal*);
literal handleIntegerArithmetic(literal*, op_type, literal*);

literal arithmeticExpressionPlus(literal*, literal*);
literal arithmeticExpressionMinus(literal*, literal*);
literal arithmeticExpressionTimes(literal*, literal*);
literal arithmeticExpressionDivide(literal*, literal*);
literal arithmeticExpressionMod(literal*, literal*);
literal arithmeticExpressionPow(literal*, literal*);
literal arithmeticExpressionNegate(literal*);
