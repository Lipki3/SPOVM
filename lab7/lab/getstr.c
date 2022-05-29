#include "lab.h"

void getstr(char *str, int n)
{

  char ch;

  while ((ch = getchar()) != '\n' && --n)
    *str++ = ch;

  *str = '\0';

  return;
}
