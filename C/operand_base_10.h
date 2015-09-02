/* This is a BCD (Binary Coded Decimal) implementation.  We need to be able to
 * do decimal math (float and double won't suffice), so this ADT provides that
 * capability.
 */

#ifndef __OPERAND_BASE_10_H__
#define __OPERAND_BASE_10_H__

#include "operand_api.h"

/****************************** CLASS DEFINITION ******************************/

typedef struct operand_base_10 operand_base_10;

/********************************* PUBLIC OPS *********************************/

bool operand_base_10_op_add(operand_base_10 *op1, operand_base_10 *op2);
bool operand_base_10_op_sub(operand_base_10 *op1, operand_base_10 *op2);
bool operand_base_10_op_mul(operand_base_10 *op1, operand_base_10 *op2);
bool operand_base_10_op_div(operand_base_10 *op1, operand_base_10 *op2);
bool operand_base_10_op_exp(operand_base_10 *op1, operand_base_10 *op2);

/********************************* PUBLIC API *********************************/

operand_api * operand_base_10_return_ops(void);

operand_base_10 *operand_base_10_new(void);

bool operand_base_10_delete(operand_base_10 *this);

bool operand_base_10_add_char_is_valid_operand(char c);

bool operand_base_10_add_char(operand_base_10 *this, char c);

bool operand_base_10_to_str(operand_base_10 *this, char *buf, size_t buf_size);

bool operand_base_10_copy(operand_base_10 *src, operand_base_10 *dst);

int operand_base_10_cmp(operand_base_10 *obj1, operand_base_10 *obj2);

bool operand_base_10_import(operand_base_10 *this, int64_t src);

bool operand_base_10_export(operand_base_10 *this, int64_t *dst);

const char * operand_base_10_get_dbg_info(operand_base_10 *this);

/********************************** TEST API **********************************/

#if defined(TEST)

bool operand_base_10_test(void);

#endif // TEST

#endif // __OPERAND_BASE_10_H__

