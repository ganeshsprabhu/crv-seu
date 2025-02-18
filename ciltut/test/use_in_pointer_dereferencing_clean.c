int main(void)
{
  int x ;
  int *ptr ;
  int y ;
  {
  x = 10;
  simulate_seu_main(& ptr);
  ptr = & x;
  y = *ptr;
  return (0);
}
}
