#include <stdlib.h>
#include <string.h>
#include "util.h"

/** Concatenate the two strings, allocating a new string long enough to hold the
 * result string.*/
char *concat(const char *str1, const char *str2) {
  size_t len1 = strlen(str1) + 1;
  size_t len2 = strlen(str2) + 1;
  char *res = (char *)malloc(len1 + len2 - 1);
  strncpy(res, str1, len1);
  strncpy(res + len1 -1, str2, len2);
  return res;
}

/** This can parse numbers in hexadecimal, octal and decimal.
 * like the following respective examples: !0x4, !04, !4.
 * It also catches negative numbers, or no numbers.
 * The output parameter err contains an error message.*/
long parse_number(const char *string, char **err) {
  long n = strtol(string, err, 0);
  if (*err == string) {
    *err = "That's not a number, is it?";
    return -1;
  }
  if (*err == '\0') *err = "Success.";
  else if (n < 0)   *err = "One step ahead :).";
  return n;
}
