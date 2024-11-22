
#include <stdio.h>

int bar = 37;

int foo()
{
  int x, y;
  bar = 0; // ! assignment to global var "bar" will be removed
  x = bar;
  bar = 1; // ! assignment to global var "bar" will be removed
  y = bar;
  return x + y;
}

int main()
{
  int r;
  r = foo();
  printf("r = %d\n", r);
  return 0;
}
