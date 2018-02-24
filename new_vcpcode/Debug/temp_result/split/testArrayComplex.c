empty int  Test_V = 5;

empty int  main()
{
  empty int  x = 2;

  empty int  y = 1;

  empty int  z = 3;

  empty int  arr[10];

  arr[0] = y;
  z = arr[0];
  arr[1] = Test_V;
  x = arr[y];
  arr[2] = 9;
  AC_Tmp_1 = arr[1];
  AC_Tmp_2 = AC_Tmp_1<=24;
  AC_IfSub_1 = AC_Tmp_2-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_2==e_TRUE)
  {
    AC_Tmp_3 = arr[2];
    AC_Tmp_4 = Test_V+AC_Tmp_3;
    Test_V = AC_Tmp_4/3;
    AC_Tmp_5 = arr[2];
    z = AC_Tmp_5<<3;
  }
  else
  {
    AC_Tmp_6 = arr[1];
    AC_Tmp_7 = arr[2];
    AC_Tmp_8 = AC_Tmp_6*AC_Tmp_7;
    arr[3] = AC_Tmp_8;
  }
  return ;
  return 0;
}
