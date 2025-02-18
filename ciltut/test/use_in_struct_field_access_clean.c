struct S {
   int x ;
};
int main(void)
{
  struct S s ;
  int y ;
  {
  s.x = 5;
  y = s.x;
  return (0);
}
}
