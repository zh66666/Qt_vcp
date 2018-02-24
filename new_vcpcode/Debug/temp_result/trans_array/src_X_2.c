empty int  g = 0;

empty int  func1(int );

empty int  func2();

empty void  func3(int );

empty void  func4();

empty int  func1(int a)
{
  empty int  b = 1;

  b = b+g;
  return b;
}
empty int  func2()
{
  empty int  b = 1;

  return b;
}
empty void  func3(int a)
{
  empty int  c = 1;

  empty int  f = 1;

  f = c;
  g = f;
}
empty void  func4()
{
  empty int  f = 1;

  f = g;
}
