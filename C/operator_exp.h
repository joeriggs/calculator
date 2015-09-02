/* An implementation of floating-point exponentiation. */

#ifndef __OPERATOR_EXP_H__
#define __OPERATOR_EXP_H__

#include "operand_base_10.h"

/****************************** CLASS DEFINITION ******************************/

typedef struct operator_exp operator_exp;

/********************************* PUBLIC API *********************************/

operator_exp *operator_exp_new(operand_base_10 *base, operand_base_10 *exp);

bool operator_exp_delete(operator_exp *this);

bool operator_exp_calc(operator_exp *this);

bool operator_exp_get_result(operator_exp *this, operand_base_10 *result);

/********************************** TEST API **********************************/

#if defined(TEST)

bool operator_exp_test(void);

#endif // TEST

#endif /* __OPERATOR_EXP_H__ */

