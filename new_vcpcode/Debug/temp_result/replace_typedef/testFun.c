empty int  func1(int inA,int inB);

empty void  func2(int inC);

empty int  g = 1;

empty int  func1(int inA,int inB)
{
  empty int  r;

  r = inA+inB;
  r = r*2;
  r = r-g;
  return r;
}
empty void  func2(int inC)
{
  g = inC;
}
empty void  main()
{
  empty int  a = 3;

  empty int  b;

  b = func1(a,7);
  func2(b);
}
