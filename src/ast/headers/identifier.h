#pragma once
#include "base.h"

extern int code_lineno;
extern FILE* yyout;

identifier_node_t* declare(identifier_node_t* id_node, data_type_t type) ;
identifier_t createTemporalIdentifier(data_type_t type);
identifier_node_t* createIdentifierNode(identifier_node_t* head, identifier_t* id, data_type_t type);
void logAssignEntry(identifier_t* id, operand_t* operand);
identifier_t assign(identifier_t* id, operand_t* operand);
void generateAssignCode(identifier_t* id, operand_t* operand);
identifier_t getIdentifier(char* name);
void expression(operand_t* operand);