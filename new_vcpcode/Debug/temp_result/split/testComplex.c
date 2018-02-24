empty int  Test_result = 0;

empty int  main()
{
  empty int  x = 1;

  empty int  y = 2;

  empty int  z = 3;

  empty int  temp = 0;

  temp = 1;
  x = x+1;
  y = 5-y;
  z = z*2;
  AC_Tmp_1 = x+y;
  AC_Tmp_2 = AC_Tmp_1*z;
  temp = AC_Tmp_2/3;
  Test_result = temp<<2;
  return ;
  return 0;
}
