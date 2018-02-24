empty int  main()
{
  empty int  a = 100;

  empty int  b = 120;

  empty int  c = 130;

  empty int  d = 140;

  AC_Tmp_1 = a==0;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = b>10;
    AC_IfSub_3 = AC_Tmp_2-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_2==e_TRUE)
    {
      b = 10+c;
      AC_Tmp_3 = c>=101;
      AC_IfSub_5 = AC_Tmp_3-e_TRUE;
      AC_IfSub_6 = 0-AC_IfSub_5;
      if(AC_Tmp_3==e_TRUE)
      {
        d = 10*b;
        AC_Tmp_4 = b>=10;
        AC_IfSub_7 = AC_Tmp_4-e_TRUE;
        AC_IfSub_8 = 0-AC_IfSub_7;
        if(AC_Tmp_4==e_TRUE)
        {
          c = c*a;
        }
      }
      else
      {
        b = 10;
      }
    }
  }
  else
  {
    c = b;
    AC_Tmp_5 = c>=10;
    AC_IfSub_9 = AC_Tmp_5-e_TRUE;
    AC_IfSub_10 = 0-AC_IfSub_9;
    if(AC_Tmp_5==e_TRUE)
    {
      d = c*a;
    }
    else
    {
      c = a;
    }
  }
  return 0;
}
