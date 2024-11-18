int bar = 37;
int foo(void)
{
  int l ;
  {
  l = bar;
  return (l);
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
