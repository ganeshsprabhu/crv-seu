int main(void)
{
  int arr[10] ;
  unsigned int tmp ;
  int x ;
  int y ;
  {
  arr[0] = 0;
  tmp = 1U;
  {
  simulate_seu_main(& x);
  while (! (tmp >= 10U)) {
    arr[tmp] = 0;
    tmp ++;
  }
  }
  x = 3;
  y = arr[x];
  simulate_seu_main(& arr[x]);
  return (0);
}
}
