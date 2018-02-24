empty int  func1();

empty int  main();

empty int  Test_V;

empty int  func1()
{
  empty int  x = 1;

  empty int  result = 0;

  result = x+1;
  return 0;
}
empty int  main()
{
  empty int  x = 3;

  empty int  y = 4;

  empty int  z = 5;

  AC_Tmp_1 = y*z;
  Test_V = x+AC_Tmp_1;
  AC_Tmp_2 = Test_V<=24;
  AC_IfSub_1 = AC_Tmp_2-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_2==e_TRUE)
  {
    AC_Void_Func_RetVal = func1();
  }
  else
  {
    AC_Void_Func_RetVal = func1();
  }
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return 0;
}
