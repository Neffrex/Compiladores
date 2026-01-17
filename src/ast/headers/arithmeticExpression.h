#pragma once
#include "base.h"

#define SUFIX(type) (type == TYPE_FLOAT) ? CODE_FLOAT_SUFIX : CODE_INTEGER_SUFIX
#define PROMOTED_LEFT -1
#define PROMOTED_RIGHT 1
#define NO_PROMOTION 0

extern int code_lineno;
extern FILE* yyout;

operand_t arithmeticExpressionLiteral(literal_t* value);
operand_t arithmeticExpressionIdentifier(identifier_t* id);

void logArithmeticExpressionUnaryEntry(op_type_t op, operand_t* operand);
void logArithmeticExpressionBinaryEntry(operand_t* loperand, op_type_t op, operand_t* roperand);
void logArithmeticExpressionResolution(literal_t* result) ;

operand_t arithmeticExpressionBinary(operand_t* loperand, op_type_t op, operand_t* roperand);
literal_t computeArithmeticExpressionBinary(literal_t* loperand, op_type_t op, literal_t* roperand);
operand_t arithmeticExpressionUnary(op_type_t op, operand_t* operand);
literal_t computeArithmeticExpressionUnary(op_type_t op, literal_t* operand);

int typePromotion(operand_t* result, operand_t* loperand, operand_t* roperand);
operand_t generateBinaryOperationCode(operand_t* loperand, op_type_t op, operand_t* roperand);
operand_t generateUnaryOperationCode(op_type_t op, operand_t* operand);
identifier_t generatePowC3aCode(operand_t* loperand, operand_t* roperand);