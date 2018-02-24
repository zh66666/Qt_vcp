empty int  main()
{
  empty int  a = 1,b,c;

  c = a+b;
  AC_Tmp_1 = a-b;
  if(AC_Tmp_1>=0)
  {
    c = a+c;
  }
  else
  {
    c = a-c;
  }
  a = c*a;
  return 0;
}
