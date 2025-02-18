extern int ( SQUARE)() ;
extern int ( printf)() ;
int main(void)
{
  int x ;
  int y ;
  int tmp ;
  {
  x = 5;
  simulate_seu_main(& x);
  tmp = SQUARE(x);
  y = tmp;
  printf("%d\n", y);
  return (0);
}
}
