#include "calc.h"

void *
clc_malloc(size_t size)
{
    void *p;

    p = MEM_storage_malloc(clc_current_interpreter->current_storage, size);

    return p;
}

void *
clc_global_malloc(size_t size)
{
    void *p;

    p = MEM_storage_malloc(clc_current_interpreter->global_storage, size);

    return p;
}

Value *
clc_search_local_variable(LocalEnvironment *env, char *identifier)
{
    Variable	*pos;

    if (env == NULL)
	return NULL;
    for (pos = env->variable; pos; pos = pos->next) {
	if (!strcmp(pos->name, identifier))
	    break;
    }
    if (pos == NULL) {
	return NULL;
    } else {
	return &pos->value;
    }
}

Value *
clc_search_global_variable(char *identifier)
{
    Variable	*pos;

    for (pos = clc_current_interpreter->variable;
	 pos; pos = pos->next) {
	if (!strcmp(pos->name, identifier))
	    break;
    }
    if (pos == NULL) {
	return NULL;
    } else {
	return &pos->value;
    }
}

void
clc_add_local_variable(LocalEnvironment *env,
		       char *identifier, Value *value)
{
    Variable	*new_variable;

    new_variable = MEM_malloc(sizeof(Variable));
    new_variable->name = identifier;
    new_variable->value = *value;
    new_variable->next = env->variable;
    env->variable = new_variable;
}

void
clc_add_global_variable(char *identifier, Value *value)
{
    Variable	*new_variable;

    new_variable = clc_global_malloc(sizeof(Variable));
    new_variable->name = clc_global_malloc(strlen(identifier) + 1);
    strcpy(new_variable->name, identifier);
    new_variable->value = *value;

    new_variable->next = clc_current_interpreter->variable;
    clc_current_interpreter->variable = new_variable;
}

FunctionDefinition *
clc_search_function(char *name)
{
    FunctionDefinition *pos;
    for (pos = clc_current_interpreter->function_list;
	 pos; pos = pos->next) {
	if (!strcmp(pos->name, name))
	    break;
    }
    return pos;
}

void
clc_reopen_current_storage(void)
{
    MEM_dispose_storage(clc_current_interpreter->current_storage);
    clc_current_interpreter->current_storage = MEM_open_storage(0);
}
