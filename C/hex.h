/* This is a Hexadecimal implementation.  We need to be able to do hex math
 * so this ADT provides that capability.
 *
 * The hex ADT doesn't support floating point math.  All hex numbers are
 * treated as 64-bit integers.
 */

#ifndef __HEX_H__
#define __HEX_H__

#include "operand_api.h"

/****************************** CLASS DEFINITION ******************************/

typedef struct hex hex;

/********************************* PUBLIC OPS *********************************/

bool hex_op_add(hex *op1, hex *op2);
bool hex_op_sub(hex *op1, hex *op2);
bool hex_op_mul(hex *op1, hex *op2);
bool hex_op_div(hex *op1, hex *op2);

/********************************* PUBLIC API *********************************/

operand_api * hex_return_ops(void);

hex *hex_new(void);

bool hex_delete(hex *this);

bool hex_add_char_is_valid_operand(char c);

bool hex_add_char(hex *this, char c);

bool hex_to_str(hex  *this, char  *buf, size_t buf_size);

bool hex_import(hex *this, int64_t src);

bool hex_export(hex *this, int64_t *dst);

/********************************** TEST API **********************************/

#if defined(TEST)

bool hex_test(void);

#endif // TEST

#endif // __HEX_H__

