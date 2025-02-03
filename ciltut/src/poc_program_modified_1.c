void printf(char *fmt , ...) ;
int p(int x , int y )
{
  int output ;
  int count ;
  {
  printf("hello\n");
  output = 4;
  count = 0;
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
  return (output);
}
}
