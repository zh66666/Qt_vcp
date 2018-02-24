empty int ;

typedef int  adj_node;

typedef int  graph;

extern int  g_Status;

empty int  init_graph_adj_lists(int name);

empty int  make_Vertex(int node_i,int input_key);

empty int  make_adjlist(int ver_node,int adj_i,int edge_wei);

empty int  find_vertex(int now_ver,int pre_ver,int key);

empty int  init_graph_adj_lists(int name)
{
  empty int  input_buffer[30];

  empty int  start = 0,end = 0,edge_w = 0;

  empty int  now_Vertex = 0,adj_vertex = 1;

  end = name;
  AC_Tmp_1 = start<end;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = start==0;
    AC_IfSub_1 = AC_Tmp_2-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_2==e_TRUE)
    {
      AC_Void_Func_RetVal = make_Vertex(edge_w,start);
    }
    now_Vertex = find_vertex(start,end,name);
    AC_Tmp_3 = now_Vertex==1;
    AC_IfSub_3 = AC_Tmp_3-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_3==e_TRUE)
    {
      AC_Void_Func_RetVal = make_Vertex(end,start);
    }
    AC_Void_Func_RetVal = make_adjlist(now_Vertex,adj_vertex,edge_w);
    start = start+1;
    AC_Tmp_1 = start<end;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  g_Status = 5;
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return g_Status;
}
empty int  make_Vertex(int node_i,int input_key)
{
  node_i = input_key+1;
  return 0;
}
empty int  make_adjlist(int ver_node,int adj_i,int edge_wei)
{
  ver_node = 1;
  adj_i = 10;
  edge_wei = 0;
  AC_Tmp_1 = ver_node<adj_i;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    edge_wei = edge_wei+1;
    ver_node = ver_node+1;
    AC_Tmp_1 = ver_node<adj_i;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  g_Status = 1;
  return 0;
}
empty int  find_vertex(int now_ver,int pre_ver,int key)
{
  empty int  array[10];

  pre_ver = 10;
  now_ver = 0;
  AC_Tmp_1 = now_ver<pre_ver;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    now_ver = now_ver+2;
    array[now_ver] = pre_ver;
    AC_Tmp_1 = now_ver<pre_ver;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  return now_ver;
}
