#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include "DBG.h"
#include "calc.h"

extern char *yytext;

static char *compile_error_message[] = {
    "internal error: bad error message(0)",
    "parse error.",
    "character invalid. ",
    "function multiple define error. ",
    "bad function call error. ",
    "internal error: bad error message(MAX)"
};

static char *runtime_error_message[] = {
    "internal error: bad error message(0)",
    "variable not found. ",
    "function not found error. ",
    "boolean expected error. ",
    "argument too many error. ",
    "argument too few error. ",
    "internal error: bad error message(MAX)"
};

void
clc_compile_error(CompileError id, char *fmt,...)
{
    va_list	ap;

    va_start(ap,fmt);
    if (fmt) {
	fprintf(stderr,"%s ", compile_error_message[id]);
	vfprintf(stderr,fmt,ap);
    } else {
	fprintf(stderr,"%s\n", compile_error_message[id]);
    }
    va_end(ap);
}

void
clc_runtime_error(RuntimeError id, char *fmt,...)
{
    va_list	ap;

    va_start(ap,fmt);
    if (fmt) {
	fprintf(stderr,"%s ", runtime_error_message[id]);
	vfprintf(stderr,fmt,ap);
    } else {
	fprintf(stderr,"%s\n", runtime_error_message[id]);
    }
    va_end(ap);

    longjmp(clc_current_interpreter->error_recovery_environment,
	    id);
}

int
yyerror(char const *str)
{
    clc_compile_error(PARSE_ERR,"near token %s\n", yytext);

    return 0;
}

