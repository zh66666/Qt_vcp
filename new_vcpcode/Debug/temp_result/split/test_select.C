empty int  main(void )
{
  empty int  a = 1;

  empty int  b = 2;

  empty int  c;

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
  return c;
}
