empty int  g = 0;

empty int  func1(int );

empty int  func2();

empty void  func3(int );

empty void  func4();

empty void  F_VCE_CheckVar(int inVar,int inErrNo)
{
}
empty void  F_VCE_CheckVar(int inVar,int inErrNo)
{
}
empty void  functest(int a)
{
}
empty int  func1()
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
empty void  func2()
{
  empty int  a = 10;

  empty int  b = 1;

  empty int  c = 2;

  empty int  arr[10];

  arr[0] = 1;
  a = arr[0];
  functest(arr[0]);
  F_VCE_CheckVar(b,0);
  if(a>0)
  {
    b = c+1;
    F_VCE_CheckVar(b,1);
  }
  else
  {
    b = a+c;
    F_VCE_CheckVar(b,2);
  }
  while(a>1)
  {
    a = a-1;
    F_VCE_CheckVar(a,3);
  }
}
empty int  main()
{
  func1();
  func2();
  return 0;
}
