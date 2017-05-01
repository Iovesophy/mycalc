#ifndef PRIVATE_CALC_H_INCLUDED
#define PRIVATE_CALC_H_INCLUDED
#include <stdio.h>
#include <setjmp.h>
#include "MEM.h"
#include "CLC.h"

#define smaller(a, b) ((a) < (b) ? (a) : (b))
#define larger(a, b) ((a) > (b) ? (a) : (b))

typedef enum {
    PARSE_ERR = 1,
    CHARACTER_INVALID_ERR,
    FUNCTION_MULTIPLE_DEFINE_ERR,
    COMPILE_ERROR_NUM
} CompileError;

typedef enum {
    VARIABLE_NOT_FOUND_ERR = 1,
    FUNCTION_NOT_FOUND_ERR,
    BOOLEAN_EXPECTED_ERR,
    ARGUMENT_TOO_MANY_ERR,
    ARGUMENT_TOO_FEW_ERR,
    RUNTIME_ERROR_NUM
} RuntimeError;

typedef struct Expression_tag Expression;

typedef enum {
    INT_EXPRESSION = 1,
    DOUBLE_EXPRESSION,
    IDENTIFIER_EXPRESSION,
    EXPRESSION_LIST_EXPRESSION,
    ASSIGN_EXPRESSION,
    ADD_EXPRESSION,
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    EQ_EXPRESSION,
    NE_EXPRESSION,
    GT_EXPRESSION,
    GE_EXPRESSION,
    LT_EXPRESSION,
    LE_EXPRESSION,
    MINUS_EXPRESSION,
    IF_EXPRESSION,
    WHILE_EXPRESSION,
    FUNCTION_CALL_EXPRESSION,
    EXPRESSION_TYPE_NUM
} ExpressionType;

typedef struct {
    Expression *expression;
    Expression *next;
} ExpressionList;

typedef struct {
    char	*variable;
    Expression	*operand;
} AssignExpression;

typedef struct {
    Expression	*left;
    Expression	*right;
} BinaryExpression;

typedef struct {
    Expression	*condition;
    Expression	*then_expression;
    Expression	*else_expression;
} IfExpression;

typedef struct {
    Expression	*condition;
    Expression	*expression_list;
} WhileExpression;

typedef struct {
    char	*identifier;
    Expression	*argument;
} FunctionCallExpression;

struct Expression_tag {
    ExpressionType type;
    union {
	int			int_value;
	double			double_value;
	char			*identifier;
	ExpressionList		expression_list;
	AssignExpression	assign_expression;
	BinaryExpression	binary_expression;
	Expression		*minus_expression;
	IfExpression		if_expression;
	FunctionCallExpression	function_call_expression;
	WhileExpression		while_expression;
    } u;
};

typedef struct ParameterList_tag {
    char	*name;
    struct ParameterList_tag *next;
} ParameterList;

typedef struct FunctionDefinition_tag {
    char		*name;
    ParameterList	*parameter;
    Expression		*expression_list;
    MEM_Storage		storage;
    struct FunctionDefinition_tag	*next;
} FunctionDefinition;

typedef enum {
    INT_VALUE = 1,
    DOUBLE_VALUE,
    NULL_VALUE
} ValueType;

typedef struct {
    ValueType	type;
    union {
	int	int_value;
	double	double_value;
    } u;
} Value;

typedef struct Variable_tag {
    char	*name;
    Value	value;
    struct Variable_tag	*next;
} Variable;

typedef struct {
    Variable	*variable;
} LocalEnvironment;

struct CLC_Interpreter_tag {
    CLC_InputMode	input_mode;
    FILE		*input_fp;
    MEM_Storage		global_storage;
    MEM_Storage		current_storage;
    Variable		*variable;
    FunctionDefinition	*function_list;
    jmp_buf		error_recovery_environment;

};

#ifdef GLOBAL_VARIABLE_DEFINE
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL CLC_Interpreter clc_current_interpreter;

/* calc.l */
void clc_initialize_readline_buffer(void);

/* create.c */
void clc_function_define(char *identifier, ParameterList *parameter_list,
			 Expression *expression_list);
ParameterList *clc_create_parameter(char *identifier);
ParameterList *clc_chain_parameter(ParameterList *list,
				   char *identifier);
Expression *clc_alloc_expression(ExpressionType type);
Expression *clc_create_expression_list(Expression *expression);
Expression *clc_chain_expression_list(Expression *list, Expression *add);
Expression *clc_create_assign_expression(char *variable,
					 Expression *operand);
Expression *clc_create_binary_expression(ExpressionType operator,
					 Expression *left,
					 Expression *right);
Expression *clc_create_minus_expression(Expression *operand);
Expression *clc_create_identifier_expression(char *identifier);
Expression *clc_create_if_expression(Expression *condition,
				     Expression *then_expression,
				     Expression *else_expression);
Expression *clc_create_while_expression(Expression *condition,
					Expression *expression);
Expression *clc_create_function_call_expression(char *func_name,
						Expression *expression);

/* eval.c */
void  clc_eval_expression(Expression *expr);
Value clc_eval_binary_expression(LocalEnvironment *env,
				 ExpressionType operator,
				 Expression *left, Expression *right);
Value clc_eval_minus_expression(LocalEnvironment *env, Expression *operand);

/* util.c */
void clc_print_expression(Expression *expression);
void *clc_malloc(size_t size);
Value *clc_search_local_variable(LocalEnvironment *env,
				 char *identifier);
Value *clc_search_global_variable(char *identifier);
Value *clc_search_local_variable(LocalEnvironment *env,
				 char *identifier);
Value *clc_search_global_variable(char *identifier);
void clc_add_local_variable(LocalEnvironment *env,
			    char *identifier, Value *value);
void clc_add_global_variable(char *identifier, Value *value);
FunctionDefinition *clc_search_function(char *name);
void clc_reopen_current_storage(void);

/* error.c */
void clc_compile_error(CompileError id, char *fmt, ...);
void clc_runtime_error(RuntimeError id, char *fmt, ...);

#endif /* PRIVATE_CALC_H_INCLUDED */
