/* This is a Hexadecimal implementation.  We need to be able to do hex math
 * so this ADT provides that capability.
 *
 * The hex ADT doesn't support floating point math.  All hex numbers are
 * treated as 64-bit integers.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#include "operand_base_16.h"

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 *****************************************************************************/

/* This is the hex class. */
struct operand_base_16 {

  /* This is the hex number. */
  uint64_t val;
};
  
/******************************************************************************
 ******************************** OPS STRUCT **********************************
 *****************************************************************************/

operand_api operand_base_16_ops = {
  .base_name = "HEX",
  .op_add = (operand_api_binary_op) operand_base_16_op_add,
  .op_sub = (operand_api_binary_op) operand_base_16_op_sub,
  .op_mul = (operand_api_binary_op) operand_base_16_op_mul,
  .op_div = (operand_api_binary_op) operand_base_16_op_div,
  .op_exp = 0,
//  .op_and = (operand_api_binary_op) operand_base_16_op_and,
//  .op_or  = (operand_api_binary_op) operand_base_16_op_or,
//  .op_xor = (operand_api_binary_op) operand_base_16_op_xor,
//  .op_not = (operand_api_binary_op) operand_base_16_op_not
};

/******************************************************************************
 ******************************** PRIVATE API *********************************
 *****************************************************************************/

/******************************************************************************
 ********************************* PUBLIC OPS *********************************
 *****************************************************************************/

/* This is the BCD addition function.
 *
 * Input:
 *   op1  = A pointer to the first operand.  The result is returned in this one.
 *
 *   op2  = The other operand.  Addtion is BINARY.
 *
 * Output:
 *   true  = success.  op1 contains the sum.
 *   false = failure.
 */
bool
operand_base_16_op_add(operand_base_16 *op1,
                       operand_base_16 *op2)
{
  bool retcode = false;

  if((op1 != (operand_base_16 *) 0) && (op2 != (operand_base_16 *) 0))
  {
    op1->val += op2->val;
    retcode = true;
  }
    
  return retcode;
}

/* This is the BCD subtraction function.
 *
 * Input:
 *   op1  = A pointer to the first operand.  The result is returned in this one.
 *
 *   op2  = The other operand.  Subtraction is BINARY.
 *
 * Output:
 *   true  = success.  op1 contains the difference.
 *   false = failure.
 */
bool
operand_base_16_op_sub(operand_base_16 *op1,
                       operand_base_16 *op2)
{
  bool retcode = false;

  if((op1 != (operand_base_16 *) 0) && (op2 != (operand_base_16 *) 0))
  {
    op1->val -= op2->val;
    retcode = true;
  }
    
  return retcode;
}

/* This is the BCD multiplication function.
 *
 * Input:
 *   op1  = A pointer to the first operand.  The result is returned in this one.
 *
 *   op2  = The other operand.  Multiplication is BINARY.
 *
 * Output:
 *   true  = success.  op1 contains the product.
 *   false = failure.
 */
bool
operand_base_16_op_mul(operand_base_16 *op1,
                       operand_base_16 *op2)
{
  bool retcode = false;

  if((op1 != (operand_base_16 *) 0) && (op2 != (operand_base_16 *) 0))
  {
    op1->val *= op2->val;
    retcode = true;
  }
    
  return retcode;
}

/* This is the BCD division function.
 *
 * Input:
 *   op1  = A pointer to the first operand.  The result is returned in this one.
 *
 *   op2  = The other operand.  Division is BINARY.
 *
 * Output:
 *   true  = success.  op1 contains the quotient.
 *   false = failure.
 */
bool
operand_base_16_op_div(operand_base_16 *op1,
                       operand_base_16 *op2)
{
  bool retcode = false;

  /* Don't allow divide by zero. */
  if((op1 != (operand_base_16 *) 0) && (op2 != (operand_base_16 *) 0) && (op2->val != 0))
  {
    op1->val /= op2->val;
    retcode = true;
  }
    
  return retcode;
}

/******************************************************************************
 ********************************* PUBLIC API *********************************
 *****************************************************************************/

