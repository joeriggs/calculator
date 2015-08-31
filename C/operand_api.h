/* This file defines the set of operations that a data type can support.  For
 * example, base_10 doesn't support bitwise operations, but base_16 does.  So
 * the base_10 class can leave the pointers to the bitwise operations set to 0
 * so operand.c won't allow bitwise operations when running in base_10 mode.
 */
#ifndef __OPERAND_API_H__
#define __OPERAND_API_H__

/*********************** JUMP TABLE FOR BASE DATA TYPES ***********************/
/* This table defines the set of operations that each data type can support.  */

typedef bool (*operand_api_binary_op)(void *op1, void *op2);
typedef bool (*operand_api_unary_op)(void *op);

typedef struct operand_api {
  const char *base_name;
  operand_api_binary_op op_add;
  operand_api_binary_op op_sub;
  operand_api_binary_op op_mul;
  operand_api_binary_op op_div;
  operand_api_binary_op op_exp;
  operand_api_binary_op op_and;
  operand_api_binary_op op_or;
  operand_api_binary_op op_xor;
  operand_api_unary_op  op_not;
  bool (*op_add_char_is_valid_operand)(char c);
  bool (*op_add_char)(void *this, char c);
  bool (*op_to_str)(void *this, char *buf, size_t buf_size);
} operand_api;

#endif // __OPERAND_API_H__

