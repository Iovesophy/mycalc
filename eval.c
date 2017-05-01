#include <stdio.h>
#include <math.h>
#include "MEM.h"
#include "DBG.h"
#include "calc.h"

static Value
eval_int_expression(int int_value)
{
    Value	v;

    v.type = INT_VALUE;
    v.u.int_value = int_value;

    return v;
}

static Value
eval_double_expression(double double_value)
{
    Value	v;

    v.type = DOUBLE_VALUE;
    v.u.double_value = double_value;

    return v;
}

static Value
eval_identifier_expression(LocalEnvironment *env, char *identifier)
{
    Value	v;
    Value	*vp;

    vp = clc_search_local_variable(env, identifier);
    if (vp != NULL) {
	v = *vp;
	return v;
    }
    vp = clc_search_global_variable(identifier);
    if (vp != NULL) {
	v = *vp;
	return v;
    }
    clc_runtime_error(VARIABLE_NOT_FOUND_ERR, "(%s)\n", identifier);
    return v; /* dummy. make compiler happy. */
}

static Value eval_expression(LocalEnvironment *env, Expression *expr);

static Value
eval_expression_list_expression(LocalEnvironment *env,
				Expression *expression, Expression *next)
{
    Value	v;

    v = eval_expression(env, expression);
    if (next) {
	v = eval_expression(env, next);
    }

    return v;
}

static Value
eval_assign_expression(LocalEnvironment *env,
		       char *identifier, Expression *expression)
{
    Value	v;
    Value	*left;

    v = eval_expression(env, expression);

    left = clc_search_local_variable(env, identifier);
    if (left == NULL) {
	left = clc_search_global_variable(identifier);
    }
    if (left != NULL) {
	/* Notice! Overwriting variable by pointer. */
	*left = v;
    } else {
	if (env != NULL) {
	    clc_add_local_variable(env, identifier, &v);
	} else {
	    clc_add_global_variable(identifier, &v);
	}
    }
    return v;
}

static int
eval_binary_int(ExpressionType operator, int left, int right)
{
    int	result;

    switch (operator) {
      case INT_EXPRESSION:	/* FALLTHRU */
      case DOUBLE_EXPRESSION:	/* FALLTHRU */
      case IDENTIFIER_EXPRESSION:	/* FALLTHRU */
      case EXPRESSION_LIST_EXPRESSION:	/* FALLTHRU */
      case ASSIGN_EXPRESSION:
	DBG_assert(0, ("bad case...%d", operator));
	break;
      case ADD_EXPRESSION:
	result = left + right;
	break;
      case SUB_EXPRESSION:
	result = left - right;
	break;
      case MUL_EXPRESSION:
	result = left * right;
	break;
      case DIV_EXPRESSION:
	result = left / right;
	break;
      case MOD_EXPRESSION:
	result = left % right;
	break;
      case EQ_EXPRESSION:
	result = left == right;
	break;
      case NE_EXPRESSION:
	result = left != right;
	break;
      case GT_EXPRESSION:
	result = left > right;
	break;
      case GE_EXPRESSION:
	result = left >= right;
	break;
      case LT_EXPRESSION:
	result = left < right;
	break;
      case LE_EXPRESSION:
	result = left <= right;
	break;
      case MINUS_EXPRESSION:	/* FALLTHRU */
      case IF_EXPRESSION:	/* FALLTHRU */
      case WHILE_EXPRESSION:	/* FALLTHRU */
      case FUNCTION_CALL_EXPRESSION:	/* FALLTHRU */
      case EXPRESSION_TYPE_NUM:	/* FALLTHRU */
      default:
	DBG_assert(0, ("bad case...%d", operator));
    }
    return result;
}