/* Return a pointer to the exported hex operations.
 *
 * Input:
 *   N/A.
 *
 * Output:
 *   Returns a pointer to the operations.
 *   Returns 0 if unable to return the pointer to the ops.
 */
operand_api *
operand_base_16_return_ops(void)
{
  return &operand_base_16_ops;
}

/* Create a new operand_base_16 object.  This object can be used to access the
 * operand_base_16 class.
 *
 * Input:
 *   N/A.
 *
 * Output:
 *   Returns a pointer to the object.
 *   Returns 0 if unable to create the object.
 */
operand_base_16 *
operand_base_16_new(void)
{
  operand_base_16 *this = malloc(sizeof(*this));

  if(this != (operand_base_16 *) 0)
  {
    /* Start with zero. */
    this->val  = 0;
  }

  return this;
}

/* Delete a operand_base_16 object that was created by operand_base_16_new().
 *
 * Input:
 *   this = A pointer to the operand_base_16 object.
 *
 * Output:
 *   true  = success.  this is deleted.
 *   false = failure.  this is undefined.
 */
bool
operand_base_16_delete(operand_base_16 *this)
{
  bool retcode = false;

  if(this != (operand_base_16 *) 0)
  {
    free(this);
    retcode = true;
  }

  return retcode;
}

/* Check to see if the specified character is a valid operand_base_16 operand
 * character that can be passed to operand_base_16_add_char().
 *
 * Input:
 *   c = The character to check.
 *
 * Output:
 *   true  = Yes, c is a valid operand character.
 *   false = No, c is NOT a valid operand character.
 */
bool
operand_base_16_add_char_is_valid_operand(char c)
{
  bool retcode = false;

  if((c >= '0') && (c <= '9'))
  {
    retcode = true;
  }
  else
  {
    c &= 0xDF;
    if((c == 'S') || ((c >= 'A') && (c <= 'F')))
    {
      retcode = true;
    }
  }

  return retcode;
}

/* Attempt to add a character to the operand_base_16 object.  The character is
 * checked to see if it's a valid part of a hexadecimal number.  If it's valid,
 * it's added.  Note that it's possible for a character to be dropped because
 * we don't have room for it.
 *
 * We only allow the user to insert 16 digits.  After that, we drop them.
 *
 * Input:
 *   this = A pointer to the operand_base_16 object.
 *
 *   c    = The char to add.  If it's valid, we use it.  If it's not valid,
 *          then we return false.
 *
 * Output:
 *   true  = success.  c is valid, and it has been added to this (if it fits).
 *   false = failure.  c is NOT a number OR we were unable to add c to this.
 */
bool
operand_base_16_add_char(operand_base_16 *this,
                         char             c)
{
  bool retcode = false;

  /* An 'S' toggles the +/- sign. */
  if((c & 0xDF) == 'S')
  {
    this->val = 0 - this->val;
    retcode = true;
  }

  /* Check to see if it's a hex a digit. */
  else
  {
    if((c >= '0') && (c <= '9'))
    {
      c -= 0x30;
      retcode = true;
    }
    else 
    {
      c &= 0xDF;
      c -= 0x37;
      if((c >= 0x0A) && (c <= 0x0F))
      {
        retcode = true;
      }
    }

    if(retcode == true)
    {
      /* If the value is already full, then silently drop the character. */
      if((this->val & 0x7000000000000000ll) == 0)
      {
        this->val <<= 4;
        this->val |= c;
      }
    }
  }

  return retcode;
}

/* Create an ASCII string the represents the current value of the number.
 *
 * Input:
 *   this     = A pointer to the operand_base_16 object.
 *
 *   buf      = The caller-supplied buffer to build the string in.
 *
 *   buf_size = The size of buf.  Note that we must allow 1 byte for the NULL
 *              terminator.
 *
 * Output:
 *   true  = success.  buf contains the string.  Note that it might be
 *                     truncated if buf is too small to hold the entire size of
 *                     the string.
 *   false = failure.  buf is undefined.
 */
