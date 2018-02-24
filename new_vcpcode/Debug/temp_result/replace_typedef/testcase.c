empty int  fun(int a,int b);

empty void  main()
{
  empty int  a = 10,b = 20,c;

  a = b/5;
  c = a+b;
  while(c>=0)
  {
    if(c>=10)
    {
      a = a+10;
      if(c>=20)
      {
        c = c-2;
      }
      else
      {
        c = c-1;
      }
    }
    else
    {
      b = b-2;
    }
    c = c-1;
  }
  c = fun(a,b);
  c = c*a;
}
empty int  fun(int a,int b)
{
  empty int  c;

  while(a>=0)
  {
    b = b+a;
    if(a>=2)
    {
      a = a-2;
    }
    else
    {
      a = a-1;
    }
  }
  c = a+b;
  return c;
}