static void
eval_binary_double(ExpressionType operator, double left, double right,
		   Value *result)
{
    if (operator == ADD_EXPRESSION || operator == SUB_EXPRESSION
	|| operator == MUL_EXPRESSION || operator == DIV_EXPRESSION
	|| operator == MOD_EXPRESSION) {
	result->type = DOUBLE_VALUE;
    } else {
	DBG_assert(operator == EQ_EXPRESSION || operator == NE_EXPRESSION
		   || operator == GT_EXPRESSION || operator == GE_EXPRESSION
		   || operator == LT_EXPRESSION || operator == LE_EXPRESSION,
		   ("operator..%d\n", operator));
	result->type = INT_VALUE;
    }
    switch (operator) {
      case INT_EXPRESSION:	/* FALLTHRU */
      case DOUBLE_EXPRESSION:	/* FALLTHRU */
      case IDENTIFIER_EXPRESSION:	/* FALLTHRU */
      case EXPRESSION_LIST_EXPRESSION:	/* FALLTHRU */
      case ASSIGN_EXPRESSION:
	DBG_assert(0, ("bad case...%d", operator));
	break;
      case ADD_EXPRESSION:
	result->u.double_value = left + right;
	break;
      case SUB_EXPRESSION:
	result->u.double_value = left - right;
	break;
      case MUL_EXPRESSION:
	result->u.double_value = left * right;
	break;
      case DIV_EXPRESSION:
	result->u.double_value = left / right;
	break;
      case MOD_EXPRESSION:
	result->u.double_value = fmod(left, right);
	break;
      case EQ_EXPRESSION:
	result->u.int_value = left == right;
	break;
      case NE_EXPRESSION:
	result->u.int_value = left != right;
	break;
      case GT_EXPRESSION:
	result->u.int_value = left > right;
	break;
      case GE_EXPRESSION:
	result->u.int_value = left >= right;
	break;
      case LT_EXPRESSION:
	result->u.int_value = left < right;
	break;
      case LE_EXPRESSION:
	result->u.int_value = left <= right;
	break;
      case MINUS_EXPRESSION:	/* FALLTHRU */
      case IF_EXPRESSION:	/* FALLTHRU */
      case WHILE_EXPRESSION:	/* FALLTHRU */
      case FUNCTION_CALL_EXPRESSION:	/* FALLTHRU */
      case EXPRESSION_TYPE_NUM:	/* FALLTHRU */
      default:
	DBG_assert(0, ("bad default...%d", operator));
    }
}

Value
clc_eval_binary_expression(LocalEnvironment *env,
			   ExpressionType operator,
			   Expression *left, Expression *right)
{
    Value	left_val;
    Value	right_val;
    Value	result;

    left_val = eval_expression(env, left);
    right_val = eval_expression(env, right);

    if (left_val.type == INT_VALUE
	&& right_val.type == INT_VALUE) {
	result.type = INT_VALUE;
	result.u.int_value
	    = eval_binary_int(operator,
			      left_val.u.int_value, right_val.u.int_value);
    } else if (left_val.type == DOUBLE_VALUE
	       && right_val.type == DOUBLE_VALUE) {
	eval_binary_double(operator,
			   left_val.u.double_value, right_val.u.double_value,
			   &result);
    } else {
	/* cast int to double */
	if (left_val.type == INT_VALUE) {
	    left_val.u.double_value = left_val.u.int_value;
	} else {
	    right_val.u.double_value = right_val.u.int_value;
	}
	eval_binary_double(operator,
			   left_val.u.double_value, right_val.u.double_value,
			   &result);
    }
    return result;
}

Value
clc_eval_minus_expression(LocalEnvironment *env, Expression *operand)
{
    Value	operand_val;
    Value	result;

    operand_val = eval_expression(env, operand);
    if (operand_val.type == INT_VALUE) {
	result.type = INT_VALUE;
	result.u.int_value = -operand_val.u.int_value;
    } else {
	DBG_assert(operand_val.type == DOUBLE_VALUE,
		   ("operand_val.type..%d", operand_val.type));
	result.type = DOUBLE_VALUE;
	result.u.double_value = -operand_val.u.double_value;
    }
    return result;
}

static Value
eval_if_expression(LocalEnvironment *env,
		   Expression *condition, Expression *then_expression,
		   Expression *else_expression)
{
    Value	condition_val;
    Value	result;

    condition_val = eval_expression(env, condition);
    if (condition_val.type != INT_VALUE) {
	clc_runtime_error(BOOLEAN_EXPECTED_ERR, NULL);
    }
    if (condition_val.u.int_value) {
	result = eval_expression(env, then_expression);
    } else {
	result = eval_expression(env, else_expression);
    }
    return result;
}

static Value
eval_while_expression(LocalEnvironment *env,
		      Expression *condition, Expression *expression_list)
{
    Value	condition_val;
    Value	result;

    for (;;) {
	condition_val = eval_expression(env, condition);
	if (condition_val.type != INT_VALUE) {
	    clc_runtime_error(BOOLEAN_EXPECTED_ERR, NULL);
	}
	if (!condition_val.u.int_value)
	    break;

	result = eval_expression(env, expression_list);
    }
    return result;
}

static LocalEnvironment *
alloc_local_environment()
{
    LocalEnvironment *ret;

    ret = MEM_malloc(sizeof(LocalEnvironment));
    ret->variable = NULL;

    return ret;
}

static void
dispose_local_environment(LocalEnvironment *env)
{
    while (env->variable) {
	Variable	*temp;
	temp = env->variable->next;
	MEM_free(env->variable);
	env->variable = temp;
    }
    MEM_free(env);
}

