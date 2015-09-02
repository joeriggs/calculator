/* This is a Hexadecimal implementation.  We need to be able to do hex math
 * so this ADT provides that capability.
 *
 * The hex ADT doesn't support floating point math.  All hex numbers are
 * treated as 64-bit integers.
 */

#ifndef __OPERAND_BASE_16_H__
#define __OPERAND_BASE_16_H__

#include "operand_api.h"

/****************************** CLASS DEFINITION ******************************/

typedef struct operand_base_16 operand_base_16;

/********************************* PUBLIC OPS *********************************/

bool operand_base_16_op_add(operand_base_16 *op1, operand_base_16 *op2);
bool operand_base_16_op_sub(operand_base_16 *op1, operand_base_16 *op2);
bool operand_base_16_op_mul(operand_base_16 *op1, operand_base_16 *op2);
bool operand_base_16_op_div(operand_base_16 *op1, operand_base_16 *op2);

/********************************* PUBLIC API *********************************/

operand_api * operand_base_16_return_ops(void);

operand_base_16 *operand_base_16_new(void);

bool operand_base_16_delete(operand_base_16 *this);

bool operand_base_16_add_char_is_valid_operand(char c);

bool operand_base_16_add_char(operand_base_16 *this, char c);

bool operand_base_16_to_str(operand_base_16  *this, char  *buf, size_t buf_size);

bool operand_base_16_import(operand_base_16 *this, int64_t src);

bool operand_base_16_export(operand_base_16 *this, int64_t *dst);

/********************************** TEST API **********************************/

#if defined(TEST)

bool operand_base_16_test(void);

#endif // TEST

#endif // __OPERAND_BASE_16_H__

