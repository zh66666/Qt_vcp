empty int  fun(int a,int b);

empty int  main()
{
  empty int  a = 10,b = 20,c;

  a = b/5;
  c = a+b;
  AC_Tmp_1 = c-0;
  while(AC_Tmp_1>=0)
  {
    AC_Tmp_2 = c-10;
    if(AC_Tmp_2>=0)
    {
      a = a+10;
      AC_Tmp_3 = c-20;
      if(AC_Tmp_3>=0)
      {
        c = c-2;
      }
      else
      {
        c = c-1;
      }
    }
    else
    {
      b = b-2;
    }
    c = c-1;
    AC_Tmp_1 = c-0;
  }
  c = fun(a,b);
  c = c*a;
  return 0;
}
empty int  fun(int a,int b)
{
  empty int  c;

  AC_Tmp_1 = a-0;
  while(AC_Tmp_1>=0)
  {
    b = b+a;
    AC_Tmp_2 = a-2;
    if(AC_Tmp_2>=0)
    {
      a = a-2;
    }
    else
    {
      a = a-1;
    }
    AC_Tmp_1 = a-0;
  }
  c = a+b;
  return c;
}
