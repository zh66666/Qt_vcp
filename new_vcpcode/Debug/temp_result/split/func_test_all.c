empty int  g = 0;

empty int  func1(int a);

empty int  func2();

empty int  func3(int a);

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
empty int  func()
{
  empty int  a = 1,b = 2;

  func1(a);
  a = func1(1);
  func2();
  a = func2();
  func3(a);
  func3(2);
  func4();
  AC_Tmp_1 = a>1;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Void_Func_RetVal = func1(a);
    a = func1(1);
    AC_Void_Func_RetVal = func2();
    AC_Tmp_2 = b>1;
    AC_IfSub_3 = AC_Tmp_2-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_2==e_TRUE)
    {
      a = func2();
      AC_Void_Func_RetVal = func3(a);
      AC_Tmp_3 = a>1;
      AC_WhileSub_1 = AC_Tmp_3-e_TRUE;
      AC_WhileSub_2 = 0-AC_WhileSub_1;
      while(AC_Tmp_3==e_TRUE)
      {
        AC_Void_Func_RetVal = func3(2);
        AC_Void_Func_RetVal = func4();
        a = func2();
        AC_Tmp_3 = a>1;
        AC_WhileSub_1 = AC_Tmp_3-e_TRUE;
        AC_WhileSub_2 = 0-AC_WhileSub_1;
      }
    }
    else
    {
      AC_Void_Func_RetVal = func3(2);
      AC_Void_Func_RetVal = func4();
    }
  }
  else
  {
    AC_Tmp_4 = b>1;
    AC_WhileSub_3 = AC_Tmp_4-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
    while(AC_Tmp_4==e_TRUE)
    {
      a = func2();
      AC_Void_Func_RetVal = func3(a);
      AC_Void_Func_RetVal = func3(2);
      b = b-1;
      AC_Tmp_4 = b>1;
      AC_WhileSub_3 = AC_Tmp_4-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
    }
  }
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return 0;
}
empty int  main()
{
  func();
  return 0;
}
