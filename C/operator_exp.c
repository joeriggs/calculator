/* This file contains an ADT that performs floating point exponentiation.
 * Floating poing exponentiation is a lot more complicated than plain old
 * decimal exponentiation, so it has been isolated in this ADT.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

#include "operand_base_10.h"
#include "operator_exp.h"

/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 *****************************************************************************/

/* This is the operator_exp class. */
struct operator_exp {
  operand_base_10 *base;
  operand_base_10 *exp;
  operand_base_10 *result;

  /* We often need to convert exp to a fraction.  It is stored here. */
  uint64_t exp_numerator;
  uint64_t exp_denominator;
};

/******************************************************************************
 ******************************** PRIMITIVES **********************************
 *****************************************************************************/

/* Perform an exponentiation with a floating point base and an integer exponent.
 *
 * Input:
 *   base   = The floating point base.
 *
 *   exp    = The integer exponent.  Currently only supports positive exponents.
 *
 *   result = A pointer to the operand_base_10 object that will receive the
 *            result.  Note that it is okay if base == result.  We will make
 *            sure we don't stomp on base while we compute result.
 *
 * Output:
 *   true  = success.  *result contains the result.
 *   false = failure.  *result is undefined.
 */
static bool
operator_exp_integer_exp(operand_base_10 *base,
                         int              exp,
                         operand_base_10 *result)
{
  bool retcode = false;

  if((base != (operand_base_10 *) 0) && (result != (operand_base_10 *) 0))
  {
    operand_base_10 *rslt_tmp = (operand_base_10 *) 0;
    operand_base_10 *base_tmp = (operand_base_10 *) 0;
    operand_base_10 *zero     = (operand_base_10 *) 0;
    operand_base_10 *one      = (operand_base_10 *) 0;

    do
    {
      if((rslt_tmp = (base == result) ? operand_base_10_new() : result) == (operand_base_10 *) 0) { break; }
      if((base_tmp = operand_base_10_new()) == (operand_base_10 *) 0)                             { break; }
      if((zero     = operand_base_10_new()) == (operand_base_10 *) 0)                             { break; }
      if((one      = operand_base_10_new()) == (operand_base_10 *) 0)                             { break; }

      if(operand_base_10_copy(base, base_tmp) == false)                                           { break; }
      if(operand_base_10_import(zero, 0) == false)                                                { break; }
      if(operand_base_10_import(one, 1) == false)                                                 { break; }

      /* Special case.  base ^ 0 = 1. */
      if(exp == 0)
      {
        retcode = operand_base_10_copy(one, rslt_tmp);
        break;
      }

      /* Special case.  0 ^ exp = 0. */
      if(operand_base_10_cmp(base, zero) == 0)
      {
        retcode = operand_base_10_copy(zero, rslt_tmp);
        break;
      }

      /* Set res = 1.  (base ^ 0) = 1, so this is the right place to start. */
      if(operand_base_10_import(rslt_tmp, 1) == false)                                            { break; }

      while(exp != 0)
      {
        if((exp & 1) != 0)
        {
          if(operand_base_10_op_mul(rslt_tmp, base_tmp) == false)                                 { break; }
        }

        /* Prepare for the next iteration. */
        if(operand_base_10_op_mul(base_tmp, base_tmp) == false)                                   { break; }
        exp >>= 1;
      }

      retcode = (exp == 0) ? true : false;
      
    } while(0);

    operand_base_10_delete(one);
    operand_base_10_delete(zero);
    operand_base_10_delete(base_tmp);

    if((retcode == true) && (base == result))
    {
      retcode = operand_base_10_copy(rslt_tmp, result);
      operand_base_10_delete(rslt_tmp);
    }
  }

  return retcode;
}

/******************************************************************************
 ******************************** PRIVATE API *********************************
 *****************************************************************************/

/* Convert the floating point exponent into a fraction, and then reduce the
 * fraction.  For example, if this->exp = 3.45:
 * 1. Convert it to = 345/100.
 * 2. Reduce it down to 69/20.
 *
 * To reduce, find the greatest common Divisor (GCD) (using the Euclid method),
 * and use it to reduce the exponent fraction.
 *
 * Input:
 *   this = A pointer to the operator_exp object.
 *
 * Output:
 *   true  = success.  The exponent has been converted to a fraction.  The
 *                     fraction is stored in this->exp_numerator and
 *                     this->exp_denominator.
 *   false = failure.  Something went wrong.
 */
