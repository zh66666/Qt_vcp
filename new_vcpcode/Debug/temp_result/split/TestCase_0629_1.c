typedef int  huffnode;

typedef int  huffcode;

empty int  b[5] = { 11,44,33,22,55};

empty int  g = 6;

empty int  jsValue()
{
  empty int  a1 = 1,a2 = 1,a12,sn,k = 2;

  sn = a1+a2;
  AC_Tmp_1 = k<5;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = 2*a2;
    a12 = a1+AC_Tmp_2;
    AC_Tmp_3 = sn<100;
    AC_Tmp_4 = sn+a12;
    AC_Tmp_5 = AC_Tmp_4>=100;
    AC_Tmp_6 = AC_Tmp_3&&AC_Tmp_5;
    AC_IfSub_1 = AC_Tmp_6-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_6==e_TRUE)
    {
      b[0] = k;
    }
    AC_Tmp_7 = sn<1000;
    AC_Tmp_8 = sn+a12;
    AC_Tmp_9 = AC_Tmp_8>=1000;
    AC_Tmp_10 = AC_Tmp_7&&AC_Tmp_9;
    AC_IfSub_3 = AC_Tmp_10-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_10==e_TRUE)
    {
      b[1] = k;
    }
    AC_Tmp_11 = sn<10000;
    AC_Tmp_12 = sn+a12;
    AC_Tmp_13 = AC_Tmp_12>=10000;
    AC_Tmp_14 = AC_Tmp_11&&AC_Tmp_13;
    AC_IfSub_5 = AC_Tmp_14-e_TRUE;
    AC_IfSub_6 = 0-AC_IfSub_5;
    if(AC_Tmp_14==e_TRUE)
    {
      b[2] = k;
    }
    sn = sn+a12;
    a1 = a2;
    a2 = a12;
    k = k+1;
    ;
    AC_Tmp_1 = k<5;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  return 0;
}
empty int  insertSort(int n)
{
  empty int  i = 2,j,tempi,tempj,m = 1000;

  AC_Tmp_1 = i<n;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    tempi = i-1;
    tempj = j+1;
    AC_Tmp_2 = b[i];
    AC_Tmp_3 = b[tempi];
    AC_Tmp_4 = AC_Tmp_2<AC_Tmp_3;
    AC_IfSub_1 = AC_Tmp_4-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_4==e_TRUE)
    {
      AC_Tmp_5 = b[i];
      b[0] = AC_Tmp_5;
      j = i-1;
      AC_Tmp_6 = b[0];
      b[tempj] = AC_Tmp_6;
    }
    i = i+1;
    AC_Tmp_1 = i<n;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  AC_Tmp_7 = m>>2;
  m = AC_Tmp_7+j;
  return 0;
}
empty int  huffMan()
{
  empty int  huff_node[295];

  empty int  huff_code[930],cd[31];

  empty int  i = 0,j = 1,m1 = 1000,m2 = 1000,x1 = 0,x2 = 1,n,c,p;

  AC_Tmp_1 = i*5;
  huff_node[AC_Tmp_1] = 0;
  AC_Tmp_2 = i*5;
  AC_Tmp_3 = AC_Tmp_2+2;
  huff_node[AC_Tmp_3] = 0;
  AC_Tmp_4 = i*5;
  AC_Tmp_5 = AC_Tmp_4+1;
  huff_node[AC_Tmp_5] = 0;
  AC_Tmp_6 = i*5;
  AC_Tmp_7 = AC_Tmp_6+3;
  huff_node[AC_Tmp_7] = -1;
  AC_Tmp_8 = i*5;
  AC_Tmp_9 = AC_Tmp_8+4;
  huff_node[AC_Tmp_9] = -1;
  AC_Tmp_10 = huff_node[0];
  AC_Tmp_11 = AC_Tmp_10<m1;
  AC_Tmp_12 = huff_node[1];
  AC_Tmp_13 = AC_Tmp_12==0;
  AC_Tmp_14 = AC_Tmp_11&&AC_Tmp_13;
  AC_IfSub_1 = AC_Tmp_14-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_14==e_TRUE)
  {
    m2 = 1000;
    x2 = x1;
    m1 = huff_node[0];
    x1 = j;
    AC_Tmp_15 = m2<<1;
    AC_Tmp_16 = AC_Tmp_15/4;
    AC_Tmp_17 = b[0];
    m2 = AC_Tmp_16+AC_Tmp_17;
  }
  return 0;
}
empty int  logicFun()
{
  empty unsigned int  a = 17;

  empty unsigned int  b = 0;

  empty unsigned int  s = 16;

  empty unsigned int  x = 1;

  empty int  z = 10;

  AC_Tmp_1 = a!=0;
  AC_Tmp_2 = b!=0;
  AC_Tmp_3 = AC_Tmp_1&&AC_Tmp_2;
  AC_IfSub_1 = AC_Tmp_3-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_3==e_TRUE)
  {
    z = z-1;
  }
  else
  {
    z = z+1;
  }
  AC_Tmp_4 = a!=0;
  AC_Tmp_5 = s!=0;
  AC_Tmp_6 = AC_Tmp_4&&AC_Tmp_5;
  AC_IfSub_3 = AC_Tmp_6-e_TRUE;
  AC_IfSub_4 = 0-AC_IfSub_3;
  if(AC_Tmp_6==e_TRUE)
  {
    g = g<<1;
  }
  else
  {
    g = g>>1;
  }
  AC_Tmp_7 = a!=0;
  AC_Tmp_8 = x!=0;
  AC_Tmp_9 = AC_Tmp_7&&AC_Tmp_8;
  AC_IfSub_5 = AC_Tmp_9-e_TRUE;
  AC_IfSub_6 = 0-AC_IfSub_5;
  if(AC_Tmp_9==e_TRUE)
  {
    x = 17;
  }
  else
  {
    a = 17;
  }
  AC_Tmp_10 = s!=0;
  AC_Tmp_11 = x!=0;
  AC_Tmp_12 = AC_Tmp_10&&AC_Tmp_11;
  AC_IfSub_7 = AC_Tmp_12-e_TRUE;
  AC_IfSub_8 = 0-AC_IfSub_7;
  if(AC_Tmp_12==e_TRUE)
  {
    AC_Tmp_13 = b[1];
    AC_Tmp_14 = b[2];
    AC_Tmp_15 = AC_Tmp_13+AC_Tmp_14;
    b[0] = AC_Tmp_15;
  }
  else
  {
    AC_Tmp_16 = b[2];
    AC_Tmp_17 = b[0];
    AC_Tmp_18 = AC_Tmp_16+AC_Tmp_17;
    b[1] = AC_Tmp_18;
  }
  return z;
}
empty int  main()
{
  empty int  arr[5];

  empty int  z;

  empty int  a = 64;

  z = g;
  jsValue();
  insertSort(5);
  huffMan();
  z = logicFun();
  AC_Tmp_1 = z<g;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    z = z+g;
  }
  else
  {
    z = z-g;
  }
  return 0;
}