bool
operand_base_16_to_str(operand_base_16 *this,
                       char             *buf,
                       size_t            buf_size)
{
  bool retcode = false;

  if( (this != (operand_base_16 *) 0) && (buf != (char *) 0) && (buf_size > 0) )
  {
    int buf_x = 0;

    do
    {
      int64_t val = this->val;

      DBG_PRINT("%s(): 0x%016llX.\n", __func__, val);

      /* If it's zero, then it's zero. */
      if(val == 0ll)
      {
        if(--buf_size == 0) { break; } else { buf[buf_x++] = '0'; }
        break;
      }

      /* Skip insignificant zeroes. */
      int i;
      for(i = 16; (i > 0) && (((val >> ((i - 1) * 4)) & 0xF) == 0); i--);

      /* Now add the actual digits. */
      for( ; i > 0; i--)
      {
        int shift = (i - 1) * 4;
        char c = (val >> shift) & 0xF;

        /* Convert to ASCII. */
        c += (c <= 9) ? 0x30 : 0x37;

        /* Insert the digit. */
        if(--buf_size == 0) { break; } else { buf[buf_x++] = c; }
      }
    } while(0);

    buf[buf_x] = 0;
    retcode = true;
  }

  return retcode;
}

/* Import a signed integer value into this object.  Note that this member
 * doesn't allow you to import an IEEE floating point value.
 *
 * Input:
 *   this     = A pointer to the operand_base_16 object.
 *
 *   src      = A signed integer value to use to seed the object.  If there is
 *              a value already loaded into this object, it will be erased.
 *
 *              NOTE: The operand_base_16 class doesn't support signed values.
 *                    So if src is negative, we'll set this to zero.
 *
 * Output:
 *   true  = success.  this has been imported.
 *   false = failure.  The contents of this is undefined.
 */
bool
operand_base_16_import(operand_base_16 *this,
                       int64_t          src)
{
  bool retcode = false;

  if(this != (operand_base_16 *) 0)
  {
    /* If src is negative, set it to zero.  We don't support negative hex. */
    if(src < 0)
    {
      src = 0;
    }

    this->val = src;
    retcode = true;
  }

  return retcode;
}

/* Export the value of this object to a signed integer.  Note that this member
 * doesn't allow you to export to an IEEE floating point value.
 *
 * Input:
 *   this     = A pointer to the operand_base_16 object.
 *
 *   dst      = A pointer to a signed integer value that will receive the value
 *              of this.
 *
 *              NOTE: If this is a larger value than will fit into *dst, then
 *              we'll set *dst to zero.
 *
 * Output:
 *   true  = success.  this has been exported.
 *   false = failure.  The contents of dst is undefined.
 */
bool
operand_base_16_export(operand_base_16 *this,
                       int64_t         *dst)
{
  bool retcode = false;

  if((this != (operand_base_16 *) 0) && (dst != (int64_t *) 0))
  {
    *dst = (this->val & 0x80000000) ? 0 : this->val;
    
    retcode = true;
  }

  return retcode;
}

/******************************************************************************
 ********************************** TEST API **********************************
 *****************************************************************************/

#ifdef TEST

