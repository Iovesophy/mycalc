#ifndef PUBLIC_CLC_H_INCLUDED
#define PUBLIC_CLC_H_INCLUDED
#include <stdio.h>

typedef struct CLC_Interpreter_tag *CLC_Interpreter;

typedef enum {
    CLC_TTY_INPUT_MODE = 1,
    CLC_FILE_INPUT_MODE
} CLC_InputMode;

CLC_Interpreter CLC_create_interpreter(void);
void CLC_interpret(CLC_Interpreter interpreter, FILE *fp,
		   CLC_InputMode input_mode);
void CLC_dispose_interpreter(CLC_Interpreter interpreter);

#endif PUBLIC_CLC_H_INCLUDED
