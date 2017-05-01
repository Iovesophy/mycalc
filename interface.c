#include "MEM.h"
#define GLOBAL_VARIABLE_DEFINE
#include "calc.h"

CLC_Interpreter
CLC_create_interpreter(void)
{
    MEM_Storage	storage = MEM_open_storage(0);

    CLC_Interpreter interpreter
	= MEM_storage_malloc(storage, sizeof(struct CLC_Interpreter_tag));
    interpreter->input_mode = CLC_TTY_INPUT_MODE;
    interpreter->input_fp = NULL;
    interpreter->current_storage = NULL;
    interpreter->global_storage = storage;
    interpreter->variable = NULL;
    interpreter->function_list = NULL;

    return interpreter;
}

void
CLC_interpret(CLC_Interpreter interpreter, FILE *fp, CLC_InputMode input_mode)
{
    extern int yyparse(void);

    interpreter->input_mode = input_mode;
    interpreter->input_fp = fp;

    interpreter->current_storage = MEM_open_storage(0);
    clc_current_interpreter = interpreter;

    if (yyparse()) {
	fprintf(stderr, "Error ! Error ! Error !\n");
	exit(1);
    }

    if (interpreter->current_storage) {
	MEM_dispose_storage(interpreter->current_storage);
    }
}

void
CLC_dispose_interpreter(CLC_Interpreter interpreter)
{
    MEM_dispose_storage(interpreter->current_storage);

    while (interpreter->function_list) {
	FunctionDefinition	*temp;
	temp = interpreter->function_list->next;
	MEM_dispose_storage(interpreter->function_list->storage);
	interpreter->function_list = temp;
    }
}