static bool
operator_exp_to_fraction(operator_exp *this)
{
  bool retcode = false;

  /* We'll use these inside a loop, then we'll delete them when we're done. */
  operand_base_10 *ten    = (operand_base_10 *) 0;
  operand_base_10 *root   = (operand_base_10 *) 0;
  operand_base_10 *tmp_f1 = (operand_base_10 *) 0;
  operand_base_10 *tmp_f2 = (operand_base_10 *) 0;

  do
  {
    if(this == (operator_exp *) 0)                                       { break; }

    if((ten    = operand_base_10_new()) == (operand_base_10 *) 0)        { break; }
    if((root   = operand_base_10_new()) == (operand_base_10 *) 0)        { break; }
    if((tmp_f1 = operand_base_10_new()) == (operand_base_10 *) 0)        { break; }
    if((tmp_f2 = operand_base_10_new()) == (operand_base_10 *) 0)        { break; }

    if(operand_base_10_import(ten, 10) == false)                         { break; }

    /* Convert the exponent to a fraction (numerator and denominator), and then
     * reduce the fraction.
     */
    int loop;
    for(loop = 1; loop < 20; loop++)
    {
      if((retcode = operator_exp_integer_exp(ten, loop, root)) == false) { break; }

      if(operand_base_10_copy(this->exp, tmp_f1) == false)               { break; }
      if((retcode = operand_base_10_op_mul(tmp_f1, root)) == false)      { break; }

      int64_t tmp_i;
      if(operand_base_10_export(tmp_f1, &tmp_i) == false)                { break; }
      if(operand_base_10_import(tmp_f2, tmp_i) == false)                 { break; }

      if(operand_base_10_cmp(tmp_f1, tmp_f2) == 0)
      {
        this->exp_numerator = tmp_i;
        if(operand_base_10_export(root, &tmp_i) == false)                { break; }
        this->exp_denominator = tmp_i;
        retcode = true;
        break;
      }
    }

    if(retcode == true)
    {
      DBG_PRINT("%s(BEF): this->exp_numerator %lld: this->exp_denominator %lld\n",
                  __func__, this->exp_numerator, this->exp_denominator);

      /* Initialize a and b to equal the numerator and denominator.  Make sure
       * that a >= b. */
      uint64_t a, b;
      if(this->exp_numerator >= this->exp_denominator)
      {
        a = this->exp_numerator;
        b = this->exp_denominator;
      }
      else
      {
        a = this->exp_denominator;
        b = this->exp_numerator;
      }

      /* Loop until we calculate the GCD. */
      uint64_t gcd;
      while(1)
      {
        if((gcd = a % b) == 0)
        {
          gcd = b;
          break;
        }

        a = b;
        b = gcd;
      }

      /* Now reduce the fraction by the GCD. */
      this->exp_numerator /= gcd;
      this->exp_denominator /= gcd;

      DBG_PRINT("%s(AFT): this->exp_numerator %lld: this->exp_denominator %lld\n",
                  __func__, this->exp_numerator, this->exp_denominator);
    }
  } while(0);

  operand_base_10_delete(tmp_f2);
  operand_base_10_delete(tmp_f1);
  operand_base_10_delete(root);
  operand_base_10_delete(ten);

  return retcode;
}

/* This is a very fast-converging nth root algorithm to find the nth root of A.
 *
 * Taken from http://en.wikipedia.org/wiki/Nth_root_algorithm
 *
 * We're calculating the "n"th root of "A".
 *
 * 1. Make an initial guess X_k.
 *
 * 2. X_k+1 = (1 / n) * ( (n - 1) * X_k + (A / X_k^(n-1))).
 *
 *    In practice we do:
 *      Delta X_k = (1 / n) * ((A / X_k^(n-1)) - X_k); X_k+1 = X_k + Delta X_k.
 *
 * 3. Repeat step 2 until the desired precision is reached, i.e.
 *      |Delta X_k| < Epsilon.
 *
 * Input:
 *   this  = A pointer to the operator_exp object.  In this member, the
 *           variables are as follows:
 *           A   = this->base
 *           n   = this->exp_denominator
 *
 *   guess = A pointer to a operand_base_10 object that will receive the guess.
 *           X_k = guess
 *
 * Output:
 *   true  = success.  *guess contains the guess.
 *   false = failure.  The contents of *guess is undefined.
 */
