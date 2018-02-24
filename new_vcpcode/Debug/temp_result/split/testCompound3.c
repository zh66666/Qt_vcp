empty int  g = 0;

empty int  arr[4] = { 1,2,3,4};

empty int  fun(int in)
{
  g = in;
  return 0;
}
empty int  main()
{
  empty int  a = 0;

  empty int  b = 12;

  empty int  i;

  AC_Tmp_1 = i<4;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    arr[i] = g;
    i = i+1;
    AC_Tmp_1 = i<4;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  AC_Tmp_2 = b>0;
  AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
  AC_WhileSub_4 = 0-AC_WhileSub_3;
  while(AC_Tmp_2==e_TRUE)
  {
    AC_Void_Func_RetVal = fun(b);
    b = b-2;
    AC_Tmp_2 = b>0;
    AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
  }
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return 0;
}
