int p(int x , int y )
{
  int output ;
  int count ;
  {
  output = 4;
  count = 0;
  while (count < 7) {
    {
    simulate_seu_main(& x);
    if (x > 10) {
      output = x + 1;
    }
    }
    {
    simulate_seu_main(& x);
    if (x < 5) {
      count = x;
    }
    }
    count ++;
  }
  return (output);
}
}