static bool
operator_exp_nth_root_guess(operator_exp    *this,
                            operand_base_10 *guess)
{
  bool retcode = false;

  operand_base_10 *A              = (operand_base_10 *) 0;
  operand_base_10 *n_f            = (operand_base_10 *) 0;
  operand_base_10 *X_k            = (operand_base_10 *) 0;
  operand_base_10 *part1          = (operand_base_10 *) 0;
  operand_base_10 *part2          = (operand_base_10 *) 0;
  operand_base_10 *part3          = (operand_base_10 *) 0;
  operand_base_10 *part4          = (operand_base_10 *) 0;
  operand_base_10 *delta_X_k      = (operand_base_10 *) 0;
  operand_base_10 *delta_X_k_prev = (operand_base_10 *) 0;
  operand_base_10 *zero           = (operand_base_10 *) 0;
  operand_base_10 *guess_tmp      = (operand_base_10 *) 0;
  operand_base_10 *best_diff      = (operand_base_10 *) 0;
  operand_base_10 *test_rslt      = (operand_base_10 *) 0;
  operand_base_10 *test_diff      = (operand_base_10 *) 0;

  do
  {
    if(this == (operator_exp *) 0)                                        { break; }
    if(guess == (operand_base_10 *) 0)                                    { break; }

    if((A = operand_base_10_new()) == (operand_base_10 *) 0)              { break; }
    if(operand_base_10_copy(this->base, A) == false)                      { break; }

    uint64_t n_int = this->exp_denominator;
    if((n_f = operand_base_10_new()) == (operand_base_10 *) 0)            { break; }
    if(operand_base_10_import(n_f, this->exp_denominator) == false)       { break; }

    if((X_k = operand_base_10_new()) == (operand_base_10 *) 0)            { break; }
    if(operand_base_10_import(X_k, 1) == false)                           { break; }

    if((part1          = operand_base_10_new()) == (operand_base_10 *) 0) { break; }
    if((part2          = operand_base_10_new()) == (operand_base_10 *) 0) { break; }
    if((part3          = operand_base_10_new()) == (operand_base_10 *) 0) { break; }
    if((part4          = operand_base_10_new()) == (operand_base_10 *) 0) { break; }
    if((delta_X_k      = operand_base_10_new()) == (operand_base_10 *) 0) { break; }
    if((delta_X_k_prev = operand_base_10_new()) == (operand_base_10 *) 0) { break; }

    if((zero           = operand_base_10_new()) == (operand_base_10 *) 0) { break; }
    if(operand_base_10_import(zero, 0) == false)                          { break; }

    if(operand_base_10_import(delta_X_k_prev, 0) == false)                { break; }

    if((best_diff = operand_base_10_new()) == (operand_base_10 *) 0)      { break; }
    if((test_rslt = operand_base_10_new()) == (operand_base_10 *) 0)      { break; }
    if((test_diff = operand_base_10_new()) == (operand_base_10 *) 0)      { break; }

    /* Solve the nth root (see description above).
     *
     *                       --------PART__4--------
     *                       -----PART__3----
     *             PART__1         -PART__2-
     * Delta X_k = (1 / n) * ((A / X_k^(n-1)) - X_k); X_k+1 = X_k + Delta X_k.
     */
    if(operand_base_10_import(part1, 1) == false)                         { break; }
    if(operand_base_10_op_div(part1, n_f) == false)                       { break; }
    DBG_PRINT("%s(): START: A %s: n %s: X_k %s: part1 %s\n", __func__,
              operand_base_10_get_dbg_info(A),
              operand_base_10_get_dbg_info(n_f),
              operand_base_10_get_dbg_info(X_k),
              operand_base_10_get_dbg_info(part1));

    int x;
    for(x = 0; x < 100000; x++)
    {
      DBG_PRINT("%s(): %4d\n", __func__, x);

      if(operator_exp_integer_exp(X_k, (n_int - 1), part2) == false)      { break; }
      DBG_PRINT("%s(): PART2: %s ^ %lld = %s\n", __func__,
                operand_base_10_get_dbg_info(X_k),
                (n_int - 1),
                operand_base_10_get_dbg_info(part2));

      if(operand_base_10_copy(A, part3) == false)                         { break; }
      if(operand_base_10_op_div(part3, part2) == false)                   { break; }
      DBG_PRINT("%s(): PART3: %s / %s = %s\n", __func__,
                operand_base_10_get_dbg_info(A),
                operand_base_10_get_dbg_info(part2),
                operand_base_10_get_dbg_info(part3));

      if(operand_base_10_copy(part3, part4) == false)                     { break; }
      if(operand_base_10_op_sub(part4, X_k) == false)                     { break; }
      DBG_PRINT("%s(): PART4: %s - %s = %s\n", __func__,
                operand_base_10_get_dbg_info(part3),
                operand_base_10_get_dbg_info(X_k),
                operand_base_10_get_dbg_info(part4));

      if(operand_base_10_copy(part1, delta_X_k) == false)                 { break; }
      if(operand_base_10_op_mul(delta_X_k, part4) == false)               { break; }
      DBG_PRINT("%s(): Delta X_k: %s * %s = %s\n", __func__,
                operand_base_10_get_dbg_info(part1),
                operand_base_10_get_dbg_info(part4),
                operand_base_10_get_dbg_info(delta_X_k));

      DBG_PRINT("%s(): Compare %s vs %s\n", __func__,
                operand_base_10_get_dbg_info(delta_X_k),
                operand_base_10_get_dbg_info(delta_X_k_prev));
      if(operand_base_10_cmp(delta_X_k, delta_X_k_prev) == 0)
      {
        break;
      }

      if(operand_base_10_copy(delta_X_k, delta_X_k_prev) == false)        { break; }
      if(operand_base_10_op_add(X_k, delta_X_k) == false)                 { break; }
      if(operand_base_10_copy(X_k, guess) == false)                       { break; }
      DBG_PRINT("%s(): guess = %s\n", __func__, operand_base_10_get_dbg_info(guess));

      /* Check to see if we found the answer. */
      {
        /* Take our current result and use it as the base to recalculate and see
         * we found the result.  Another way to look at it is:
         * We're trying to find: guess = n'th root of A.
         * We'll test by finding: test_rslt = guess ^ n.
         * Then we compare test_rslt against A and see how close we are. */
        if(operator_exp_integer_exp(guess, n_int, test_rslt) == false)    { break; }

        int test_result = operand_base_10_cmp(A, test_rslt);
        if(test_result == 0)
        {
          /* We found the exactly perfect answer.  Drop out and return. */
          retcode = true;
          break;
        }
        else if(test_result == -1)
        {
          /* guess is too high.  Calculate how far off we are. */
          operand_base_10_copy(test_rslt, test_diff);
          operand_base_10_op_sub(test_diff, A);
        }
        else if(test_result == 1)
        {
          /* guess is too low.  Calculate how far off we are. */
          operand_base_10_copy(A, test_diff);
          operand_base_10_op_sub(test_diff, test_rslt);
        }

        /* If this is the first test, just save the test_diff and go again. */
        if(operand_base_10_cmp(best_diff, zero) == 0)
        {
          operand_base_10_copy(test_diff, best_diff);
        }

        /* Otherwise, check to see if this is the best answer we've calculated
         * so far.  If it is, save it. */
        else
        {
          test_result = operand_base_10_cmp(test_diff, best_diff);
          if(test_result == 0)
          {
            /* We've seen this answer before.  This is the best we're going to
             * do.  Return that answer. */
            retcode = true;
            break;
          }
          else if(test_result == -1)
          {
            /* This is our best answer so far.  Save it. */
            operand_base_10_copy(test_diff, best_diff);
          }
        }
      }
    }

    /* The guess is always positive. */
    if(operand_base_10_cmp(guess, zero) < 0)
    {
      if((guess_tmp = operand_base_10_new()) == (operand_base_10 *) 0)    { break; }
      if(operand_base_10_copy(zero, guess_tmp) == false)                  { break; }
      if(operand_base_10_op_sub(guess_tmp, guess) == false)               { break; }
      if(operand_base_10_copy(guess_tmp, guess) == false)                 { break; }
    }

    DBG_PRINT("%s(): guess %s\n", __func__, operand_base_10_get_dbg_info(guess));

    retcode = true;
  } while(0);

  operand_base_10_delete(test_diff);
  operand_base_10_delete(test_rslt);
  operand_base_10_delete(best_diff);
  operand_base_10_delete(guess_tmp);
  operand_base_10_delete(zero);
  operand_base_10_delete(delta_X_k);
  operand_base_10_delete(part4);
  operand_base_10_delete(part3);
  operand_base_10_delete(part2);
  operand_base_10_delete(part1);
  operand_base_10_delete(X_k);
  operand_base_10_delete(n_f);
  operand_base_10_delete(A);

  return retcode;
}

