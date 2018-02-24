empty int  main()
{
  empty int  a = 1,b,c;

  c = a+1;
  AC_Tmp_1 = a-b;
  if(AC_Tmp_1>=0)
  {
    c = a+c;
    AC_Tmp_2 = c-2;
    if(AC_Tmp_2>=0)
    {
      c = c+2;
    }
  }
  else
  {
    c = a-c;
  }
  a = c*a;
  return 0;
}
