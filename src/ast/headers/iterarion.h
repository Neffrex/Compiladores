#pragma once

#include "base.h"

extern int code_lineno;

identifier_t iterationRepeatStart();
void iterationRepeatEnd(operand_t* tmp_condition, identifier_t* tmp_counter);