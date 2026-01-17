#pragma once
#include <stdbool.h>
#include <math.h>
#include "io.h"
#include "log.h"
#include "types.h"
#include "../../symtab/symtab.h"
#include "messages.h"
#include "identifier.h"

extern int yylineno;

literal_t createEmptyLiteral();
literal_t createIntegerLiteral(int);
literal_t createFloatLiteral(float);

operand_t createOperandFromLiteral(literal_t* value);
operand_t createOperandFromIdentifier(identifier_t* id);

void endOfProgram(void);