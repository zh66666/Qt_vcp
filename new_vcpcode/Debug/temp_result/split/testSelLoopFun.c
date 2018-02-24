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
  empty int  i = 3;

  empty int  j = 2;

  empty int  x = 0;

  empty int  y = 30;

  empty int  z;

  AC_Tmp_1 = i+j;
  AC_Tmp_2 = AC_Tmp_1>0;
  AC_WhileSub_1 = AC_Tmp_2-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_2==e_TRUE)
  {
    i = i-1;
    x = func1(i,j);
    AC_Tmp_3 = y-x;
    AC_Tmp_4 = AC_Tmp_3>0;
    AC_WhileSub_3 = AC_Tmp_4-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
    while(AC_Tmp_4==e_TRUE)
    {
      AC_Void_Func_RetVal = func2(y);
      y = y-2;
      AC_Tmp_3 = y-x;
      AC_Tmp_4 = AC_Tmp_3>0;
      AC_WhileSub_3 = AC_Tmp_4-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
    }
    AC_Tmp_5 = y>x;
    AC_IfSub_1 = AC_Tmp_5-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_5==e_TRUE)
    {
      z = i;
    }
    else
    {
      z = j;
    }
    AC_Tmp_1 = i+j;
    AC_Tmp_2 = AC_Tmp_1>0;
    AC_WhileSub_1 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return 0;
}
