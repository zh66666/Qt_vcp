empty int  main()
{
  empty int  a = 3;

  empty int  b = 4;

  empty int  c = 10;

  empty int  x;

  empty int  y = 1;

  empty int  z;

  empty int  m;

  empty int  n;

  empty int  p;

  empty int  q;

  AC_Tmp_1 = a>0;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    x = a;
    z = a+b;
    AC_Tmp_2 = b>0;
    AC_IfSub_3 = AC_Tmp_2-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_2==e_TRUE)
    {
      m = 1;
    }
    else
    {
      n = 1;
    }
  }
  else
  {
    y = a;
    z = a-c;
    AC_Tmp_3 = c>0;
    AC_IfSub_5 = AC_Tmp_3-e_TRUE;
    AC_IfSub_6 = 0-AC_IfSub_5;
    if(AC_Tmp_3==e_TRUE)
    {
      p = 1;
    }
    else
    {
      q = 1;
    }
  }
  return 0;
}
