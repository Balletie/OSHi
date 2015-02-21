#define _XOPEN_SOURCE
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<unistd.h>

#include "prompt.h"
#include "util.h"

#define MAX_LINE 80 /*The maximum length command*/
int should_run = 1;/*flag to determine when to exit program*/

/** Execute a builtin command. Returns 1 if the specified command
 * was a builtin and was executed, return 0 otherwise.*/
int builtin(char **cmd) {
  if (strcmp(cmd[0], "history") == 0) {
    prompt_history();
    return 1;
  } else if (strcmp(cmd[0], "exit") == 0) {
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
  shell_init();
  prompt_init();

  while (should_run) {
    char *line = NULL;
    should_run = prompt("oshi-> ", &line);
    int argc;

    // If the command was valid, tokenize it and execute.
    if (line) {
      argc = tokenize(args, line);
      // If the command is not a builtin, execute normally.
      if (!builtin(args))
        execute(args, argc);

      free(line); // Free the memory.
    }
  }
  prompt_delete();
  return EXIT_SUCCESS;
}
