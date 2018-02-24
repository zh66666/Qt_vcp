empty int  func1();

empty int  func2();

empty int  func3(int a);

empty int  func4();

empty int  matrix_multi();

empty int  prime_5000();

empty int  loop_ten();

empty int  graph();

empty int  line(int x1,int y1,int x2,int y2);

empty int  initgraph(int indriver,int inmode);

empty int  setcolor(int incolor);

empty int  func1()
{
  empty int  i = 1,j = 1,k = 1;

  empty int  x = 2,y = 1,z = 9,t = 0;

  AC_Tmp_1 = i<5;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    j = 1;
    AC_Tmp_2 = j<5;
    AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
    while(AC_Tmp_2==e_TRUE)
    {
      k = 1;
      AC_Tmp_3 = k<5;
      AC_WhileSub_5 = AC_Tmp_3-e_TRUE;
      AC_WhileSub_6 = 0-AC_WhileSub_5;
      while(AC_Tmp_3==e_TRUE)
      {
        AC_Tmp_4 = i!=k;
        AC_Tmp_5 = i!=j;
        AC_Tmp_6 = j!=k;
        AC_Tmp_7 = AC_Tmp_4&&AC_Tmp_5;
        AC_Tmp_8 = AC_Tmp_7&&AC_Tmp_6;
        AC_IfSub_1 = AC_Tmp_8-e_TRUE;
        AC_IfSub_2 = 0-AC_IfSub_1;
        if(AC_Tmp_8==e_TRUE)
        {
          ;
        }
        k = k+1;
        AC_Tmp_3 = k<5;
        AC_WhileSub_5 = AC_Tmp_3-e_TRUE;
        AC_WhileSub_6 = 0-AC_WhileSub_5;
      }
      j = j+1;
      AC_Tmp_2 = j<5;
      AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
    }
    i = i+1;
    AC_Tmp_1 = i<5;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  AC_Tmp_9 = x>y;
  AC_IfSub_3 = AC_Tmp_9-e_TRUE;
  AC_IfSub_4 = 0-AC_IfSub_3;
  if(AC_Tmp_9==e_TRUE)
  {
    t = x;
    x = y;
    y = t;
  }
  AC_Tmp_10 = x>z;
  AC_IfSub_5 = AC_Tmp_10-e_TRUE;
  AC_IfSub_6 = 0-AC_IfSub_5;
  if(AC_Tmp_10==e_TRUE)
  {
    t = z;
    z = x;
    x = t;
  }
  AC_Tmp_11 = y>z;
  AC_IfSub_7 = AC_Tmp_11-e_TRUE;
  AC_IfSub_8 = 0-AC_IfSub_7;
  if(AC_Tmp_11==e_TRUE)
  {
    t = y;
    y = z;
    z = t;
  }
  return 0;
}
empty int  func2()
{
  empty int  i,j,result,n,k;

  i = 1;
  AC_Tmp_1 = i<10;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    j = 1;
    AC_Tmp_2 = j<10;
    AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
    while(AC_Tmp_2==e_TRUE)
    {
      result = i*j;
      j = j+1;
      AC_Tmp_2 = j<10;
      AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
    }
    i = i+1;
    AC_Tmp_1 = i<10;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  n = 100;
  AC_Tmp_3 = n<1000;
  AC_WhileSub_5 = AC_Tmp_3-e_TRUE;
  AC_WhileSub_6 = 0-AC_WhileSub_5;
  while(AC_Tmp_3==e_TRUE)
  {
    i = n/100;
    j = n/10;
    k = n;
    AC_Tmp_4 = i*100;
    AC_Tmp_5 = j*10;
    AC_Tmp_6 = AC_Tmp_4+AC_Tmp_5;
    AC_Tmp_7 = AC_Tmp_6+k;
    AC_Tmp_8 = i*i;
    AC_Tmp_9 = AC_Tmp_8*i;
    AC_Tmp_10 = j*j;
    AC_Tmp_11 = AC_Tmp_10*j;
    AC_Tmp_12 = k*k;
    AC_Tmp_13 = AC_Tmp_12*k;
    AC_Tmp_14 = AC_Tmp_9+AC_Tmp_11;
    AC_Tmp_15 = AC_Tmp_14+AC_Tmp_13;
    AC_Tmp_16 = AC_Tmp_7==AC_Tmp_15;
    AC_IfSub_1 = AC_Tmp_16-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_16==e_TRUE)
    {
      result = n;
    }
    n = n+1;
    AC_Tmp_3 = n<1000;
    AC_WhileSub_5 = AC_Tmp_3-e_TRUE;
    AC_WhileSub_6 = 0-AC_WhileSub_5;
  }
  return result;
}
empty int  func3(int a)
{
  empty int  i,aa[4],t,temp = 0;

  aa[0] = a;
  AC_Tmp_1 = a/10;
  aa[1] = AC_Tmp_1;
  AC_Tmp_2 = a/100;
  aa[2] = AC_Tmp_2;
  AC_Tmp_3 = a/1000;
  aa[3] = AC_Tmp_3;
  i = 0;
  AC_Tmp_4 = i<=3;
  AC_WhileSub_1 = AC_Tmp_4-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_4==e_TRUE)
  {
    AC_Tmp_5 = aa[i];
    AC_Tmp_6 = AC_Tmp_5+5;
    aa[i] = AC_Tmp_6;
    AC_Tmp_7 = aa[i];
    AC_Tmp_8 = AC_Tmp_7/10;
    aa[i] = AC_Tmp_8;
    i = i+1;
    AC_void_func_RetVal = func2();
    AC_Tmp_4 = i<=3;
    AC_WhileSub_1 = AC_Tmp_4-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  i = 0;
  AC_Tmp_9 = 3/2;
  AC_Tmp_10 = i<=AC_Tmp_9;
  AC_WhileSub_3 = AC_Tmp_10-e_TRUE;
  AC_WhileSub_4 = 0-AC_WhileSub_3;
  while(AC_Tmp_10==e_TRUE)
  {
    t = aa[i];
    temp = 3-i;
    AC_Tmp_11 = aa[temp];
    aa[i] = AC_Tmp_11;
    aa[temp] = t;
    i = i+1;
    AC_Tmp_9 = 3/2;
    AC_Tmp_10 = i<=AC_Tmp_9;
    AC_WhileSub_3 = AC_Tmp_10-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
  }
  F_VCE_CheckVar(AC_void_func_RetVal,0);
  return 0;
}
empty int  func4()
{
  empty int  i = 0,k = 0,m = 0,n = 0,p[100];

  i = 0;
  AC_Tmp_1 = i<100;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = i+1;
    p[i] = AC_Tmp_2;
    k = 0;
    m = 0;
    i = i+1;
    AC_Tmp_1 = i<100;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  m = 0;
  AC_Tmp_3 = m<100;
  AC_WhileSub_3 = AC_Tmp_3-e_TRUE;
  AC_WhileSub_4 = 0-AC_WhileSub_3;
  while(AC_Tmp_3==e_TRUE)
  {
    AC_Tmp_4 = p[i];
    AC_Tmp_5 = AC_Tmp_4!=0;
    AC_IfSub_1 = AC_Tmp_5-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_5==e_TRUE)
    {
      k = k+1;
    }
    AC_Tmp_6 = k==3;
    AC_IfSub_3 = AC_Tmp_6-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_6==e_TRUE)
    {
      p[i] = 0;
      k = 0;
      m = m+1;
    }
    i = i+1;
    AC_Tmp_7 = i==n;
    AC_IfSub_5 = AC_Tmp_7-e_TRUE;
    AC_IfSub_6 = 0-AC_IfSub_5;
    if(AC_Tmp_7==e_TRUE)
    {
      i = 0;
    }
    m = m+1;
    AC_Tmp_3 = m<100;
    AC_WhileSub_3 = AC_Tmp_3-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
  }
  return 0;
}
