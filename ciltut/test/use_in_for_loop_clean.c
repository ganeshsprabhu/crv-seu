extern int ( printf)() ;
int main(void)
{
  int x ;
  int i ;
  {
  x = 10;
  i = 0;
  {
  simulate_seu_main(& x);
  while (i < x) {
    printf("%d ", i);
    i ++;
  }
  }
  return (0);
}
}
