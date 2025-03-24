extern int ( SQUARE)() ;
extern int ( printf)() ;
int main(void)
{
  int x ;
  int y ;
  int tmp ;
  {
  x = 5;
  tmp = SQUARE(x);
  simulate_seu_main(& x);
  y = tmp;
  printf("%d\n", y);
  return (0);
}
}
