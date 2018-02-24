empty int  Test_result = 0;

empty int  main()
{
  empty int  x = 1;

  empty int  y = -1;

  empty int  z = 0;

  empty int  m = 1;

  empty int  n = 5;

  empty int  i = 0;

  empty int  j = 0;

  i = 1;
  j = i;
  Test_result = j;
  x = x+1;
  x = 1+x;
  x = x+x;
  y = y-1;
  y = -2-y;
  y = y-2;
  y = x+y;
  m = m*-2;
  m = -3*m;
  m = 1*10;
  m = m*n;
  n = n/2;
  AC_Tmp_1 = x+1;
  AC_Tmp_2 = AC_Tmp_1/2;
  AC_Tmp_3 = y-2;
  AC_Tmp_4 = AC_Tmp_3*y;
  AC_Tmp_5 = AC_Tmp_2+AC_Tmp_4;
  z = AC_Tmp_5*3;
  AC_Tmp_6 = -1+x;
  AC_Tmp_7 = AC_Tmp_6/3;
  AC_Tmp_8 = -2+y;
  AC_Tmp_9 = AC_Tmp_8*y;
  AC_Tmp_10 = AC_Tmp_7+AC_Tmp_9;
  z = AC_Tmp_10*-3;
  AC_Tmp_11 = i+2;
  AC_Tmp_12 = AC_Tmp_11>>2;
  AC_Tmp_13 = j+100;
  AC_Tmp_14 = AC_Tmp_12+AC_Tmp_13;
  z = AC_Tmp_14<<2;
  AC_Tmp_15 = x+y;
  AC_Tmp_16 = AC_Tmp_15+z;
  AC_Tmp_17 = AC_Tmp_16*m;
  AC_Tmp_18 = AC_Tmp_17/2;
  AC_Tmp_19 = i*n;
  AC_Tmp_20 = AC_Tmp_18-AC_Tmp_19;
  Test_result = AC_Tmp_20+j;
  return 0;
}
