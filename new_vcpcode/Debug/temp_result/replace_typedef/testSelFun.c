empty int  func1(int inA,int inB);

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
  empty int  a = 3;

  empty int  b = 4;

  empty int  c = 10;

  empty int  x;

  empty int  y = 1;

  empty int  z = 5;

  if(c>0)
  {
    x = c;
    z = func1(a,b);
  }
  else
  {
    y = c;
    func2(z);
    z = g;
  }
}