/******************************************************************************
 ********************************* PUBLIC API *********************************
 *****************************************************************************/

/* Create a new operator_exp object.  This object can be used to access the
 * operator_exp class.
 *
 * Input:
 *   base = The floating point base.
 *
 *   exp  = The floating point exponent.
 *
 * Output:
 *   Returns a pointer to the object.
 *   Returns 0 if unable to create the object.
 */
operator_exp *
operator_exp_new(operand_base_10 *base,
                 operand_base_10 *exp)
{
  operator_exp *this = (operator_exp *) 0;

  bool success = false;
  do
  {
    /* Initialize. */
    if((this = (operator_exp *) malloc(sizeof(*this))) == (operator_exp *) 0)         { break; }

    this->base   = operand_base_10_new();
    this->exp    = operand_base_10_new();
    this->result = operand_base_10_new();

    /* Make copies of the base and exponent. */
    if((this->base == (operand_base_10 *) 0) || (this->exp == (operand_base_10 *) 0)) { break; }
    
    if(operand_base_10_copy(base, this->base) == false)                               { break; }
    if(operand_base_10_copy(exp,  this->exp) == false)                                { break; }

    success = true;
  } while(0);

  if(success == false)
  {
    operator_exp_delete(this);
    this = (operator_exp *) 0;
  }

  return this;
}

