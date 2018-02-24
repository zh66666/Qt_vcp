empty int  func1(int a);

empty int  func2();

empty int  func1(int a)
{
  empty int  x = 1;

  empty int  result = 0;

  result = x+1;
  F_VCE_CheckVar(result,1);
  return 0;
}
empty int  main()
{
  empty int  test = 0;

  empty int  param = 1;

  func1(param);
  return 0;
}
