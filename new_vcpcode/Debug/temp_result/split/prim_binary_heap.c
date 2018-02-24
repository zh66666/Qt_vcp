empty int ;

typedef int  adj_node;

typedef int  graph;

extern int  g_Status;

empty int  init_graph_adj_lists(int name);

empty int  make_Vertex(int node_i,int input_key);

empty int  make_adjlist(int ver_node,int adj_i,int edge_wei);

empty int  find_vertex(int now_ver,int pre_ver,int key);

typedef int  binheap_node;

empty int  binheap_init(int num);

empty int  binheap_make_node(int input_vertex,int heap_i);

empty int  binheap_build_heap();

empty int  min_heap(int start_i);

empty int  binheap_decrease_key(int heap_i,int dec_key);

empty int  binheap_extract_min();

empty int  swap_data(int i_1,int i_2);

empty int  heap_left(int i);

empty int  heap_right(int i);

empty int  heap_get_parent(int i);

empty int  binheap_get_dis_key(int vertex);

empty int  binheap_is_in_heap(int vertex);

empty int  binheap_isempty();

static int  binheap[110];

static int  heapsize;

empty int  binheap_init(int num)
{
  AC_Tmp_1 = num>9;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    heapsize = 9;
  }
  else
  {
    heapsize = num-1;
  }
  return 0;
}
empty int  binheap_make_node(int input_vertex,int heap_i)
{
  AC_Tmp_1 = heap_i<=heapsize;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = heap_i*11;
    AC_Tmp_3 = AC_Tmp_2+7;
    binheap[AC_Tmp_3] = input_vertex;
    AC_Tmp_4 = heap_i*11;
    AC_Tmp_5 = AC_Tmp_4+10;
    binheap[AC_Tmp_5] = 268435455;
  }
  return 0;
}
empty int  binheap_build_heap()
{
  empty int  heap_i = 10;

  AC_Tmp_1 = heapsize-1;
  heap_i = AC_Tmp_1/2;
  AC_Tmp_2 = heap_i>=0;
  AC_WhileSub_1 = AC_Tmp_2-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_2==e_TRUE)
  {
    heap_i = heap_i-1;
    AC_Void_Func_RetVal = min_heap(heap_i);
    AC_Tmp_2 = heap_i>=0;
    AC_WhileSub_1 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return 0;
}
empty int  min_heap(int start_i)
{
  empty int  now_compare_i = 0;

  empty int  left_i = 0,right_i = 0,min_i = 0;

  now_compare_i = start_i;
  AC_Tmp_1 = now_compare_i<=heapsize;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = now_compare_i*11;
    AC_Tmp_3 = AC_Tmp_2+10;
    AC_Tmp_4 = binheap[AC_Tmp_3];
    AC_Tmp_5 = left_i*11;
    AC_Tmp_6 = AC_Tmp_5+10;
    AC_Tmp_7 = binheap[AC_Tmp_6];
    AC_Tmp_8 = AC_Tmp_4<=AC_Tmp_7;
    AC_IfSub_1 = AC_Tmp_8-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_8==e_TRUE)
    {
      min_i = now_compare_i;
    }
    else
    {
      min_i = left_i;
    }
    right_i = heap_right(now_compare_i);
    AC_Tmp_9 = right_i<=heapsize;
    AC_Tmp_10 = right_i*11;
    AC_Tmp_11 = AC_Tmp_10+10;
    AC_Tmp_12 = binheap[AC_Tmp_11];
    AC_Tmp_13 = min_i*11;
    AC_Tmp_14 = AC_Tmp_13+10;
    AC_Tmp_15 = binheap[AC_Tmp_14];
    AC_Tmp_16 = AC_Tmp_12<AC_Tmp_15;
    AC_Tmp_17 = AC_Tmp_9&&AC_Tmp_16;
    AC_IfSub_3 = AC_Tmp_17-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_17==e_TRUE)
    {
      min_i = right_i;
    }
    AC_Tmp_18 = min_i==now_compare_i;
    AC_IfSub_5 = AC_Tmp_18-e_TRUE;
    AC_IfSub_6 = 0-AC_IfSub_5;
    if(AC_Tmp_18==e_TRUE)
    {
      AC_Void_Func_RetVal = swap_data(min_i,now_compare_i);
      now_compare_i = min_i;
    }
    now_compare_i = now_compare_i+1;
    AC_Tmp_1 = now_compare_i<=heapsize;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return 0;
}
empty int  binheap_decrease_key(int heap_i,int dec_key)
{
  empty int  now_compare_i = 0,parent_i = 0;

  AC_Tmp_1 = now_compare_i*11;
  AC_Tmp_2 = AC_Tmp_1+10;
  binheap[AC_Tmp_2] = dec_key;
  now_compare_i = heap_i;
  AC_Tmp_3 = now_compare_i>0;
  AC_WhileSub_1 = AC_Tmp_3-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_3==e_TRUE)
  {
    now_compare_i = now_compare_i-2;
    AC_Tmp_3 = now_compare_i>0;
    AC_WhileSub_1 = AC_Tmp_3-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  return 0;
}
empty int  binheap_extract_min()
{
  return 0;
}
empty int  swap_data(int i_1,int i_2)
{
  empty int  temp[11];

  AC_Tmp_1 = i_1*11;
  AC_Tmp_2 = AC_Tmp_1+10;
  AC_Tmp_3 = binheap[AC_Tmp_2];
  temp[10] = AC_Tmp_3;
  AC_Tmp_4 = i_1*11;
  AC_Tmp_5 = binheap[AC_Tmp_4];
  temp[0] = AC_Tmp_5;
  AC_Tmp_6 = i_1*11;
  AC_Tmp_7 = AC_Tmp_6+2;
  AC_Tmp_8 = binheap[AC_Tmp_7];
  temp[2] = AC_Tmp_8;
  AC_Tmp_9 = i_1*11;
  AC_Tmp_10 = AC_Tmp_9+1;
  AC_Tmp_11 = binheap[AC_Tmp_10];
  temp[1] = AC_Tmp_11;
  AC_Tmp_12 = i_1*11;
  AC_Tmp_13 = AC_Tmp_12+4;
  AC_Tmp_14 = binheap[AC_Tmp_13];
  temp[4] = AC_Tmp_14;
  AC_Tmp_15 = i_1*11;
  AC_Tmp_16 = AC_Tmp_15+3;
  AC_Tmp_17 = binheap[AC_Tmp_16];
  temp[3] = AC_Tmp_17;
  AC_Tmp_18 = i_1*11;
  AC_Tmp_19 = AC_Tmp_18+5;
  AC_Tmp_20 = binheap[AC_Tmp_19];
  temp[5] = AC_Tmp_20;
  AC_Tmp_21 = i_1*11;
  AC_Tmp_22 = AC_Tmp_21+6;
  AC_Tmp_23 = binheap[AC_Tmp_22];
  temp[6] = AC_Tmp_23;
  AC_Tmp_24 = i_1*11;
  AC_Tmp_25 = AC_Tmp_24+7;
  AC_Tmp_26 = binheap[AC_Tmp_25];
  temp[7] = AC_Tmp_26;
  AC_Tmp_27 = i_1*11;
  AC_Tmp_28 = AC_Tmp_27+8;
  AC_Tmp_29 = binheap[AC_Tmp_28];
  temp[8] = AC_Tmp_29;
  AC_Tmp_30 = i_1*11;
  AC_Tmp_31 = AC_Tmp_30+9;
  AC_Tmp_32 = binheap[AC_Tmp_31];
  temp[9] = AC_Tmp_32;
  AC_Tmp_33 = i_1*11;
  AC_Tmp_34 = AC_Tmp_33+10;
  binheap[AC_Tmp_34] = i_1;
  AC_Tmp_35 = i_2*11;
  AC_Tmp_36 = AC_Tmp_35+10;
  binheap[AC_Tmp_36] = i_2;
  return 0;
}
empty int  heap_left(int i)
{
  AC_Tmp_1 = 2*i;
  AC_Tmp_2 = AC_Tmp_1+1;
  return AC_Tmp_2;
}
empty int  heap_right(int i)
{
  AC_Tmp_1 = 2*i;
  AC_Tmp_2 = AC_Tmp_1+2;
  return AC_Tmp_2;
}
empty int  heap_get_parent(int i)
{
  AC_Tmp_1 = i-1;
  AC_Tmp_2 = AC_Tmp_1/2;
  return AC_Tmp_2;
}
empty int  binheap_isempty()
{
  return 6;
}
empty int  binheap_get_dis_key(int vertex)
{
  AC_Tmp_1 = binheap[21];
  return AC_Tmp_1;
}
empty int  binheap_is_in_heap(int vertex)
{
  empty int  iRet = 0;

  AC_Tmp_1 = vertex<10;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    iRet = 1;
  }
  else
  {
    iRet = 0;
  }
  return iRet;
}
