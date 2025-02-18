extern int ( printf)() ;
int main(void)
{
  int x ;
  int y ;
  {
  x = 5;
  y = 0;
  {
  simulate_seu_main(& x);
  if (x > 10) {
    y = 1;
  }
  }
  printf("%d\n", y);
  return (0);
}
}
