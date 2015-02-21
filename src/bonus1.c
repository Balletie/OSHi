#define _XOPEN_SOURCE // Needed for signal.
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<unistd.h>

#include "builtin.h"
#include "prompt.h"
#include "util.h"

#define MAX_LINE 80 /*The maximum length command*/

/** Execute a command with the specified arguments, and amount of arguments.
 * This command may be a builtin or an executable file on $PATH.
 * The return value indicates whether the shell should exit.*/
int execute(char **args, int argc) {
  if (argc == 0) return 1; // No command.
  builtin_func_t bi;

  // If the command is a builtin, execute that instead.
  if ((bi = builtin(args[0]))) return (*bi)(args);

  // If the last token is an ampersand, run in background.
  int wait = 0;
  pid_t process_id;

  if (argc == 1 || args[argc-1][0] != '&')
    wait = 1; //If there's no command followed by an '&', wait for the child.
  else
    args[argc-1] = NULL; // Remove the ampersand.

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

  return 1;
}

/** Tokenize the string and put the tokens in the args array.*/
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

void shell_init() {
  struct sigaction sa;
  // TODO: make own handler for retrieving exit status, etc.
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  // No flags
  sa.sa_flags = 0;
  if (sigaction(SIGCHLD, &sa, NULL) < 0)
    osh_error(errno);
}

int main(void) {
  char *args[MAX_LINE/2+1];/*command line arguments*/
  int should_run = 1;/*flag to determine when to exit program*/
  shell_init();
  prompt_init();

  while (should_run) {
    char *line = NULL;
    should_run = prompt("oshi-> ", &line);
    int argc;

    // If the command was valid, tokenize it and execute.
    if (line) {
      argc = tokenize(args, line);
      should_run = execute(args, argc);

      free(line); // Free the memory.
    }
  }
  prompt_delete();
  return EXIT_SUCCESS;
}