/* Delete an operator_exp object that was created by operator_exp_new().
 *
 * Input:
 *   this = A pointer to the operator_exp object.
 *
 * Output:
 *   true  = success.  The object is deleted.
 *   false = failure.
 */
bool
operator_exp_delete(operator_exp *this)
{
  bool retcode = false;

  if(this != (operator_exp *) 0)
  {
    operand_base_10_delete(this->result);
    operand_base_10_delete(this->exp);
    operand_base_10_delete(this->base);

    free(this);

    retcode = true;
  }

  return retcode;
}

/*
 * Taken from http://mathforum.org/library/drmath/view/55896.html
 *
 * What we want to do in this module is calculate an exponentiation where the
 * exponent is not an integer.  For this explanation, we'll use the following
 * example:
 *
 *     x = (5^3.4).
 *
 * Using this example, we need to think of (3.4) as (34/10).  (34/10) can be
 * thought of as (34 * 1/10).  Using this info, we can rewrite the original
 * problem as:
 *
 *     x = ( 5^(1/10) )^34).
 *
 * 1. Breaking this down into 2 steps, we first solve the 5th root of 10.  Easy
 *    enough.  We can do that using Newton's guessing algorithm.
 *
 *     R = 5^(1/10)
 *
 * 2. Then we solve the following:
 *
 *     x = R^34
 *
 * And then we have solved x = (5^3.4).
 *
 * Input:
 *   this   = A pointer to the operator_exp object.
 *
 * Output:
 *   true  = success.  *result contains the result.
 *   false = failure.  *results is undefined.
 */
