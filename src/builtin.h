#ifndef _BUILTIN_H_
#define _BUILTIN_H_

typedef int (*builtin_func_t)(char **);
typedef struct builtin_t {
  const char      *name;
  builtin_func_t function;
} builtin_t;

extern builtin_t builtins[];

enum {
  BI_EXIT,
  BI_HISTORY,
  BI_NUM_BUILTINS // Keep last.
};

builtin_func_t builtin(char *cmd);
int             builtin_exit(char **);
int             builtin_history(char **);

#endif /*_BUILTIN_H_*/
