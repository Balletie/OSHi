#ifndef _PROMPT_H_
#define _PROMPT_H_

void prompt_init(void);
void prompt_history(void);
int  prompt(const char *, char **);
void prompt_delete(void);

#endif /*_PROMPT_H_*/
