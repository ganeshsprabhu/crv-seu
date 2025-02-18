extern int ( printf)() ;
void foo(int val )
{
  {
  printf("%d\n", val);
  return;
}
}
int main(void)
{
  int x ;
  {
  x = 10;
  simulate_seu_main(& x);
  foo(x);
  return (0);
}
}
