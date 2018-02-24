empty int  g = 0;

empty int  func1(int a);

empty int  func2();

empty void  func3(int a);

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
empty void  func()
{
  empty int  a = 1,b = 2;

  func1(a);
  a = func1(1);
  func2();
  a = func2();
  func3(a);
  func3(2);
  func4();
  if(a>1)
  {
    func1(a);
    a = func1(1);
    func2();
    if(b>1)
    {
      a = func2();
      func3(a);
      while(a>1)
      {
        func3(2);
        func4();
        a = func2();
      }
    }
    else
    {
      func3(2);
      func4();
    }
  }
  else
  {
    while(b>1)
    {
      a = func2();
      func3(a);
      func3(2);
      b = b-1;
    }
  }
}
empty int  main()
{
  func();
  return 0;
}
