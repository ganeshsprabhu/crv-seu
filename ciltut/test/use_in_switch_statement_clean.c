int main(void)
{
  int x ;
  int y ;
  {
  x = 2;
  {
  simulate_seu_main(& x);
  switch (x) {
  case 1:
  y = 10;
  break;
  default:
  y = 0;
  }
  }
  return (0);
}
}
