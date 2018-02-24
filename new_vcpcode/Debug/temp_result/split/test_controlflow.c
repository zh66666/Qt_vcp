empty int  main(void )
{
  empty int  i = 10;

  empty int  a = 1;

  empty int  b = 2;

  empty int  c;

  c = a+b;
  AC_Tmp_1 = a-2;
  if(AC_Tmp_1>=0)
  {
    c = a+b;
    a = b+c;
  }
  else
  {
    c = a-b;
  }
  c = a+b;
  AC_Tmp_2 = i-0;
  while(AC_Tmp_2>=0)
  {
    a = a+1;
    i = i-1;
    AC_Tmp_2 = i-0;
  }
  c = a+b;
  return c;
}
