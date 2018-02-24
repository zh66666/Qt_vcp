empty int  g = 0;

empty int  func1(int );

empty int  func2();

empty int  func3(int );

empty int  func4();

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
empty int  func3(int a)
{
  empty int  c = 1;

  empty int  f = 1;

  f = c;
  g = f;
  return 0;
}
empty int  func4()
{
  empty int  f = 1;

  f = g;
  return 0;
}
