#include <stdio.h>

void tut_begin_loop(char const *f, int l)
{
  {
    return;
  }
}
// extern int(printf)();
void tut_end_loop(char const *f, int l, int c)
{
  {
    printf("loop: %s:%d- %d times\n", f, l, c - 1);
    return;
  }
}
int main(void)
{
  int i;
  int j;
  int c;
  int __cil_tmp4;
  int __cil_tmp5;
  {
    c = 1;
    i = 0;
    {
      tut_begin_loop("tut4.c", 16);
      __cil_tmp5 = 0;
      while (1)
      {
        __cil_tmp5++;
        if (!(i < 10))
        {
          break;
        }
        j = 0;
        {
          tut_begin_loop("tut4.c", 18);
          __cil_tmp4 = 0;
          while (1)
          {
            __cil_tmp4++;
            if (!(j < 5))
            {
              break;
            }
            c *= i;
            j++;
          }
          tut_end_loop("tut4.c", 18, __cil_tmp4);
        }
        i++;
      }
      tut_end_loop("tut4.c", 16, __cil_tmp5);
    }
    printf("c = %d\n", c);
    return (0);
  }
}
