empty int  func1(int inA,int inB);

empty int  g = 1;

empty int  func1(int inA,int inB)
{
  empty int  r;

  r = inA+inB;
  return r;
}
empty int  func2(int inC)
{
  g = inC;
  return 0;
}
empty int  main()
{
  empty int  a = 3;

  empty int  b = 4;

  empty int  c = 10;

  empty int  x;

  empty int  y = 1;

  empty int  z = 5;

  AC_Tmp_1 = c>0;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    x = c;
    z = func1(a,b);
  }
  else
  {
    y = c;
    AC_Void_Func_RetVal = func2(z);
    z = g;
  }
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return 0;
}