bool
operator_exp_calc(operator_exp *this)
{
  bool retcode = false;

  operand_base_10 *zero      = (operand_base_10 *) 0;
  operand_base_10 *one       = (operand_base_10 *) 0;
  operand_base_10 *tmp_exp_f = (operand_base_10 *) 0;
  operand_base_10 *guess     = (operand_base_10 *) 0;
  operand_base_10 *exp_tmp   = (operand_base_10 *) 0;

  do
  {
    if(this == (operator_exp *) 0)                                                           { break; }

    if((zero      = operand_base_10_new()) == (operand_base_10 *) 0)                         { break; }
    if((one       = operand_base_10_new()) == (operand_base_10 *) 0)                         { break; }
    if((tmp_exp_f = operand_base_10_new()) == (operand_base_10 *) 0)                         { break; }
    if((guess     = operand_base_10_new()) == (operand_base_10 *) 0)                         { break; }
    if((exp_tmp   = operand_base_10_new()) == (operand_base_10 *) 0)                         { break; }

    /* If the exponent is negative, convert to its absolute value and set a flag
     * to remind us it was negative.  x^-n = 1/(x^n), so we just need to get the
     * inverse when we're done. */
    bool is_neg_exponent = (operand_base_10_cmp(this->exp, zero) < 0);
    if(is_neg_exponent)
    {
      if(operand_base_10_copy(this->exp, exp_tmp) == false)                                  { break; }
      if(operand_base_10_copy(zero, this->exp) == false)                                     { break; }
      if(operand_base_10_op_sub(this->exp, exp_tmp) == false)                                { break; }
    }

    /* Check to see if the exponent is a whole number.  If it is, then we can do
     * easy exponentiation. */
    int64_t tmp_exp_i;
    if(operand_base_10_export(this->exp, &tmp_exp_i) == false)                               { break; }
    if(operand_base_10_import(tmp_exp_f, tmp_exp_i) == false)                                { break; }
    if(operand_base_10_cmp(this->exp, tmp_exp_f) == 0)
    {
      if((retcode = operator_exp_integer_exp(this->base, tmp_exp_i, this->result)) == false) { break; }
    }

    else
    {
      /* At this point we know the exponent is a fraction.  This means the base
       * must be a positive number.  If it's not, the equation is invalid. */
     if(operand_base_10_cmp(this->base, zero) < 0)                                           { break; }

      /* Convert the exponent to a fraction (numerator and denominator). */
      if(operator_exp_to_fraction(this) == false)                                            { break; }

      /* Solve the nth root (see description above). */
      if(operator_exp_nth_root_guess(this, guess) == false)                                  { break; }

      char buf1[64], buf2[64];
      if(operand_base_10_to_str(this->base, buf1, sizeof(buf1)) == false)                    { break; }
      if(operand_base_10_to_str(guess,      buf2, sizeof(buf2)) == false)                    { break; }
      DBG_PRINT("%s(): nth_root: this->base %s: this->exp_denominator %lld: guess %s\n",
                 __func__, buf1, this->exp_denominator, buf2);

      if(operator_exp_integer_exp(guess, this->exp_numerator, this->result) == false)        { break; }
      if(operand_base_10_to_str(guess,        buf1, sizeof(buf1)) == false)                  { break; }
      if(operand_base_10_to_str(this->result, buf2, sizeof(buf2)) == false)                  { break; }
      DBG_PRINT("%s(): exp: guess %s: this->exp_numerator %lld: this->result %s\n",
                  __func__, buf1, this->exp_numerator, buf2);

      retcode = true;
    }

    if((retcode == true) && (is_neg_exponent == true))
    {
      if((retcode = operand_base_10_import(one, 1)) == false)                                { break; }
      if((retcode = operand_base_10_op_div(one, this->result)) == false)                     { break; }
      if((retcode = operand_base_10_copy(one, this->result)) == false)                       { break; }
    }
  } while(0);

  operand_base_10_delete(exp_tmp);
  operand_base_10_delete(guess);
  operand_base_10_delete(tmp_exp_f);
  operand_base_10_delete(one);
  operand_base_10_delete(zero);

  return retcode;
}

/* Get the result from operator_exp_calc().
 *
 * Input:
 *   this   = A pointer to the operator_exp object.
 *
 *   result = A pointer to the location where we will store the result.
 *
 * Output:
 *   true  = success.  *result contains the result.
 *   false = failure.  *results is undefined.
 */
bool
operator_exp_get_result(operator_exp    *this,
                        operand_base_10 *result)
{
  bool retcode = false;

  if((this != (operator_exp *) 0) && (result != (operand_base_10 *) 0))
  {
    retcode = operand_base_10_copy(this->result, result);
  }

  return retcode;
}

/******************************************************************************
 ********************************** TEST API **********************************
 *****************************************************************************/

