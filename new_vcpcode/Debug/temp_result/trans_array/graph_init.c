empty int ;

typedef int  adj_node;

typedef int  graph;

extern int  g_Status;

empty int  init_graph_adj_lists(int name);

empty void  make_Vertex(int node_i,int input_key);

empty void  make_adjlist(int ver_node,int adj_i,int edge_wei);

empty int  find_vertex(int now_ver,int pre_ver,int key);

empty int  init_graph_adj_lists(int name)
{
  empty int  input_buffer[30];

  empty int  start = 0,end = 0,edge_w = 0;

  empty int  now_Vertex = 0,adj_vertex = 1;

  end = name;
  while(start<end)
  {
    if(start==0)
    {
      make_Vertex(edge_w,start);
    }
    now_Vertex = find_vertex(start,end,name);
    if(now_Vertex==1)
    {
      make_Vertex(end,start);
    }
    make_adjlist(now_Vertex,adj_vertex,edge_w);
    start = start+1;
  }
  g_Status = 5;
  return g_Status;
}
empty void  make_Vertex(int node_i,int input_key)
{
  node_i = input_key+1;
}
empty void  make_adjlist(int ver_node,int adj_i,int edge_wei)
{
  ver_node = 1;
  adj_i = 10;
  edge_wei = 0;
  while(ver_node<adj_i)
  {
    edge_wei = edge_wei+1;
    ver_node = ver_node+1;
  }
  g_Status = 1;
}
empty int  find_vertex(int now_ver,int pre_ver,int key)
{
  empty int  array[10];

  pre_ver = 10;
  now_ver = 0;
  while(now_ver<pre_ver)
  {
    now_ver = now_ver+2;
    array[now_ver] = pre_ver;
  }
  return now_ver;
}
