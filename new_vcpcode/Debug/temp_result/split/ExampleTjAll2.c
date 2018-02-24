empty int  fun_max(int a,int b);

empty int  Test_result_a = 0;

empty int  Test_result_b = 1;

empty int  fun_max(int a,int b)
{
  empty int  ret = 0;

  AC_Tmp_1 = a>b;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    ret = a;
  }
  else
  {
    ret = b;
  }
  return ret;
}
empty int  main()
{
  empty int  i = 0;

  empty int  j = 1;

  empty int  m = 2;

  empty int  n = 3;

  AC_Tmp_1 = j>i;
  AC_Tmp_2 = m>n;
  AC_Tmp_3 = AC_Tmp_1&&AC_Tmp_2;
  AC_IfSub_1 = AC_Tmp_3-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_3==e_TRUE)
  {
    Test_result_a = 10;
    Test_result_b = fun_max(j,m);
  }
  else
  {
    AC_Tmp_4 = j>i;
    AC_Tmp_5 = m>n;
    AC_Tmp_6 = AC_Tmp_4||AC_Tmp_5;
    AC_IfSub_3 = AC_Tmp_6-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_6==e_TRUE)
    {
      Test_result_a = 100;
    }
    else
    {
      Test_result_b = 2;
    }
  }
  return 0;
}
