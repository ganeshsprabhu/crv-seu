extern int(printf)();
int main(void)
{
  int a;
  int b;
  int c;
  int d;
  {
    a = 1;
    b = 2;
    c = 3;
    d = 4;
    a += b + c;
    c *= d - b;
    b -= d + a;
    printf("a = %d, b = %d, c = %d, d = %d\n", a, b, c, d);
    return (0);
  }
}
