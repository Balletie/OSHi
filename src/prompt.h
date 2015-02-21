#ifndef _PROMPT_H_
#define _PROMPT_H_

void prompt_init();
void prompt_history();
int  prompt(const char *, char **);
void prompt_delete();

#endif /*_PROMPT_H_*/
