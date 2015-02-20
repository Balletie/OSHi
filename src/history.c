#include <math.h>
#include <stdio.h>
#include <string.h>
#include "history.h"

/** Construct a new list with space for initial_size elements. Note that the
 * count is 0.*/
history *history_new(size_t initial_size) {
  char **lines  = (char **) calloc(initial_size, sizeof(char *));
  history *hist = (history *) malloc(sizeof(history));
  hist->lines = lines;
  hist->count = 0;
  hist->size  = initial_size;
  return hist;
}

/** Delete the list, along with the allocated strings.*/
void history_delete(history *hist) {
  for(int i = 0; i < hist->count; i++) {
    free(hist->lines[i]);
  }
  free(hist->lines);
  free(hist);
}

/** Append to the list, resize if needed.*/
void history_append(history *hist, const char *line) {
  if (hist->count == hist->size) {
    char **tmp;
    // Resize the list.
    tmp = (char **)realloc(hist->lines, 2 * hist->size * sizeof(char *));
    if (tmp == NULL) {
      // Realloc went wrong, so don't add the element. Original pointer is
      // left intact.
      return;
    }

    hist->lines = tmp;
    hist->size *= 2;
  }
  // Copy the string
  size_t len = strlen(line) + 1;
  hist->lines[hist->count] = (char *)calloc(len, sizeof(char));
  strncpy(hist->lines[hist->count], line, len);
  hist->count += 1;
}

/** Get the element at index. If index is too large, return NULL*/
char *history_get(history *hist, unsigned index) {
  if (index >= hist->count)
    return NULL;

  // Return a copy for safety.
  char * res;
  size_t len = strlen(hist->lines[index]) + 1;
  res = (char *)calloc(len, sizeof(char));
  strncpy(res, hist->lines[index], len);
  return res;
}

/** Gets the last element of the list, for !!*/
char *history_last(history *hist) {
  if (hist->count == 0) return NULL;
  return history_get(hist, hist->count - 1);
}

/** Pretty prints the history, with padding for the numbers on the left side.*/
void history_pprint(history *hist) {
  unsigned int_length = floor(log10(abs(hist->count))) + 1;
  for (int i = 0; i < hist->count; i++) {
    printf("%*d %s\n",int_length, i, hist->lines[i]);
    fflush(stdout);
  }
}
