#include <stdio.h>

int main()
{
  // ! Dataflow analysis to find out oekind of each var at before executing each instr
  int a, b, c, d;
  a = 1;
  b = 2;
  c = 3;
  d = 4;
  a += b + c;
  c *= d - b;
  b -= d + a;

  // ! with if included:
  // in dataflow we don't know exactly if a%2 branch will get executed or not
  // so in end oekind of a is Top & not Even
  if (a % 2)
    a++;

  // ! by commenting out the if
  // oekind of a is Even

  printf("a = %d, b = %d, c = %d, d = %d\n", a, b, c, d);
  return 0;
}