static Value
eval_function_call_expression(LocalEnvironment *env,
			      char *identifier, Expression *argument)
{
    Value	result;
    Expression		*arg_p;
    ParameterList	*param_p;
    LocalEnvironment	*local_env;
    FunctionDefinition	*func;
    
    func = clc_search_function(identifier);
    if (func == NULL) {
	clc_runtime_error(FUNCTION_NOT_FOUND_ERR, "name..%s\n", identifier);
    }
    local_env = alloc_local_environment();

    DBG_assert(argument->type == EXPRESSION_LIST_EXPRESSION,
	       ("type..%d\n", argument->type));

    for (arg_p = argument, param_p = func->parameter;
	 arg_p;
	 arg_p = arg_p->u.expression_list.next,
	 param_p = param_p->next) {
	Value arg_val;

	if (param_p == NULL) {
	    clc_runtime_error(ARGUMENT_TOO_MANY_ERR, NULL);
	}
	arg_val = eval_expression(env,
				  arg_p->u.expression_list.expression);
	clc_add_local_variable(local_env, param_p->name, &arg_val);
    }
    if (param_p) {
	clc_runtime_error(ARGUMENT_TOO_FEW_ERR, NULL);
    }
    result = eval_expression(local_env, func->expression_list);

    dispose_local_environment(local_env);

    return result;
}

static Value
eval_expression(LocalEnvironment *env, Expression *expr)
{
    Value	v;
    switch (expr->type) {
      case INT_EXPRESSION:
	v = eval_int_expression(expr->u.int_value);
	break;
      case DOUBLE_EXPRESSION:
	v = eval_double_expression(expr->u.double_value);
	break;
      case IDENTIFIER_EXPRESSION:
	v = eval_identifier_expression(env, expr->u.identifier);
	break;
      case EXPRESSION_LIST_EXPRESSION:
	v = eval_expression_list_expression
	    (env,
	     expr->u.expression_list.expression,
	     expr->u.expression_list.next);
	break;
      case ASSIGN_EXPRESSION:
	v = eval_assign_expression(env,
				   expr->u.assign_expression.variable,
				   expr->u.assign_expression.operand);
	break;
      case ADD_EXPRESSION:	/* FALLTHRU */
      case SUB_EXPRESSION:	/* FALLTHRU */
      case MUL_EXPRESSION:	/* FALLTHRU */
      case DIV_EXPRESSION:	/* FALLTHRU */
      case MOD_EXPRESSION:	/* FALLTHRU */
      case EQ_EXPRESSION:	/* FALLTHRU */
      case NE_EXPRESSION:	/* FALLTHRU */
      case GT_EXPRESSION:	/* FALLTHRU */
      case GE_EXPRESSION:	/* FALLTHRU */
      case LT_EXPRESSION:	/* FALLTHRU */
      case LE_EXPRESSION:	/* FALLTHRU */
	v = clc_eval_binary_expression(env,
				       expr->type,
				       expr->u.binary_expression.left,
				       expr->u.binary_expression.right);
	break;
      case MINUS_EXPRESSION:
	v = clc_eval_minus_expression(env, expr->u.minus_expression);
	break;
      case IF_EXPRESSION:
	v = eval_if_expression(env,
			       expr->u.if_expression.condition,
			       expr->u.if_expression.then_expression,
			       expr->u.if_expression.else_expression);
	break;
      case WHILE_EXPRESSION:
	v = eval_while_expression(env,
				  expr->u.while_expression.condition,
				  expr->u.while_expression.expression_list);
	break;
      case FUNCTION_CALL_EXPRESSION:
	v = eval_function_call_expression
	    (env,
	     expr->u.function_call_expression.identifier,
	     expr->u.function_call_expression.argument);
	break;
      case EXPRESSION_TYPE_NUM:	/* FALLTHRU */
      default:
	DBG_assert(0, ("bad case. type..%d\n", expr->type));
    }
    return v;
}

void
clc_eval_expression(Expression *expression)
{
    Value		v;
    RuntimeError	error_id;

    if ((error_id = (RuntimeError)setjmp(clc_current_interpreter
					 ->error_recovery_environment)) == 0) {
	v = eval_expression(NULL, expression);
	if (clc_current_interpreter->input_mode == CLC_TTY_INPUT_MODE) {
	    if (v.type == INT_VALUE) {
		printf(">>%d\n", v.u.int_value);
	    } else if (v.type == DOUBLE_VALUE) {
		printf(">>%f\n", v.u.double_value);
	    } else {
		printf("<void>\n");
	    }
	}
    } else {
    }
    clc_reopen_current_storage();
}
