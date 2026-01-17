#pragma once
#include "base.h"

extern int code_lineno;
extern FILE* yyout;

identifier_node_t* declare(identifier_node_t* id_node, data_type_t type) ;

identifier_t createTemporalIdentifier(data_type_t type);
identifier_t createArrayIdentifier(char* name, int size);
identifier_t createIdentifier(char* name);
identifier_node_t* createIdentifierNode(identifier_t* id, identifier_node_t* next);

identifier_t assign(identifier_t* id, operand_t* operand);
identifier_t assign_array(identifier_t* id, literal_t* index, operand_t* operand);
void generateAssignCode(identifier_t* id, operand_t* operand);
void generateDisplacedAssignCode(identifier_t* id, literal_t* index, operand_t* operand);
identifier_t getIdentifier(char* name);
void expression(operand_t* operand);