typedef struct tag_BLKPLCE
{
  empty int  x;
  empty int  y;
  empty int  height;
}  BLKPLCE;

empty BLKPLCE  block_list[7] = { { 11,20,31},{ 10,23,32},{ 10,20,33},{ 11,2,3},{ 1,22,3},{ 13,20,3},{ 6,2,10}};

empty int  larger(int i1,int i2);

empty void  swap_block(int i1,int i2);

empty int  cmp(int i1,int i2);

empty int  partiton(int st,int ed);

empty void  sort_block(int start,int end);

empty int  main()
{
  empty int  num_type = 0,type_i = 0,case_i = 0;

  empty int  tri_side[3] = { 0,0,0};

  empty int  put_method[6][2] = { { 0,1},{ 1,0},{ 0,2},{ 2,0},{ 1,2},{ 2,1}};

  empty int  method_code = 0;

  empty int  block_i = 0,block_num = 0,i = 0,input_block_i = 0,placed_i = 0;

  empty int  now_max_height = 0;

  empty int  larger_result = 0;

  while(num_type<5)
  {
    case_i = case_i+1;
    type_i = 0;
    block_i = 0;
    while(type_i<num_type)
    {
      i = 0;
      while(i<7)
      {
        method_code = put_method[i][0];
        block_list[block_i].x = tri_side[method_code];
        block_list[block_i].y = tri_side[method_code];
        block_list[block_i].height = tri_side[method_code];
        i = i+1;
        block_i = block_i+1;
      }
      type_i = type_i+1;
    }
    block_num = block_i;
    sort_block(0,block_num-1);
    now_max_height = -1;
    input_block_i = 1;
    while(input_block_i<block_num)
    {
      now_max_height = 0;
      placed_i = 0;
      while(placed_i<input_block_i)
      {
        larger_result = larger(input_block_i,placed_i);
        if(larger_result==1)
        {
          if(block_list[placed_i].height>now_max_height)
          {
            now_max_height = block_list[placed_i].height;
          }
        }
        placed_i = placed_i+1;
      }
      block_list[input_block_i].height = block_list[input_block_i].height+now_max_height;
      input_block_i = input_block_i+1;
    }
    now_max_height = -1;
    block_i = 0;
    while(block_i<block_num)
    {
      if(block_list[block_i].height>now_max_height)
      {
        now_max_height = block_list[block_i].height;
      }
      block_i = block_i+1;
    }
    num_type = num_type+1;
  }
  return 0;
}
empty int  larger(int i1,int i2)
{
  empty int  iRet = 0;

  if((block_list[i1].x>block_list[i2].x&&block_list[i1].y>block_list[i2].y)||(block_list[i1].x>block_list[i2].y&&block_list[i1].y>block_list[i2].x))
  {
    iRet = 1;
  }
  else
  {
    iRet = 0;
  }
  return iRet;
}
empty void  sort_block(int start,int end)
{
  empty int  split_i = 0;

  if(start<end)
  {
    split_i = partiton(start,end);
  }
}
empty int  partiton(int st,int ed)
{
  empty int  base_i = 0;

  empty int  i = 0,min_i = 0;

  empty int  cmp_result = 0;

  base_i = ed;
  i = st;
  min_i = st-1;
  while(i<=ed-1)
  {
    cmp_result = cmp(i,base_i);
    if(cmp_result==-1)
    {
      swap_block(i,min_i+1);
      min_i = min_i+1;
    }
    i = i+1;
  }
  min_i = min_i+1;
  swap_block(base_i,min_i);
  return min_i;
}
empty int  cmp(int i1,int i2)
{
  empty int  iRet = 0;

  if(block_list[i1].x<block_list[i2].x)
  {
    iRet = -1;
  }
  else  if(block_list[i1].x==block_list[i2].x)
  {
    if(block_list[i1].y<block_list[i2].y)
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

  return iRet;
}
empty void  swap_block(int i1,int i2)
{
  empty int  b = 0;

  empty int  a = 0;

  b = i1;
  a = i2;
}
