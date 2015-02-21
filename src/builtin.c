#include <string.h>
#include <stdio.h>
#include "builtin.h"
#include "prompt.h"

/** Builtin table: To create a new builtin:
 *    1) Add enum value in builtin.h (keep BI_NUM_BUILTINS last)
 *    2) Implement function
 *    3) Put struct with name and function pointer in the table below.*/
builtin_t builtins[BI_NUM_BUILTINS] = {{"exit",    &builtin_exit},\
                                       {"history", &builtin_history}};

/** Returns the builtin command given by 'cmd'.*/
builtin_func_t builtin(char *cmd) {
  for (int i = 0; i < BI_NUM_BUILTINS; i++)
    if (strcmp(cmd, builtins[i].name) == 0) return builtins[i].function;
  return NULL;
}

/** Just return 0 (false), causing the shell to exit.*/
int builtin_exit(char **args) {
  return 0;
}

/** Show the shell history.*/
int builtin_history(char **args) {
  prompt_history();
  return 1;
}
