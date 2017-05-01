#include <stdio.h>
#include "DBG.h"
#include "calc.h"

void
clc_function_define(char *identifier, ParameterList *parameter_list,
		    Expression *expression_list)
{
    FunctionDefinition *f;

    if (clc_search_function(identifier)) {
	clc_compile_error(FUNCTION_MULTIPLE_DEFINE_ERR, "(%s)\n", identifier);
	clc_reopen_current_storage();
	return;
    }
    f = clc_malloc(sizeof(FunctionDefinition));
    f->name = identifier;
    f->parameter = parameter_list;
    f->expression_list = expression_list;
    f->storage = clc_current_interpreter->current_storage;
    clc_current_interpreter->current_storage = MEM_open_storage(0);

    f->next = clc_current_interpreter->function_list;
    clc_current_interpreter->function_list = f;
}

ParameterList*
clc_create_parameter(char *identifier)
{
    ParameterList	*p;

    p = clc_malloc(sizeof(ParameterList));
    p->name = identifier;
    p->next = NULL;

    return p;
}

ParameterList *
chain_parameter(ParameterList *list, ParameterList *add)
{
    ParameterList *pos;

    for (pos = list; pos->next; pos = pos->next)
	;
    pos->next = add;

    return list;
}

ParameterList*
clc_chain_parameter(ParameterList *list,  char *identifier)
{
    ParameterList	*add;

    add = clc_create_parameter(identifier);

    return chain_parameter(list, add);
}

Expression *
clc_alloc_expression(ExpressionType type)
{
    Expression	*exp;

    exp = clc_malloc(sizeof(Expression));
    exp->type = type;

    return exp;
}

Expression *
clc_create_expression_list(Expression *expression)
{
    Expression	*exp;

    exp = clc_alloc_expression(EXPRESSION_LIST_EXPRESSION);
    exp->u.expression_list.expression = expression;
    exp->u.expression_list.next = NULL;

    return exp;
}

Expression *
clc_chain_expression_list(Expression *list, Expression *add)
{
    Expression	*exp;
    Expression	*pos;

    exp = clc_alloc_expression(EXPRESSION_LIST_EXPRESSION);
    exp->u.expression_list.expression = add;
    exp->u.expression_list.next = NULL;

    for (pos = list; pos->u.expression_list.next;
	 pos = pos->u.expression_list.next)
	;
    pos->u.expression_list.next = exp;

    return list;
}

Expression *
clc_create_assign_expression(char *variable,
			     Expression *operand)
{
    Expression *exp;

    exp = clc_alloc_expression(ASSIGN_EXPRESSION);
    exp->u.assign_expression.variable = variable;
    exp->u.assign_expression.operand = operand;

    return exp;
}

static Expression
convert_value_to_expression(Value *v)
{
    Expression	expr;

    if (v->type == INT_VALUE) {
	expr.type = INT_EXPRESSION;
	expr.u.int_value = v->u.int_value;
    } else {
	DBG_assert(v->type == DOUBLE_VALUE, ("v->type..%d\n", v->type));
	expr.type = DOUBLE_EXPRESSION;
	expr.u.double_value = v->u.double_value;
    }
    return expr;
}

Expression *
clc_create_binary_expression(ExpressionType operator,
			     Expression *left,
			     Expression *right)
{
    if ((left->type == INT_EXPRESSION
	 || left->type == DOUBLE_EXPRESSION)
	&& (right->type == INT_EXPRESSION
	    || right->type == DOUBLE_EXPRESSION)) {
	Value v;
	v = clc_eval_binary_expression(NULL, operator, left, right);
	/* Overwriting left hand expression. */
	*left = convert_value_to_expression(&v);

	return left;
    } else {
	Expression *exp;
	exp = clc_alloc_expression(operator);
	exp->u.binary_expression.left = left;
	exp->u.binary_expression.right = right;
	return exp;
    }
}

Expression *
clc_create_minus_expression(Expression *operand)
{
    if (operand->type == INT_EXPRESSION
	|| operand->type == DOUBLE_EXPRESSION) {
	Value	v;
	v = clc_eval_minus_expression(NULL, operand);
	/* Notice! Overwriting operand expression. */
	*operand = convert_value_to_expression(&v);
	return operand;
    } else {
	Expression	*exp;
	exp = clc_alloc_expression(MINUS_EXPRESSION);
	exp->u.minus_expression = operand;
	return exp;
    }
}

Expression *
clc_create_identifier_expression(char *identifier)
{
    Expression	*exp;

    exp = clc_alloc_expression(IDENTIFIER_EXPRESSION);
    exp->u.identifier = identifier;

    return exp;
}

Expression *
clc_create_if_expression(Expression *condition,
			 Expression *then_expression,
			 Expression *else_expression)
{
    Expression	*exp;

    exp = clc_alloc_expression(IF_EXPRESSION);
    exp->u.if_expression.condition = condition;
    exp->u.if_expression.then_expression = then_expression;
    exp->u.if_expression.else_expression = else_expression;

    return exp;
}

Expression *
clc_create_while_expression(Expression *condition,
			    Expression *expression)
{
    Expression	*exp;

    exp = clc_alloc_expression(WHILE_EXPRESSION);
    exp->u.while_expression.condition = condition;
    exp->u.while_expression.expression_list = expression;

    return exp;
}

Expression *
clc_create_function_call_expression(char *func_name,
				    Expression *argument)
{
    Expression	*exp;

    exp = clc_alloc_expression(FUNCTION_CALL_EXPRESSION);
    exp->u.function_call_expression.identifier = func_name;
    exp->u.function_call_expression.argument = argument;

    return exp;
}
