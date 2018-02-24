empty int  fun(int a,int b);

empty void  main()
{
  empty int  a = 10,b = 20,c;

  a = a+b;
  c = fun(a,b);
  c = c*a;
}
empty int  fun(int a,int b)
{
  empty int  c;

  c = a+b;
  return c;
}
