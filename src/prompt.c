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
    char *extra_string = NULL;
    if (line[1] == '!') {
      extra_string = line + 2;
      line = history_last(hist);
      if (line == NULL) {
        osh_str_error("No commands in history.");
        return NULL;
      }
    } else {
      char *err_str;
      long n = parse_number(line + 1, &err_str);

      if (n < 0) {
        // Parsing the number was not successful. It was
        // either negative or not a number at all.
        osh_str_error(err_str);
        return NULL;
      }
      // err_str contains the leftovers of parsing.
      // Eg; !40asdf, err_str = asdf
      extra_string = err_str;

      line = history_get(hist, n);
      if (line == NULL) {// This line doesn't exist (yet).
        osh_str_error("No such command in history.");
        return NULL;
      }
    }
    res = concat(line, extra_string);
    // Free the memory allocated by history_get or history_last.
    free(line);
    // Print the substituted and concatenated line.
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
int prompt(const char *ps1, char **line_addr) {
  char *line = NULL;
  char *subst_line = NULL;
  size_t length = 0;

  if (ps1)
    printf(ps1);

  // Set errno to 0 to check later if an error occurred.
  errno=0;
  ssize_t num_chars = getline(&line, &length, stdin);

  if (num_chars < 0) {
    free(line);
    *line_addr = NULL;
    if (errno != 0) {
      osh_error(errno);
      return 1; // Keep spinning.
    } else {
      // EOF found, exit the shell.
      printf("exit\n");
      return 0;
    }
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

  *line_addr = subst_line;
  return 1; // Keep spinning
}

void prompt_delete() {
  if (hist != NULL) history_delete(hist);
}
