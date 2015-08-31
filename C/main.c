/* This is the main entrypoint for the calculator UI and test program.
 */

#include <stdlib.h>

#include "common.h"
#include "operand.h"
#include "test.h"
#include "ui.h"

int main(int argc, char **argv)
{
  int retcode = 1;

  /* One-time initialization of the operand class.  It needs to do some init
   * in order to be able to access all of the classes that manager our data
   * base types (i.e. base_10 (bcd.c) and base_16 (hex.c)). */
  if(operand_initialize() == true)
  {
#ifdef TEST
    retcode = test();
#else
    retcode = ui();
#endif
  }

  return retcode;
}

