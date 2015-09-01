/* An implementation of floating-point exponentiation. */

#ifndef __OPERATOR_EXP_H__
#define __OPERATOR_EXP_H__

#include "bcd.h"

/****************************** CLASS DEFINITION ******************************/

typedef struct operator_exp operator_exp;

/********************************* PUBLIC API *********************************/

operator_exp *operator_exp_new(bcd *base, bcd *exp);

bool operator_exp_delete(operator_exp *this);

bool operator_exp_calc(operator_exp *this);

bool operator_exp_get_result(operator_exp *this, bcd *result);

/********************************** TEST API **********************************/

#if defined(TEST)

bool operator_exp_test(void);

#endif // TEST

#endif /* __OPERATOR_EXP_H__ */

