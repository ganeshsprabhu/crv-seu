
#include <stdio.h>

int deleted = 37;

int target()
{
  int l;
  deleted = 0; // ! assignment to global var "deleted" will be removed
  l = deleted;
  return l;
}

int main()
{
  int r;
  r = target();
  printf("r = %d\n", r);
  return 0;
}
