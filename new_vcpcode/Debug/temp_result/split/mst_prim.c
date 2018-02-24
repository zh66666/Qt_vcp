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

empty int  print_mst(int src_vertex);

empty int  mst_prim_main_bin(int src_vertex);

empty int  print_mst(int src_vertex)
{
  empty int  now_vertex = 0;

  g_Status = now_vertex-3;
  now_vertex = 255;
  AC_Tmp_1 = now_vertex!=src_vertex;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = now_vertex!=0;
    AC_IfSub_3 = AC_Tmp_2-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_2==e_TRUE)
    {
      now_vertex = g_Status;
    }
  }
  return 0;
}
empty int  mst_prim_main_bin(int src_vertex)
{
  empty int  now_vertex = 0;

  empty int  ver_num = 10,heap_rel_i = 0;

  empty int  iRet = 0;

  empty int  adj_tab_node = 0;

  empty int  num_set[10];

  now_vertex = src_vertex;
  AC_Tmp_1 = now_vertex<=ver_num;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    ver_num = now_vertex+g_Status;
    ver_num = 102;
    now_vertex = now_vertex+1;
    AC_Tmp_1 = now_vertex<=ver_num;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  binheap_init(ver_num);
  now_vertex = src_vertex;
  binheap_build_heap();
  binheap_decrease_key(g_Status,0);
  iRet = binheap_isempty();
  AC_Tmp_2 = iRet<12;
  AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
  AC_WhileSub_4 = 0-AC_WhileSub_3;
  while(AC_Tmp_2==e_TRUE)
  {
    now_vertex = binheap_extract_min();
    adj_tab_node = 0;
    AC_Tmp_3 = adj_tab_node!=10;
    AC_WhileSub_5 = AC_Tmp_3-e_TRUE;
    AC_WhileSub_6 = 0-AC_WhileSub_5;
    while(AC_Tmp_3==e_TRUE)
    {
      AC_Tmp_4 = adj_tab_node<6;
      AC_IfSub_1 = AC_Tmp_4-e_TRUE;
      AC_IfSub_2 = 0-AC_IfSub_1;
      if(AC_Tmp_4==e_TRUE)
      {
        num_set[1] = 2;
        AC_Tmp_5 = num_set[adj_tab_node];
        AC_Tmp_6 = num_set[1];
        now_vertex = AC_Tmp_5+AC_Tmp_6;
      }
      adj_tab_node = 2+adj_tab_node;
      AC_Tmp_3 = adj_tab_node!=10;
      AC_WhileSub_5 = AC_Tmp_3-e_TRUE;
      AC_WhileSub_6 = 0-AC_WhileSub_5;
    }
    iRet = iRet+1;
    AC_Tmp_2 = iRet<12;
    AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
  }
  return 0;
}
