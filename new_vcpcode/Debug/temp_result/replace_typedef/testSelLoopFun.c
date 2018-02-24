empty int  g = 1;

empty int  func1(int inA,int inB)
{
  empty int  r;

  r = inA+inB;
  return r;
}
empty void  func2(int inC)
{
  g = inC;
}
empty void  main()
{
  empty int  i = 3;

  empty int  j = 2;

  empty int  x = 0;

  empty int  y = 30;

  empty int  z;

  while(i+j>0)
  {
    i = i-1;
    x = func1(i,j);
    while(y-x>0)
    {
      func2(y);
      y = y-2;
    }
    if(y>x)
    {
      z = i;
    }
    else
    {
      z = j;
    }
  }
}
