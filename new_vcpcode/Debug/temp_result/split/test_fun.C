empty int  fun(int a)
{
  empty int  b;

  b = a+1;
  return b;
}
empty int  fun1()
{
  return 0;
}
empty int  fun2(int a,int b)
{
  return 0;
}
empty int  main(void )
{
  empty int  a = 1;

  empty int  c;

  a = 2;
  c = a;
  c = a+1;
  c = fun(a);
  fun1();
  return c;
}
