int p(int x , int y )
{
  int output ;
  int count ;
  {
  output = 4;
  count = 0;
  {
  simulate_seu_main(& output);
  while (count < 7) {
    if (x > 10) {
      if (y == 1) {
        output = 2;
      } else {
        output = 1;
      }
    } else {
      output ++;
    }
    count ++;
  }
  }
  return (output);
}
}
int main(void)
{
  int output ;
  int x ;
  int y ;
  {
  output = p(x, y);
  return (0);
}
}
