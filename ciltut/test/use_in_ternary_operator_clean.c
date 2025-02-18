int main(void)
{
  int x ;
  int y ;
  {
  x = 10;
  {
  simulate_seu_main(& x);
  if (x > 5) {
    y = 1;
  } else {
    y = 0;
  }
  }
  return (0);
}
}
