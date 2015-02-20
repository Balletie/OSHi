#ifndef _UTIL_H_
#define _UTIL_H_

#define RED_B "\033[1m\033[31m"
#define RESET "\033[0m"

/** Some utility macros for error reporting. Shows a nice fat bold red "error"
 * text.*/
#define osh_str_error(s) fprintf(stderr, RED_B"error: "RESET"%s\n", s)
#define osh_error(x) osh_str_error(strerror(errno))

char *concat(const char*, const char*);
long parse_number(const char *, char **);

#endif /*_UTIL_H_*/
