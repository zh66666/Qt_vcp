empty int  func1(int a);

empty void  func2();

empty int  func1(int a)
{
  empty int  x = 1;

  empty int  result = 0;

  result = x+1;
  F_VCE_CheckVar(result,1);
  return result;
}
empty void  main()
{
  empty int  test = 0;

  empty int  param = 1;

  test = func1(param);
  F_VCE_CheckVar(test,2);
}
