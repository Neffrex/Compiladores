#include "headers/identifier.h"

identifier_node_t* declare(identifier_node_t* id_node, data_type_t type) 
{	
  for(identifier_node_t* current = id_node; current != NULL; current = current->next)
  {
    if(sym_lookup(current->id.name, &current->id) == SYMTAB_NOT_FOUND)
    {	
      current->id.type = type;
      sym_enter(current->id.name, &current->id);
      log_message(LOG_INFO, LOG_MSG_IDENTIFIER_DECLARED, current->id.name, type2str(current->id.type));
    } else 
    {	
      // Identifier already declared
      halt(ERR_MSG_IDENTIFIER_ALREADY_DECLARED, current->id.name, current->id.declaration_lineno);
    }
  }
  return id_node;
}

identifier_t createTemporalIdentifier(data_type_t type)
{
  static int counter = 1;
  char buffer[STR_MAX_LENGTH];
  snprintf(buffer, STR_MAX_LENGTH, "$t%d", counter++);
  identifier_t id = { .name = strdup(buffer), .type = type, .value = createEmptyLiteral(), .declaration_lineno = code_lineno };
  return id;
}

identifier_node_t* createIdentifierNode(identifier_node_t* head, identifier_t* id, data_type_t type)
{
  identifier_node_t* new_node = (identifier_node_t*) malloc (sizeof(identifier_node_t));
  new_node->id.name = strdup(id->name);
  new_node->id.type = type;
  new_node->id.declaration_lineno = id->declaration_lineno;
  new_node->next = head;
  return new_node;
}

void logAssignEntry(identifier_t* id, operand_t* operand)
{
  log_message(LOG_INFO, LOG_MSG_IDENTIFIER_ASSIGNED, id->name, operand2str(operand), type2str(getOperandDataType(operand)));
}

identifier_t assign(identifier_t* id, operand_t* operand)
{
  logAssignEntry(id, operand);

	data_type_t type = getOperandDataType(operand);

  if(id->type != type)
  { halt(ERR_MSG_TYPE_MISMATCH, operand2str(operand), type2str(type), id->name, type2str(id->type)); }

  // id->value = value;
	generateAssignCode(id, operand);
  sym_enter(id->name, id);

  return *id;
}

void generateAssignCode(identifier_t* id, operand_t* operand)
{
	fprintf(yyout, "%d: %s := %s\n", code_lineno++, id->name, operand2str(operand));
}

identifier_t getIdentifier(char* name)
{
  identifier_t* id = (identifier_t*) malloc(sizeof(identifier_t));
  if(sym_lookup(name, id) != SYMTAB_NOT_FOUND)
  { 
    char *svalue = literal2str(&id->value);
    log_message(LOG_INFO, LOG_MSG_IDENTIFIER_RETRIEVED, id->name, svalue, type2str(id->type));
    free(svalue);
  } 
  identifier_t result = *id;
  free(id);
  return result;
}

void expression(operand_t* operand)
{
	fprintf(yyout, "%d: PARAM %s\n", code_lineno++, operand2str(operand));
	fprintf(yyout, "%d: CALL %s,1\n", code_lineno++, getOperandDataType(operand) == TYPE_INTEGER? "PUTI" : "PUTF");
}