typedef int  BLKPLCE;

empty int  block_list[21] = { { 11,20,31},{ 10,23,32},{ 10,20,33},{ 11,2,3},{ 1,22,3},{ 13,20,3},{ 6,2,10}};

empty int  larger(int i1,int i2);

empty int  swap_block(int i1,int i2);

empty int  cmp(int i1,int i2);

empty int  partiton(int st,int ed);

empty int  sort_block(int start,int end);

empty int  main()
{
  empty int  num_type = 0,type_i = 0,case_i = 0;

  empty int  tri_side[3] = { 0,0,0};

  empty int  put_method[12] = { { 0,1},{ 1,0},{ 0,2},{ 2,0},{ 1,2},{ 2,1}};

  empty int  method_code = 0;

  empty int  block_i = 0,block_num = 0,i = 0,input_block_i = 0,placed_i = 0;

  empty int  now_max_height = 0;

  empty int  larger_result = 0;

  AC_Tmp_1 = num_type<5;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    case_i = case_i+1;
    type_i = 0;
    block_i = 0;
    AC_Tmp_2 = type_i<num_type;
    AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
    while(AC_Tmp_2==e_TRUE)
    {
      i = 0;
      AC_Tmp_3 = i<7;
      AC_WhileSub_5 = AC_Tmp_3-e_TRUE;
      AC_WhileSub_6 = 0-AC_WhileSub_5;
      while(AC_Tmp_3==e_TRUE)
      {
        AC_Tmp_4 = i*2;
        method_code = put_method[AC_Tmp_4];
        AC_Tmp_5 = block_i*3;
        AC_Tmp_6 = tri_side[method_code];
        block_list[AC_Tmp_5] = AC_Tmp_6;
        AC_Tmp_7 = block_i*3;
        AC_Tmp_8 = AC_Tmp_7+1;
        AC_Tmp_9 = tri_side[method_code];
        block_list[AC_Tmp_8] = AC_Tmp_9;
        AC_Tmp_10 = block_i*3;
        AC_Tmp_11 = AC_Tmp_10+2;
        AC_Tmp_12 = tri_side[method_code];
        block_list[AC_Tmp_11] = AC_Tmp_12;
        i = i+1;
        block_i = block_i+1;
        AC_Tmp_3 = i<7;
        AC_WhileSub_5 = AC_Tmp_3-e_TRUE;
        AC_WhileSub_6 = 0-AC_WhileSub_5;
      }
      type_i = type_i+1;
      AC_Tmp_2 = type_i<num_type;
      AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
    }
    block_num = block_i;
    AC_Tmp_13 = block_num-1;
    AC_Void_Func_RetVal = sort_block(0,AC_Tmp_13);
    now_max_height = -1;
    input_block_i = 1;
    AC_Tmp_14 = input_block_i<block_num;
    AC_WhileSub_7 = AC_Tmp_14-e_TRUE;
    AC_WhileSub_8 = 0-AC_WhileSub_7;
    while(AC_Tmp_14==e_TRUE)
    {
      now_max_height = 0;
      placed_i = 0;
      AC_Tmp_15 = placed_i<input_block_i;
      AC_WhileSub_9 = AC_Tmp_15-e_TRUE;
      AC_WhileSub_10 = 0-AC_WhileSub_9;
      while(AC_Tmp_15==e_TRUE)
      {
        larger_result = larger(input_block_i,placed_i);
        AC_Tmp_16 = larger_result==1;
        AC_IfSub_1 = AC_Tmp_16-e_TRUE;
        AC_IfSub_2 = 0-AC_IfSub_1;
        if(AC_Tmp_16==e_TRUE)
        {
          AC_Tmp_17 = placed_i*3;
          AC_Tmp_18 = AC_Tmp_17+2;
          AC_Tmp_19 = block_list[AC_Tmp_18];
          AC_Tmp_20 = AC_Tmp_19>now_max_height;
          AC_IfSub_3 = AC_Tmp_20-e_TRUE;
          AC_IfSub_4 = 0-AC_IfSub_3;
          if(AC_Tmp_20==e_TRUE)
          {
            AC_Tmp_21 = placed_i*3;
            AC_Tmp_22 = AC_Tmp_21+2;
            now_max_height = block_list[AC_Tmp_22];
          }
        }
        placed_i = placed_i+1;
        AC_Tmp_15 = placed_i<input_block_i;
        AC_WhileSub_9 = AC_Tmp_15-e_TRUE;
        AC_WhileSub_10 = 0-AC_WhileSub_9;
      }
      AC_Tmp_23 = input_block_i*3;
      AC_Tmp_24 = AC_Tmp_23+2;
      AC_Tmp_25 = input_block_i*3;
      AC_Tmp_26 = AC_Tmp_25+2;
      AC_Tmp_27 = block_list[AC_Tmp_26];
      AC_Tmp_28 = AC_Tmp_27+now_max_height;
      block_list[AC_Tmp_24] = AC_Tmp_28;
      input_block_i = input_block_i+1;
      AC_Tmp_14 = input_block_i<block_num;
      AC_WhileSub_7 = AC_Tmp_14-e_TRUE;
      AC_WhileSub_8 = 0-AC_WhileSub_7;
    }
    now_max_height = -1;
    block_i = 0;
    AC_Tmp_29 = block_i<block_num;
    AC_WhileSub_11 = AC_Tmp_29-e_TRUE;
    AC_WhileSub_12 = 0-AC_WhileSub_11;
    while(AC_Tmp_29==e_TRUE)
    {
      AC_Tmp_30 = block_i*3;
      AC_Tmp_31 = AC_Tmp_30+2;
      AC_Tmp_32 = block_list[AC_Tmp_31];
      AC_Tmp_33 = AC_Tmp_32>now_max_height;
      AC_IfSub_5 = AC_Tmp_33-e_TRUE;
      AC_IfSub_6 = 0-AC_IfSub_5;
      if(AC_Tmp_33==e_TRUE)
      {
        AC_Tmp_34 = block_i*3;
        AC_Tmp_35 = AC_Tmp_34+2;
        now_max_height = block_list[AC_Tmp_35];
      }
      block_i = block_i+1;
      AC_Tmp_29 = block_i<block_num;
      AC_WhileSub_11 = AC_Tmp_29-e_TRUE;
      AC_WhileSub_12 = 0-AC_WhileSub_11;
    }
    num_type = num_type+1;
    AC_Tmp_1 = num_type<5;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return 0;
}
empty int  larger(int i1,int i2)
{
  empty int  iRet = 0;

  AC_Tmp_1 = i1*3;
  AC_Tmp_2 = block_list[AC_Tmp_1];
  AC_Tmp_3 = i2*3;
  AC_Tmp_4 = block_list[AC_Tmp_3];
  AC_Tmp_5 = AC_Tmp_2>AC_Tmp_4;
  AC_Tmp_6 = i1*3;
  AC_Tmp_7 = AC_Tmp_6+1;
  AC_Tmp_8 = block_list[AC_Tmp_7];
  AC_Tmp_9 = i2*3;
  AC_Tmp_10 = AC_Tmp_9+1;
  AC_Tmp_11 = block_list[AC_Tmp_10];
  AC_Tmp_12 = AC_Tmp_8>AC_Tmp_11;
  AC_Tmp_13 = AC_Tmp_5&&AC_Tmp_12;
  AC_Tmp_14 = i1*3;
  AC_Tmp_15 = block_list[AC_Tmp_14];
  AC_Tmp_16 = i2*3;
  AC_Tmp_17 = AC_Tmp_16+1;
  AC_Tmp_18 = block_list[AC_Tmp_17];
  AC_Tmp_19 = AC_Tmp_15>AC_Tmp_18;
  AC_Tmp_20 = i1*3;
  AC_Tmp_21 = AC_Tmp_20+1;
  AC_Tmp_22 = block_list[AC_Tmp_21];
  AC_Tmp_23 = i2*3;
  AC_Tmp_24 = block_list[AC_Tmp_23];
  AC_Tmp_25 = AC_Tmp_22>AC_Tmp_24;
  AC_Tmp_26 = AC_Tmp_19&&AC_Tmp_25;
  AC_Tmp_27 = AC_Tmp_13||AC_Tmp_26;
  AC_IfSub_1 = AC_Tmp_27-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_27==e_TRUE)
  {
    iRet = 1;
  }
  else
  {
    iRet = 0;
  }
  return iRet;
}
empty int  sort_block(int start,int end)
{
  empty int  split_i = 0;

  AC_Tmp_1 = start<end;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    split_i = partiton(start,end);
  }
  return 0;
}
empty int  partiton(int st,int ed)
{
  empty int  base_i = 0;

  empty int  i = 0,min_i = 0;

  empty int  cmp_result = 0;

  base_i = ed;
  i = st;
  min_i = st-1;
  AC_Tmp_1 = ed-1;
  AC_Tmp_2 = i<=AC_Tmp_1;
  AC_WhileSub_1 = AC_Tmp_2-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_2==e_TRUE)
  {
    cmp_result = cmp(i,base_i);
    AC_Tmp_3 = cmp_result==-1;
    AC_IfSub_1 = AC_Tmp_3-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_3==e_TRUE)
    {
      AC_Tmp_4 = min_i+1;
      AC_Void_Func_RetVal = swap_block(i,AC_Tmp_4);
      min_i = min_i+1;
    }
    i = i+1;
    AC_Tmp_1 = ed-1;
    AC_Tmp_2 = i<=AC_Tmp_1;
    AC_WhileSub_1 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  min_i = min_i+1;
  swap_block(base_i,min_i);
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return min_i;
}
empty int  cmp(int i1,int i2)
{
  empty int  iRet = 0;

  AC_Tmp_1 = i1*3;
  AC_Tmp_2 = block_list[AC_Tmp_1];
  AC_Tmp_3 = i2*3;
  AC_Tmp_4 = block_list[AC_Tmp_3];
  AC_Tmp_5 = AC_Tmp_2<AC_Tmp_4;
  AC_IfSub_1 = AC_Tmp_5-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_5==e_TRUE)
  {
    iRet = -1;
  }
  else
  {
    AC_Tmp_6 = i1*3;
    AC_Tmp_7 = block_list[AC_Tmp_6];
    AC_Tmp_8 = i2*3;
    AC_Tmp_9 = block_list[AC_Tmp_8];
    AC_Tmp_10 = AC_Tmp_7==AC_Tmp_9;
    AC_IfSub_3 = AC_Tmp_10-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_10==e_TRUE)
    {
      AC_Tmp_11 = i1*3;
      AC_Tmp_12 = AC_Tmp_11+1;
      AC_Tmp_13 = block_list[AC_Tmp_12];
      AC_Tmp_14 = i2*3;
      AC_Tmp_15 = AC_Tmp_14+1;
      AC_Tmp_16 = block_list[AC_Tmp_15];
      AC_Tmp_17 = AC_Tmp_13<AC_Tmp_16;
      AC_IfSub_5 = AC_Tmp_17-e_TRUE;
      AC_IfSub_6 = 0-AC_IfSub_5;
      if(AC_Tmp_17==e_TRUE)
      {
        iRet = -1;
      }
      else
      {
        iRet = 1;
      }
    }
    else
    {
      iRet = 1;
    }
  }
  return iRet;
}
empty int  swap_block(int i1,int i2)
{
  empty int  b = 0;

  empty int  a = 0;

  b = i1;
  a = i2;
  return 0;
}
