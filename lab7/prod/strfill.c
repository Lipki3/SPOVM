#include "prod.h"

void strfill(char *str, size_t n)
{
  if (!str || !n)
    return;

  while (--n)
    *str++ = rand() % ('z' - 'a' + 1) + 'a';

  *str = '\0';

  return;
}
