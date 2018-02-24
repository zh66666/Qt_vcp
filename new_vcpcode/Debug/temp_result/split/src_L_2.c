empty int  getValue(int value);

empty int  getMax(int x,int y,int z);

empty int  getMin(int x,int y,int z);

empty int  getMid(int x,int y,int z);

empty int  calMeth();

empty int  getValue(int value)
{
  empty int  ret = 0;

  AC_Tmp_1 = value!=0;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = value*value;
    AC_Tmp_3 = 2*value;
    AC_Tmp_4 = AC_Tmp_3+1;
    ret = AC_Tmp_2+AC_Tmp_4;
  }
  else
  {
    AC_Tmp_5 = ret>>2;
    AC_Tmp_6 = AC_Tmp_5+10;
    ret = AC_Tmp_6/2;
  }
  AC_Tmp_7 = ret+100;
  AC_Tmp_8 = AC_Tmp_7*2;
  AC_Tmp_9 = AC_Tmp_8-19;
  AC_Tmp_10 = AC_Tmp_9/3;
  ret = AC_Tmp_10+11;
  return ret;
}
empty int  getMax(int x,int y,int z)
{
  empty int  ret = 0;

  AC_Tmp_1 = x>y;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = x>z;
    AC_IfSub_3 = AC_Tmp_2-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_2==e_TRUE)
    {
      AC_Tmp_3 = x+y;
      AC_Tmp_4 = AC_Tmp_3+z;
      AC_Tmp_5 = AC_Tmp_4/3;
      ret = AC_Tmp_5+x;
    }
    else
    {
      AC_Tmp_6 = z+x;
      AC_Tmp_7 = AC_Tmp_6+y;
      AC_Tmp_8 = AC_Tmp_7*3;
      AC_Tmp_9 = AC_Tmp_8/2;
      AC_Tmp_10 = z/2;
      ret = AC_Tmp_9+AC_Tmp_10;
    }
  }
  else
  {
    AC_Tmp_11 = y>z;
    AC_IfSub_5 = AC_Tmp_11-e_TRUE;
    AC_IfSub_6 = 0-AC_IfSub_5;
    if(AC_Tmp_11==e_TRUE)
    {
      AC_Tmp_12 = 2*y;
      AC_Tmp_13 = AC_Tmp_12+x;
      AC_Tmp_14 = AC_Tmp_13+z;
      AC_Tmp_15 = x+z;
      AC_Tmp_16 = AC_Tmp_15/2;
      AC_Tmp_17 = AC_Tmp_16*2;
      ret = AC_Tmp_14-AC_Tmp_17;
    }
    else
    {
      AC_Tmp_18 = z/3;
      AC_Tmp_19 = AC_Tmp_18+x;
      AC_Tmp_20 = AC_Tmp_19*3;
      AC_Tmp_21 = x+y;
      AC_Tmp_22 = AC_Tmp_21/3;
      AC_Tmp_23 = AC_Tmp_20-AC_Tmp_22;
      AC_Tmp_24 = y*3;
      AC_Tmp_25 = AC_Tmp_24/9;
      ret = AC_Tmp_23+AC_Tmp_25;
    }
  }
  return ret;
}
empty int  getMin(int x,int y,int z)
{
  empty int  ret = 0;

  AC_Tmp_1 = x<y;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = x<z;
    AC_IfSub_3 = AC_Tmp_2-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_2==e_TRUE)
    {
      ret = x;
    }
    else
    {
      ret = z;
    }
  }
  else
  {
    AC_Tmp_3 = y<z;
    AC_IfSub_5 = AC_Tmp_3-e_TRUE;
    AC_IfSub_6 = 0-AC_IfSub_5;
    if(AC_Tmp_3==e_TRUE)
    {
      ret = y;
    }
    else
    {
      ret = z;
    }
  }
  return ret;
}
empty int  getMid(int x,int y,int z)
{
  empty int  ret = 0;

  AC_Tmp_1 = x<y;
  AC_Tmp_2 = y<z;
  AC_Tmp_3 = AC_Tmp_1&&AC_Tmp_2;
  AC_IfSub_1 = AC_Tmp_3-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_3==e_TRUE)
  {
    ret = y;
  }
  else
  {
    AC_Tmp_4 = y<x;
    AC_Tmp_5 = x<z;
    AC_Tmp_6 = AC_Tmp_4&&AC_Tmp_5;
    AC_IfSub_3 = AC_Tmp_6-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_6==e_TRUE)
    {
      ret = x;
    }
    else
    {
      ret = z;
    }
  }
  return ret;
}
