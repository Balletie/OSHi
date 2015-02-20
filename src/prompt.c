#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "history.h"
#include "prompt.h"
#include "util.h"

static history *hist;

/** This function substitutes !! and !N commands. It takes a 'line', and returns
 * the substituted line from history, 'line' if the command was not
 * substituted, NULL when an error occurred (such as '!asdf')*/
static char *subst(char *line) {
  char *res = line;
  if (line[0] == '!') {
    if (line[1] == '!') {
      res = line; // Re-use res as a temporary here.
      line = history_last(hist);
      if (line == NULL) {
        osh_str_error("No commands in history.");
        return NULL;
      }
      res = concat(line, res+2);
      // Free the memory allocated by history_last.
      free(line);
    } else {
      char *err_str;
      long n = parse_number(line + 1, &err_str);

      if (n < 0) {
        // Parsing the number was not successful. It was
        // either negative or not a number at all.
        osh_str_error(err_str);
        return NULL;
      }

      line = history_get(hist, n);
      if (line == NULL) {// This line doesn't exist (yet).
        osh_str_error("No such command in history.");
        return NULL;
      }
      // err_str has the rest of the parsed number, concatenate it.
      res = concat(line, err_str);
      // Free the memory allocated by history_get.
      free(line);
    }
    // Print the substituted line.
    printf("%s\n",res);
  }
  return res;
}

void prompt_init() {
  if (hist == NULL) hist = history_new(1);
}

void prompt_history() {
  history_pprint(hist);
}

/** Displays a prompt with an optional char * 'ps1', reads the user's input and
 * adds the line to the history. Output is the effective command, or NULL when
 * and error occurred.*/
char *prompt(const char *ps1) {
  char *line = NULL;
  char *subst_line = NULL;
  size_t length = 0;

  if (ps1)
    printf(ps1);

  ssize_t num_chars = getline(&line, &length, stdin);

  if (num_chars < 0) {
    osh_error(errno);
    free(line);
    return NULL;
  }
  // remove newline character.
  if (line[num_chars - 1] == '\n')
    line[num_chars - 1] = '\0';

  // Substitute !! and !N commands.
  subst_line = subst(line);

  // If the substituted line does not point to the same address as the raw line,
  // then free the memory.
  if (subst_line != line) free(line);
  if (subst_line != NULL)
    // Add the line to the history.
    history_append(hist, subst_line);

  return subst_line;
}

void prompt_delete() {
  if (hist != NULL) history_delete(hist);
}
