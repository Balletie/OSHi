#include <string.h>
#include <stdio.h>
#include "builtin.h"
#include "prompt.h"

builtin_t  builtins[BI_NUM_BUILTINS] = {{"exit", &builtin_exit},\
                                        {"history", &builtin_history}};

/** Returns the builtin command given by 'cmd'.*/
builtin_func_t builtin(char *cmd) {
  for (int i = 0; i < BI_NUM_BUILTINS; i++)
    if (strcmp(cmd, builtins[i].name) == 0) return builtins[i].function;
  return NULL;
}

int builtin_exit(char **args) {
  return 0;
}

int builtin_history(char **args) {
  prompt_history();
  return 1;
}
