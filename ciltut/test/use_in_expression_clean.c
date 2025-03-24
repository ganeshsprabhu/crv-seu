extern int ( printf)() ;
int main(void)
{
  int x ;
  int z ;
  {
  x = 5;
  z = x + 1;
  simulate_seu_main(& x);
  printf("%d\n", z);
  return (0);
}
}
