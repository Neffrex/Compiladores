#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "log.h"

#define STR_MAX_LENGTH 256
#define MAX_VERBOSE_SIZE 64

// EMPTY VALUES
#define S_UNDEFINED "undefined"
#define S_NULL "null"
#define S_NAN "NaN"

// TYPES
#define S_INTEGER "integer"
#define S_FLOAT "float"
#define S_ARRAY "array"

// OPERATORS
#define S_NEGATE "negate"
#define S_PLUS "+"
#define S_MINUS "-"
#define S_TIMES "*"
#define S_DIVIDE "/"
#define S_MOD "%"
#define S_POW "**"
#define S_F2I "F2I"
#define S_I2F "I2F"


// C3A
#define CODE_NEGATE "CHS"
#define CODE_PLUS "ADD"
#define CODE_MINUS "SUB"
#define CODE_TIMES "MUL"
#define CODE_DIVIDE "DIV"
#define CODE_MOD "MOD"
#define CODE_POW "<POW:undefined>"
#define CODE_F2I "F2"
#define CODE_I2F "I2"
#define CODE_HALT "HALT"
#define CODE_UNDEFINED "<undefined>"

#define CODE_INTEGER_SUFIX "I"
#define CODE_FLOAT_SUFIX "F"

// Literals
typedef enum data_type_t {
	TYPE_UNDEFINED,
	TYPE_INTEGER,
	TYPE_FLOAT,
	TYPE_ARRAY
} data_type_t;

typedef struct {
	data_type_t type;
	union {
		int ivalue;
		float fvalue;
	};	
} literal_t;

// Identifiers
typedef struct {
	char* name;
	data_type_t type;
	union {
		literal_t value;
		struct {
			literal_t* members;
			data_type_t members_type;
			int size;
		};
	};
	int declaration_lineno;
} identifier_t;

typedef struct identifier_node_t {
	identifier_t id;
  struct identifier_node_t* next;
} identifier_node_t;


// Operands
typedef enum {
	OPERAND_UNDEFINED,
	OPERAND_LITERAL,
	OPERAND_IDENTIFIER
} operand_type_t;

typedef struct {
	operand_type_t type;
	union {
		literal_t literal;
		identifier_t identifier;
	};
} operand_t;



// Operators
typedef enum {
  OP_UNDEFINED,
  OP_NEGATE,
  OP_PLUS,
  OP_MINUS,
  OP_TIMES,
  OP_DIVIDE,
  OP_MOD,
  OP_POW,
	OP_I2F,
	OP_F2I
} op_type_t;

bool isNullLiteral(literal_t* literal);
bool isInteger(literal_t* literal);
bool isFloat(literal_t* literal);
bool isOperandInteger(operand_t* operand);
bool isOperandFloat(operand_t* operand);
char* literal2str(literal_t* literal);
const char* type2str(data_type_t type);
const char* op2str(op_type_t op);
const data_type_t getOperandDataType(operand_t* operand);
const char* operand2str(operand_t* operand);
const char* op2code(op_type_t op);