bool
operand_base_16_test(void)
{
  bool retcode = false;

  /* Loop through some hex numbers.  This tests the basic functionality of
   * the operand_base_16 class.  We're checking to make sure it can handle any
   * type of hex number that we might throw at it. */
  typedef struct operand_base_16_test {
    const char  *src;
    const char  *dst;
  } operand_base_16_test;
  operand_base_16_test tests[] = {
    {                 "1"  ,                "1" }, // A single-digit number.
    {               "123"  ,              "123" }, // Simple integer value.
    {            "123000"  ,           "123000" }, // Integer with trailing zeroes.
    {         "000123000"  ,           "123000" }, // Insignificant leading zeroes.
    {  "FEDCBA9876543210"  , "FEDCBA9876543210" }, // Full size (including MSB).
    {  "FEDCBA9876543210S" ,  "123456789ABCDF0" }, // Negative number.
    { "123456789abcdef01"  , "123456789ABCDEF0" }, // Too big (truncate).
  };
  size_t operand_base_16_test_size = (sizeof(tests) / sizeof(operand_base_16_test));

  int x;
  for(x = 0; x < operand_base_16_test_size; x++)
  {
    operand_base_16_test *t = &tests[x];
    printf("  %s\n", t->src);

    operand_base_16 *this = operand_base_16_new();
    if((retcode = (this != (operand_base_16 *) 0)) != true)                return false;

    const char *src = t->src;
    while(*src)
    {
      if((retcode = operand_base_16_add_char(this, *src++)) != true)       return false;
    }

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    if((retcode = operand_base_16_to_str(this, buf, sizeof(buf))) != true) return false;

    DBG_PRINT("strcmp(%s, %s)\n", t->dst, buf);
    if((retcode = (strcmp(t->dst, buf) == 0)) != true)                     return false;
  }
  
  /* Math operations. */
  typedef struct operand_base_16_math_test {
    const char  *name;
    const char  *val1;
    const char  *val2;
    bool (*func)(operand_base_16 *val1, operand_base_16 *val2);
    const char  *result;
  } operand_base_16_math_test;
  operand_base_16_math_test math_tests[] = {
    { "HEX_ADD_01",                "1",     "2", operand_base_16_op_add,                "3" },
    { "HEX_ADD_02", "FFFFFFFFFFFFFFFF",     "1", operand_base_16_op_add,                "0" },
    { "HEX_SUB_01",                "0",     "1", operand_base_16_op_sub, "FFFFFFFFFFFFFFFF" },
    { "HEX_SUB_02",             "1000",   "123", operand_base_16_op_sub,              "EDD" },
    { "HEX_MUL_01",                "1",     "0", operand_base_16_op_mul,                "0" },
    { "HEX_MUL_02",             "1234",  "5678", operand_base_16_op_mul,          "6260060" },
    { "HEX_MUL_03", "1000000000000000",    "10", operand_base_16_op_mul,                "0" },
    { "HEX_DIV_01",        "136bdbca4",  "1234", operand_base_16_op_div,           "11121D" },
    { "HEX_DIV_02",            "fffff", "fffff", operand_base_16_op_div,                "1" },
  };
  size_t operand_base_16_math_test_size = (sizeof(math_tests) / sizeof(operand_base_16_math_test));

  for(x = 0; x < operand_base_16_math_test_size; x++)
  {
    operand_base_16_math_test *t = &math_tests[x];
    const char *val1 = t->val1;
    const char *val2 = t->val2;
    printf("  %s\n", t->name);

    operand_base_16 *obj1 = operand_base_16_new();
    if((retcode = (obj1 != (operand_base_16 *) 0)) != true)                return false;
    while(*val1)
    {
      if((retcode = operand_base_16_add_char(obj1, *val1++)) != true)      return false;
    }

    operand_base_16 *obj2 = operand_base_16_new();
    if((retcode = (obj2 != (operand_base_16 *) 0)) != true)                return false;
    while(*val2)
    {
      if((retcode = operand_base_16_add_char(obj2, *val2++)) != true)      return false;
    }

    if((retcode = t->func(obj1, obj2)) != true)                            return false;
    
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    if((retcode = operand_base_16_to_str(obj1, buf, sizeof(buf))) != true) return false;

    DBG_PRINT("strcmp(%s, %s)\n", t->result, buf);
    if((retcode = (strcmp(t->result, buf) == 0)) != true)                  return false;

    if((retcode = operand_base_16_delete(obj1)) != true)                   return false;
    if((retcode = operand_base_16_delete(obj2)) != true)                   return false;
  }

  /* Divide by zero test.  We're not fiddling around with creating a bunch of
   * nifty little objects.  We're building them and firing off the test. */
  printf("Divide by zero test.\n");
  operand_base_16 *o1 = operand_base_16_new(); o1->val = 0x1;
  operand_base_16 *o2 = operand_base_16_new(); o1->val = 0x0;
  if(operand_base_16_op_div(o1, o2) != false)                              return false;
  operand_base_16_delete(o1); operand_base_16_delete(o2);

  return retcode;
}

#endif // TEST

