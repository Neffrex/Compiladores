#pragma once


#define ERR_MSG_INVALID_ARGUMENT_TYPE "[Semantic Error] Invalid argument type %s for argument #%d on %s, expected %s\n"
#define ERR_MSG_INVALID_FUNCTION_IDENTIFIER "[Semantic Error] Invalid function identifier %d\n"

#define ERR_MSG_IDENTIFIER_NOT_DECLARED "[Semantic Error] Cannot find declaration of identifier '%s', first use in line %d\n"
#define ERR_MSG_INVALID_OPERATION "[Semantic Error] Invalid Operation: %s %s %s\n"
#define ERR_MSG_INVALID_OPERAND_TYPES "[Semantic Error] Invalid operand types for: %s %s %s\n"
#define ERR_MSG_INVALID_OPERAND_TYPE "[Semantic Error] Invalid operand type %s for operation %s\n"

#define ERR_MSG_INVALID_IDENTIFIER_TYPE "[Semantic Error] Invalid identifier type %s for operation %s"
#define ERR_MSG_IDENTIFIER_ALREADY_DECLARED "[Semantic Error] Identifier already declared: Identifier %s was already declared in line <%d>"
#define ERR_MSG_TYPE_MISMATCH "[Semantic Error] Type mismatch: Cannot assign value %s with type %s to identifier %s with type %s\n"

#define ERR_MSG_MEMORY_ALLOCATION_FAILED "[Fatal Error] Memory allocation failed\n"
#define ERR_MSG_UNDEFINED_SEQUENCE "[Lexical Error] undefined sequence: %s\n"

#define ERR_MSG_DIVISION_BY_ZERO "[Semantic Error] Division by zero\n"

// IDENTIFIERS
#define LOG_MSG_IDENTIFIER_RETRIEVED "[       Retrieved       ]: %s:%s = %s:%s"
#define LOG_MSG_UNDEFINED_IDENTIFIER_RETRIEVED "[       Retrieved       ]: %s:%s (Unassigned)"
#define LOG_MSG_IDENTIFIER_DECLARED "[      Declaration      ]: %s:%s"
#define LOG_MSG_IDENTIFIER_ARRAY_DECLARED "[      Declaration      ]: %s:%s[%d]"
#define LOG_MSG_IDENTIFIER_ASSIGNED "[      Assignation      ]: %s := %s:%s"

// ARITHMETIC EXPRESSIONS
#define LOG_MSG_ARITHMETIC_EXPRESSION_UNARY_ENTRY "[ Arithmetic Expression ]: %s %s:%s"
#define LOG_MSG_ARITHMETIC_EXPRESSION_BINARY_ENTRY "[ Arithmetic Expression ]: %s:%s %s %s:%s"
#define LOG_MSG_ARITHMETIC_EXPRESSION_UNARY_TMP "[ Arithmetic Expression ]: %s := %s %s:%s"
#define LOG_MSG_ARITHMETIC_EXPRESSION_BINARY_TMP "[ Arithmetic Expression ]: %s := %s:%s %s %s:%s"
#define LOG_MSG_ARITHMETIC_EXPRESSION_BINARY_RESOLUTION "[ Arithmetic Expression ]: %s:%s %s %s:%s = %s:%s"
#define LOG_MSG_ARITHMETIC_EXPRESSION_UNARY_RESOLUTION "[ Arithmetic Expression ]: %s %s:%s = %s:%s"

// BOOLEAN EXPRESSIONS
#define LOG_MSG_BOOLEAN_UNARY_EXPRESSION  "[   Boolean Expression  ]: %s %s:%s"
#define LOG_MSG_BOOLEAN_BINARY_EXPRESSION "[   Boolean Expression  ]: %s:%s %s %s:%s"
#define LOG_MSG_END_OF_BOOLEAN_EXPRESSION "[   Boolean Expression  ]: %s"

// RELATIONAL EXPRESSIONS
#define LOG_MSG_RELATIONAL_EXPRESSION "[ Relational Expression ]: %s:%s %s %s:%s"
#define LOG_MSG_END_OF_RELATIONAL_EXPRESSION "[ Relational Expression ]: %s\n"

// END OF STATEMENTS/PROGRAM
#define LOG_MSG_END_OF_STATEMENT "• %02d: End of statement\n>> %s"
#define LOG_MSG_END_OF_PROGRAM "• %02d: END OF FILE\n"
