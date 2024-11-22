
#include <stdio.h>

void tut_begin_loop(const char *f, int l) {}
void tut_end_loop(const char *f, int l, int c)
{
  // ! reports how many iterations were there in the loop
  printf("loop: %s:%d - %d times\n", f, l, c - 1);
  return;
}

int main()
{
  int i, j;
  int c = 1;

  for (i = 0; i < 10; i++)
  {
    for (j = 0; j < 5; j++)
    {
      c *= i;
    }
  }

  printf("c = %d\n", c);
  return 0;
}
