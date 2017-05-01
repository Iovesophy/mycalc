#include <stdio.h>
#include "CLC.h"

int
main(int argc, char **argv)
{
    CLC_Interpreter	interpreter;

    interpreter = CLC_create_interpreter();
    CLC_interpret(interpreter, stdin, CLC_TTY_INPUT_MODE);
    CLC_dispose_interpreter(interpreter);

    return 0;
}
