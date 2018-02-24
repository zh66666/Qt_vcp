empty int  main()
{
  empty int  a = 100;

  empty int  b = 120;

  empty int  c = 130;

  empty int  d = 140;

  AC_Tmp_1 = a>0;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = a<=30;
    AC_IfSub_1 = AC_Tmp_2-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_2==e_TRUE)
    {
      c = d*10;
      b = 130;
      AC_Tmp_3 = b<=100;
      AC_WhileSub_3 = AC_Tmp_3-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
      while(AC_Tmp_3==e_TRUE)
      {
        c = c+10;
        b = b-1;
        AC_Tmp_3 = b<=100;
        AC_WhileSub_3 = AC_Tmp_3-e_TRUE;
        AC_WhileSub_4 = 0-AC_WhileSub_3;
      }
    }
    else
    {
      c = 10;
      AC_Tmp_4 = c>=0;
      AC_WhileSub_5 = AC_Tmp_4-e_TRUE;
      AC_WhileSub_6 = 0-AC_WhileSub_5;
      while(AC_Tmp_4==e_TRUE)
      {
        d = 10;
        b = d<<2;
        c = c-1;
        AC_Tmp_4 = c>=0;
        AC_WhileSub_5 = AC_Tmp_4-e_TRUE;
        AC_WhileSub_6 = 0-AC_WhileSub_5;
      }
    }
    a = a-2;
    AC_Tmp_1 = a>0;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  return 0;
}
