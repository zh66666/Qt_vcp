empty void  func1(int a);

empty void  func2();

empty void  func1(int a)
{
  empty int  x = 1;

  empty int  result = 0;

  result = x+1;
  F_VCE_CheckVar(result,1);
}
empty void  main()
{
  empty int  test = 0;

  empty int  param = 1;

  func1(param);
}
