#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "log.h"

#define STR_MAX_LENGTH 256
#define MAX_VERBOSE_SIZE 64

// EMPTY VALUES
#define S_UNDEFINED "UNDEFINED"
#define S_NULL "NULL"
#define S_NAN "NaN"

// TYPES
#define S_INTEGER "integer"
#define S_FLOAT "float"

// OPERATORS
#define S_NEGATE "negate"
#define S_PLUS "+"
#define S_MINUS "-"
#define S_TIMES "*"
#define S_DIVIDE "/"
#define S_MOD "%"
#define S_POW "**"


typedef enum {
  TYPE_UNDEFINED,
  TYPE_INTEGER,
  TYPE_FLOAT
} type_t;

typedef enum {
  OP_UNDEFINED,
  OP_NEGATE,
  OP_PLUS,
  OP_MINUS,
  OP_TIMES,
  OP_DIVIDE,
  OP_MOD,
  OP_POW,
} op_type;

typedef struct {
  type_t type;
	union {
		int ivalue;
		float fvalue;
	};	
} literal;

typedef struct {
	char* name;
	type_t type;
	literal value;
	int lineno;
} identifier;

typedef struct identifier_node {
  identifier id;
  struct identifier_node* next;
} identifier_node;

bool isNullLiteral(literal*);
bool isInteger(literal*);
bool isFloat(literal*);
char* literal2str(literal*);
const char* type2str(type_t);
const char* op2str(op_type);
