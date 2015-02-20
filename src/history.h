#ifndef _HISTORY_H_
#define _HISTORY_H_
#include <stdlib.h>

typedef struct history {
  char     **lines;
  unsigned count;
  size_t   size;
} history;

history *history_new(size_t);
void history_delete(history*);

void history_append(history*, const char *);
char *history_get(history*, unsigned);
char *history_last(history*);
void history_pprint(history*);

#endif /*_HISTORY_H_*/

