// dive.c
#include <__fc_builtin.h>
volatile int nondet;
unsigned f(char a, unsigned b, unsigned c)
{
  unsigned res;
  if (b > 10)
  {
    res = 5;
  }
  else if (b > 1)
  {
    res = a;
  }
  else
  {
    res = c / 2;
  }
  return res;
}

int main(void)
{
  char a = 42;
  unsigned b = Frama_C_unsigned_int_interval(0, 100);
  unsigned c = Frama_C_unsigned_int_interval(0, (unsigned)-1);
  unsigned r = f(a, b, c);
  char arr[100];
  arr[r] = 1; // alarm
  return 0;
}