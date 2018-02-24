empty int  main()
{
  empty int  a = 3;

  empty int  b = 4;

  empty int  x;

  empty int  z;

  empty int  i = 3;

  empty int  j = 2;

  AC_Tmp_1 = a>0;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = i+j;
    AC_Tmp_3 = AC_Tmp_2>0;
    AC_WhileSub_1 = AC_Tmp_3-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
    while(AC_Tmp_3==e_TRUE)
    {
      i = i-1;
      x = x+j;
      AC_Tmp_2 = i+j;
      AC_Tmp_3 = AC_Tmp_2>0;
      AC_WhileSub_1 = AC_Tmp_3-e_TRUE;
      AC_WhileSub_2 = 0-AC_WhileSub_1;
    }
    z = a+x;
  }
  else
  {
    z = a-b;
  }
  return 0;
}
