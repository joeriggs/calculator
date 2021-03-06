/* A class that can be used to store and manipulate operands.  It is used
 * exclusively by the calculator class.
 */
#ifndef __OPERAND_H__
#define __OPERAND_H__

/****************************** CLASS DEFINITION ******************************/

/* This is a list of the supported number bases.  NOTE that these values are
 * used as indexes into the operand_ops table in operand.c. */
typedef enum {
  operand_type_base_10 = 1,
  operand_type_base_16,
  operand_type_base_max
} operand_type;

typedef struct operand operand;

/********************************* PUBLIC OPS *********************************/

typedef bool (*operand_binary_op)(operand *op1, operand *op2);
typedef bool (*operand_unary_op)(operand *op);

bool operand_op_add(operand *op1, operand *op2);
bool operand_op_sub(operand *op1, operand *op2);
bool operand_op_mul(operand *op1, operand *op2);
bool operand_op_div(operand *op1, operand *op2);
bool operand_op_exp(operand *op1, operand *op2);
bool operand_op_and(operand *op1, operand *op2);
bool operand_op_or (operand *op1, operand *op2);
bool operand_op_xor(operand *op1, operand *op2);
bool operand_op_not(operand *op);

/********************************* PUBLIC API *********************************/

bool operand_initialize(void);

operand *operand_new(operand_type base);

bool operand_delete(operand *this);

bool operand_get_base(operand *this, operand_type *base);

bool operand_set_base(operand *this, operand_type base);

bool operand_add_char_is_valid_operand(operand_type base, char c);

bool operand_add_char_allowed(operand *this);

bool operand_add_char(operand *this, char c);

bool operand_to_str(operand *this, char *buf, size_t buf_size);

/********************************** TEST API **********************************/

#if defined(TEST)

bool operand_test(void);

#endif // TEST

#endif // __OPERAND_H__

