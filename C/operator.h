/* A class that can be used to evaluate C-style operators.
 */
#ifndef __OPERATOR_H__
#define __OPERATOR_H__

/****************************** CLASS DEFINITION ******************************/

/* This is a list of operator types.  It deals specifically with how many
 * operands are required.
 *
 * - None means it's not a regular operand.  For example, a parentheses.  You
 *   need to check the operator_special_type value to gather more info.
 *
 * - Unary operators require 1 operand.
 *
 * - Binary operators require 2 operands.
 */
typedef enum {
  op_type_none,
  op_type_unary,
  op_type_binary
} operator_type;

/* This is a list of information about special operators.  The purpose of this
 * information is to help upper-level classes know how to deal with an operator.
 */
typedef enum {
  op_special_type_none,
  op_special_type_l_paren,
  op_special_type_r_paren
} operator_special_type;

typedef struct operator operator;

/********************************* PUBLIC API *********************************/

operator *operator_new(const char c);

bool operator_delete(operator *this);

bool operator_is_valid_operator(char c);

bool operator_precedence(operator *this, int *input, int *stack);

bool operator_get_name(operator *this, const char **op_name);

bool operator_get_op_type(operator *this, operator_type *type);

bool operator_get_op_specialtype(operator *this, operator_special_type *special_type);

bool operator_do_unary(operator *this, operand *op);

bool operator_do_binary(operator *this, operand *op1, operand *op2);

/********************************** TEST API **********************************/

#if defined(TEST)

bool operator_test(void);

#endif // TEST

#endif // __OPERATOR_H__

