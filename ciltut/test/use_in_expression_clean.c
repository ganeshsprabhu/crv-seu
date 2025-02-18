extern int ( printf)() ;
int main(void)
{
  int x ;
  int z ;
  {
  x = 5;
  simulate_seu_main(& x);
  z = x + 1;
  printf("%d\n", z);
  return (0);
}
}
