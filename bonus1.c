#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<unistd.h>

#include "history.h"
#include "util.h"

#define MAX_LINE 80 /*The maximum length command*/
int should_run = 1;/*flag to determine when to exit program*/
history *hist;

/** Execute a builtin command. Returns 1 if the specified command
 * was a builtin and was executed, return 0 otherwise.*/
int builtin(char *cmd) {
  if (strcmp(cmd, "history") == 0) {
    history_pprint(hist);
    return 1;
  } else if (strcmp(cmd, "exit") == 0) {
    should_run = 0;
    return 1;
  }
  return 0;
}

/** Execute a command with the specified arguments, and amount of arguments.
 * This command may be a builtin or an executable file on $PATH.*/
void execute(char **args, int argc) {
  // If the last token is an ampersand, run in background.
  int wait = 0;
  pid_t process_id;

  if (argc == 0) return;
  else if (argc == 1 || args[argc-1][0] != '&')
    wait = 1; //If there's no command followed by an '&', wait for the child.
  else
    args[argc-1] = NULL; // Remove the ampersand.

  // If the command was a builtin, then don't fork but just return.
  if (builtin(args[0])) return;

  process_id = fork();

  if (process_id == 0) {
    // Execute the command.
    execvp(args[0], args);
    osh_error(errno);
    _exit(EXIT_FAILURE);
  } else if ((process_id > 0) && wait)
    waitpid(process_id, NULL, 0); // Parent process waits for child.
  else if (process_id < 0)
    osh_error(errno);

  return;
}

/** This function substitutes !! and !N commands, and adds commands to the
 * history.*/
char *subst(char *line) {
  if (line[0] == '!') {
    if (line[1] == '!') {
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

      line = history_get(hist, n);
      if (line == NULL) {// This line doesn't exist (yet).
        osh_str_error("Can't see into the future.");
        return NULL;
      }
    }
    // Print the substituted line.
    printf("%s\n",line);
  }
  // Add the line to the history.
  history_append(hist, line);
  return line;
}

int tokenize(char **args, char *line) {
  int argc = 0;
  char *current_arg;

  current_arg = args[0] = strtok(line, " ");

  if (current_arg == NULL) return 0;

  argc = 1;
  while ((current_arg = strtok(NULL, " ")) != NULL)
    args[argc++] = current_arg;

  args[argc] = NULL;// NULL-terminate so that execvp knows where to stop.
  return argc;
}

int main(void) {
  char *args[MAX_LINE/2+1];/*command line arguments*/
  hist = history_new(1);

  while (should_run) {
    // TODO: replace readline with own prompt.
    char *line = readline("oshi-> ");
    int argc;
    char *subst_str;

    subst_str = subst(line);
    // If the command was valid, tokenize it and execute.
    if (subst_str) {
      argc = tokenize(args, subst_str);
      execute(args, argc);
      // Free subst_str, because history returns a copy.
      if (subst_str != line) free(subst_str);
    }
    free(line); // Free the memory.
  }
  history_delete(hist); // Delete the history.
  return EXIT_SUCCESS;
}