#if defined(TEST)
bool
operator_exp_test(void)
{
  bool retcode = false;

  /* Here are some problems to test against. */
  typedef struct operator_exp_test {
    char *name;
    char *base;
    char *exp;
    char *result;
  } operator_exp_test;
  operator_exp_test tests[] = {
    { "FP_EXP_01",  "2"    ,   "3"      ,                     "8"                     }, // Int ^ Int = Int.
    { "FP_EXP_02", "18"    ,   "8"      ,        "11,019,960,576"                     }, //   Little tougher.
    { "FP_EXP_03", "97"    ,  "16"      ,                     "6.142536534626857e+31" }, // Test_1.
    { "FP_EXP_04", "97"    ,   "8"      , "7,837,433,594,376,961"                     }, // Test_2.
    { "FP_EXP_05", "97"    ,   "1"      ,                    "97"                     }, // Test_3.
    { "FP_EXP_06", "97"    ,  "25"      ,                     "4.66974705254372e+49"  }, // Test_1 * Test_2 * Test_3.
    { "FP_EXP_07",  "2"    ,   "3s"     ,                     "0.125"                 }, // Int ^ -Int = Fraction.
    { "FP_EXP_08", "25"    ,   "7s"     ,                     "0.00000000016384"      }, //   Little tougher.
    { "FP_EXP_09", "17"    ,  "21s"     ,                     "1.447346952625563e-26" }, //   Little tougher.
    { "FP_EXP_10", "17"    ,    ".23"   ,                     "1.918683107361833"     },
    { "FP_EXP_11",   ".9"  ,    ".7"    ,                     "0.928901697685371"     },
    { "FP_EXP_12",   ".63" ,   "8"      ,                     "0.0248155780267521"    },
    { "FP_EXP_13",  "2"    ,   "2.3456s",                     "0.1967451531162147"    },
    { "FP_EXP_14",  "2.34" ,   "3.45"   ,                    "18.78428669635901"      },
    { "FP_EXP_15",  "2"    ,   "3.5"    ,                    "11.31370849898476"      },
    { "FP_EXP_16",  "2"    ,   "3.6"    ,                    "12.12573253208318"      },
    { "FP_EXP_17",  "2"    ,   "0"      ,                     "1"                     }, // zero exponent.
    { "FP_EXP_18",  "0"    ,   "3"      ,                     "0"                     }, // zero base.
    { "FP_EXP_19",  "0"    ,   "0"      ,                     "1"                     }, // zero base and exponent.
    { "FP_EXP_20",  "2"    , "199"      ,                     "8.034690221294951e+59" }, // big exponent.
    { "FP_EXP_21", "25.43" ,   "1"      ,                    "25.43"                  }, // X ^ 1 = X.
    { "FP_EXP_22",  "3"    ,  "12.345"  ,               "776,357.7442839795"          }, // Stolen from calculator.c.
  };
  size_t tests_size = (sizeof(tests) / sizeof(operator_exp_test));

  operand_base_10 *base   = (operand_base_10 *) 0;
  operand_base_10 *exp    = (operand_base_10 *) 0;
  operand_base_10 *result = (operand_base_10 *) 0;

  do
  {
    int x;
    for(x = 0; x < tests_size; x++)
    {
      operator_exp_test *t = &tests[x];
      printf("%s: %s ^ %s\n", t->name, t->base, t->exp);

      if((base   = operand_base_10_new()) == (operand_base_10 *) 0)   { break; }
      if((exp    = operand_base_10_new()) == (operand_base_10 *) 0)   { break; }
      if((result = operand_base_10_new()) == (operand_base_10 *) 0)   { break; }

      /* Load the base and exponent into operand_base_10 objects.  We're not testing the operand_base_10
       * class here, so don't worry too much about error checking. */
      {
        char *p;
        for(p = t->base; *p != 0; p++) { operand_base_10_add_char(base, *p); }
        for(p = t->exp;  *p != 0; p++) { operand_base_10_add_char(exp,  *p); }
      }

      operator_exp *obj;
      if((obj = operator_exp_new(base, exp)) == (operator_exp *) 0)   { break; }

      if(operator_exp_calc(obj) == false)                             { break; }
      if(operator_exp_get_result(obj, result) == false)               { break; }
      if(operator_exp_delete(obj) == false)                           { break; }

      char buf1[1024];
      if(operand_base_10_to_str(result, buf1, sizeof(buf1)) == false) { break; }
      printf("  result = %s: t->result %s: ", buf1, t->result);

      if(strcmp(buf1, t->result) == 0)
      {
        printf("PASS\n");
      }
      else
      {
        printf("FAIL\n");
        break;
      }

      if(operand_base_10_delete(base)   != true)                      { break; }
      base   = (operand_base_10 *) 0;
      if(operand_base_10_delete(exp)    != true)                      { break; }
      exp    = (operand_base_10 *) 0;
      if(operand_base_10_delete(result) != true)                      { break; }
      result = (operand_base_10 *) 0;
    }

    retcode = true;

  } while(0);

  operand_base_10_delete(result);
  operand_base_10_delete(exp);
  operand_base_10_delete(base);

  return retcode;
}
#endif // TEST
