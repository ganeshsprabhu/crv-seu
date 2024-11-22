int bar = 37;
int foo(void)
{
  int x ;
  int y ;
  {
  x = bar;
  y = bar;
  return (x + y);
}
}
extern int ( printf)() ;
int main(void)
{
  int r ;
  {
  r = foo();
  printf("r = %d\n", r);
  return (0);
}
}
