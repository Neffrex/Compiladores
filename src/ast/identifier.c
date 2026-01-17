#include "headers/identifier.h"

identifier_node_t* declare(identifier_node_t* id_node, data_type_t type) 
{	
  for(identifier_node_t* current = id_node; current != NULL; current = current->next)
  {
    if(sym_lookup(current->id.name, &current->id) == SYMTAB_NOT_FOUND)
    {	
			if(current->id.type == TYPE_ARRAY)
			{ current->id.members_type = type; }
			else
			{ current->id.type = type; }
      
      sym_enter(current->id.name, &current->id);
			if(current->id.type == TYPE_ARRAY)
			{	log_message(LOG_MSG_IDENTIFIER_ARRAY_DECLARED, current->id.name, type2str(current->id.members_type), current->id.size); }
      else
			{ log_message(LOG_MSG_IDENTIFIER_DECLARED, current->id.name, type2str(current->id.type)); }
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
  snprintf(buffer, STR_MAX_LENGTH, "$t%02d", counter++);
  identifier_t id = { .name = strdup(buffer), .type = type, .value = createEmptyLiteral(), .declaration_lineno = code_lineno };
  return id;
}

identifier_t createArrayIdentifier(char* name, int size)
{
	identifier_t result;
	result.name = name;
	result.type = TYPE_ARRAY;
	result.value = createEmptyLiteral();
	result.members = NULL;
	result.members_type = TYPE_UNDEFINED;
	result.size = size;
	result.declaration_lineno = yylineno-1;
	return result;
}

identifier_t createIdentifier(char* name)
{
	identifier_t result;
	result.name = name;
	result.type = TYPE_UNDEFINED;
	result.value = createEmptyLiteral();
	result.declaration_lineno = yylineno-1;
	return result;
}

identifier_node_t* createIdentifierNode(identifier_t* id, identifier_node_t* next)
{
  identifier_node_t* new_node = (identifier_node_t*) malloc (sizeof(identifier_node_t));
  new_node->id = *id;
  new_node->next = next;
  return new_node;
}

identifier_t assign(identifier_t* id, operand_t* operand)
{
	data_type_t type = getOperandDataType(operand);

  if(id->type != type)
  { halt(ERR_MSG_TYPE_MISMATCH, operand2str(operand), type2str(type), id->name, type2str(id->type)); }

	log_message(LOG_MSG_IDENTIFIER_ASSIGNED, id->name, operand2str(operand), type2str(getOperandDataType(operand)));
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
		if(id->value.type == TYPE_UNDEFINED)
		{ log_message(LOG_MSG_UNDEFINED_IDENTIFIER_RETRIEVED, id->name, type2str(id->type)); }
		else {
			char *svalue = literal2str(&id->value);
			log_message(LOG_MSG_IDENTIFIER_RETRIEVED, id->name, type2str(id->type), svalue, type2str(id->value.type));
			free(svalue);
		}
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