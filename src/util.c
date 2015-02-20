#include <stdlib.h>
#include "util.h"

/** This can parse numbers in hexadecimal, octal and decimal.
 * like the following respective examples: !0x4, !04, !4.
 * It also catches negative numbers, or no numbers.
 * The output parameter err contains an error message.*/
long parse_number(const char *string, char **err) {
  char *endptr;
  long n = strtol(string, &endptr, 0);
  if (endptr == string) {
    *err = "That's not a number, is it?";
    return -1;
  }
  if (*endptr != '\0') *err = endptr;
  else if (n < 0)      *err = "One step ahead :).";
  else                 *err = "Success.";
  return n;
}
