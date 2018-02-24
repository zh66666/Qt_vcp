empty int  getValue(int value);

empty int  getMax(int x,int y,int z);

empty int  getMin(int x,int y,int z);

empty int  getMid(int x,int y,int z);

empty int  calMeth();

empty int  bestlen;

empty int  bestsele[20];

empty int  sele[20];

empty int  n;

empty int  orderlen[20];

empty int  total;

empty int  calMeth()
{
  empty int  i = 0;

  empty int  len = 0;

  AC_Tmp_1 = i<n;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = sele[i];
    AC_Tmp_3 = AC_Tmp_2!=0;
    AC_IfSub_1 = AC_Tmp_3-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_3==e_TRUE)
    {
      AC_Tmp_4 = orderlen[i];
      AC_Tmp_5 = len+AC_Tmp_4;
      len = AC_Tmp_5+2;
    }
    else
    {
      AC_Tmp_6 = orderlen[i];
      AC_Tmp_7 = len-AC_Tmp_6;
      len = AC_Tmp_7+3;
    }
    i = i+1;
    AC_Tmp_1 = i<n;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  AC_Tmp_8 = len-2;
  AC_Tmp_9 = AC_Tmp_8<=total;
  AC_IfSub_3 = AC_Tmp_9-e_TRUE;
  AC_IfSub_4 = 0-AC_IfSub_3;
  if(AC_Tmp_9==e_TRUE)
  {
    AC_Tmp_10 = bestlen<len;
    AC_IfSub_5 = AC_Tmp_10-e_TRUE;
    AC_IfSub_6 = 0-AC_IfSub_5;
    if(AC_Tmp_10==e_TRUE)
    {
      bestlen = len;
      i = 0;
      AC_Tmp_11 = i<n;
      AC_WhileSub_3 = AC_Tmp_11-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
      while(AC_Tmp_11==e_TRUE)
      {
        AC_Tmp_12 = sele[i];
        bestsele[i] = AC_Tmp_12;
        i = i+1;
        AC_Tmp_11 = i<n;
        AC_WhileSub_3 = AC_Tmp_11-e_TRUE;
        AC_WhileSub_4 = 0-AC_WhileSub_3;
      }
    }
    i = 0;
    AC_Tmp_13 = i<n;
    AC_WhileSub_5 = AC_Tmp_13-e_TRUE;
    AC_WhileSub_6 = 0-AC_WhileSub_5;
    while(AC_Tmp_13==e_TRUE)
    {
      AC_Tmp_14 = sele[i];
      AC_Tmp_15 = AC_Tmp_14!=2;
      AC_IfSub_7 = AC_Tmp_15-e_TRUE;
      AC_IfSub_8 = 0-AC_IfSub_7;
      if(AC_Tmp_15==e_TRUE)
      {
        AC_Tmp_16 = sele[i];
        AC_Tmp_17 = AC_Tmp_16+10;
        sele[i] = AC_Tmp_17;
      }
      else
      {
        AC_Tmp_18 = sele[i];
        AC_Tmp_19 = AC_Tmp_18*10;
        AC_Tmp_20 = AC_Tmp_19-2;
        sele[i] = AC_Tmp_20;
      }
      i = i+1;
      AC_Tmp_13 = i<n;
      AC_WhileSub_5 = AC_Tmp_13-e_TRUE;
      AC_WhileSub_6 = 0-AC_WhileSub_5;
    }
  }
  return 0;
}
empty int  main()
{
  empty int  i = 0;

  total = getValue(10);
  n = getValue(10);
  AC_Tmp_1 = i<n;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = n!=2;
    AC_Tmp_3 = n!=10;
    AC_Tmp_4 = AC_Tmp_2||AC_Tmp_3;
    AC_IfSub_1 = AC_Tmp_4-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_4==e_TRUE)
    {
      AC_Tmp_5 = n+3;
      AC_Tmp_6 = AC_Tmp_5-2;
      n = AC_Tmp_6/2;
    }
    else
    {
      AC_Tmp_7 = n*n;
      AC_Tmp_8 = 2*n;
      AC_Tmp_9 = AC_Tmp_7+AC_Tmp_8;
      AC_Tmp_10 = AC_Tmp_9/10;
      n = AC_Tmp_10+2;
    }
    i = i+1;
    AC_Tmp_1 = i<n;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  bestlen = 0;
  i = 0;
  AC_Tmp_11 = i<n;
  AC_WhileSub_3 = AC_Tmp_11-e_TRUE;
  AC_WhileSub_4 = 0-AC_WhileSub_3;
  while(AC_Tmp_11==e_TRUE)
  {
    sele[i] = 0;
    bestsele[i] = 0;
    i = i+1;
    AC_Tmp_11 = i<n;
    AC_WhileSub_3 = AC_Tmp_11-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
  }
  calMeth();
  i = 0;
  AC_Tmp_12 = i<n;
  AC_WhileSub_5 = AC_Tmp_12-e_TRUE;
  AC_WhileSub_6 = 0-AC_WhileSub_5;
  while(AC_Tmp_12==e_TRUE)
  {
    AC_Tmp_13 = bestsele[i];
    AC_Tmp_14 = AC_Tmp_13!=0;
    AC_IfSub_3 = AC_Tmp_14-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_14==e_TRUE)
    {
      AC_Tmp_15 = bestsele[i];
      AC_Tmp_16 = AC_Tmp_15+10;
      bestsele[i] = AC_Tmp_16;
    }
    else
    {
      AC_Tmp_17 = bestsele[i];
      AC_Tmp_18 = bestsele[i];
      AC_Tmp_19 = AC_Tmp_18/2;
      AC_Tmp_20 = AC_Tmp_17+AC_Tmp_19;
      AC_Tmp_21 = AC_Tmp_20-2;
      bestsele[i] = AC_Tmp_21;
    }
    i = i+1;
    AC_Tmp_12 = i<n;
    AC_WhileSub_5 = AC_Tmp_12-e_TRUE;
    AC_WhileSub_6 = 0-AC_WhileSub_5;
  }
  return 0;
}
