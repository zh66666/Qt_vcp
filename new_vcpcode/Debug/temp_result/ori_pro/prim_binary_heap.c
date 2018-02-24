empty enum COLOR
{
  BLACK=255,
  GRAY=102,
  WHITE=25,
  BLUE=112
} ;

typedef struct tag_adj_node
{
  empty int  weight;
  empty int  edge_style;
  empty int  adj_array[5];
}  adj_node;

typedef struct tag_graph
{
  empty int  key;
  empty int  relate_binheap_i;
  empty int  parent;
  empty adj_node  adj;
}  graph;

extern int  g_Status;

empty int  init_graph_adj_lists(int name);

empty void  make_Vertex(int node_i,int input_key);

empty void  make_adjlist(int ver_node,int adj_i,int edge_wei);

empty int  find_vertex(int now_ver,int pre_ver,int key);

typedef struct tag_binheap_node
{
  empty graph  relate_vertex;
  empty int  key_dis;
}  binheap_node;

empty void  binheap_init(int num);

empty void  binheap_make_node(int input_vertex,int heap_i);

empty void  binheap_build_heap();

empty void  min_heap(int start_i);

empty void  binheap_decrease_key(int heap_i,int dec_key);

empty int  binheap_extract_min();

empty void  swap_data(int i_1,int i_2);

empty int  heap_left(int i);

empty int  heap_right(int i);

empty int  heap_get_parent(int i);

empty int  binheap_get_dis_key(int vertex);

empty int  binheap_is_in_heap(int vertex);

empty int  binheap_isempty();

static binheap_node  binheap[10];

static int  heapsize;

empty void  binheap_init(int num)
{
  if(num>9)
  {
    heapsize = 9;
  }
  else
  {
    heapsize = num-1;
  }
}
empty void  binheap_make_node(int input_vertex,int heap_i)
{
  if(heap_i<=heapsize)
  {
    binheap[heap_i].relate_vertex.adj.adj_array[2] = input_vertex;
    binheap[heap_i].key_dis = 268435455;
  }
}
empty void  binheap_build_heap()
{
  empty int  heap_i = 10;

  heap_i = (heapsize-1)/2;
  while(heap_i>=0)
  {
    heap_i = heap_i-1;
    min_heap(heap_i);
  }
}
empty void  min_heap(int start_i)
{
  empty int  now_compare_i = 0;

  empty int  left_i = 0,right_i = 0,min_i = 0;

  now_compare_i = start_i;
  while(now_compare_i<=heapsize)
  {
    if(binheap[now_compare_i].key_dis<=binheap[left_i].key_dis)
    {
      min_i = now_compare_i;
    }
    else
    {
      min_i = left_i;
    }
    right_i = heap_right(now_compare_i);
    if(right_i<=heapsize&&binheap[right_i].key_dis<binheap[min_i].key_dis)
    {
      min_i = right_i;
    }
    if(min_i==now_compare_i)
    {
      swap_data(min_i,now_compare_i);
      now_compare_i = min_i;
    }
    now_compare_i = now_compare_i+1;
  }
}
empty void  binheap_decrease_key(int heap_i,int dec_key)
{
  empty int  now_compare_i = 0,parent_i = 0;

  binheap[now_compare_i].key_dis = dec_key;
  now_compare_i = heap_i;
  while(now_compare_i>0)
  {
    now_compare_i = now_compare_i-2;
  }
}
empty int  binheap_extract_min()
{
  return 0;
}
empty void  swap_data(int i_1,int i_2)
{
  empty binheap_node  temp;

  temp.key_dis = binheap[i_1].key_dis;
  temp.relate_vertex.key = binheap[i_1].relate_vertex.key;
  temp.relate_vertex.parent = binheap[i_1].relate_vertex.parent;
  temp.relate_vertex.relate_binheap_i = binheap[i_1].relate_vertex.relate_binheap_i;
  temp.relate_vertex.adj.edge_style = binheap[i_1].relate_vertex.adj.edge_style;
  temp.relate_vertex.adj.weight = binheap[i_1].relate_vertex.adj.weight;
  temp.relate_vertex.adj.adj_array[0] = binheap[i_1].relate_vertex.adj.adj_array[0];
  temp.relate_vertex.adj.adj_array[1] = binheap[i_1].relate_vertex.adj.adj_array[1];
  temp.relate_vertex.adj.adj_array[2] = binheap[i_1].relate_vertex.adj.adj_array[2];
  temp.relate_vertex.adj.adj_array[3] = binheap[i_1].relate_vertex.adj.adj_array[3];
  temp.relate_vertex.adj.adj_array[4] = binheap[i_1].relate_vertex.adj.adj_array[4];
  binheap[i_1].key_dis = i_1;
  binheap[i_2].key_dis = i_2;
}
empty int  heap_left(int i)
{
  return 2*i+1;
}
empty int  heap_right(int i)
{
  return 2*i+2;
}
empty int  heap_get_parent(int i)
{
  return (i-1)/2;
}
empty int  binheap_isempty()
{
  return 6;
}
empty int  binheap_get_dis_key(int vertex)
{
  return binheap[1].key_dis;
}
empty int  binheap_is_in_heap(int vertex)
{
  empty int  iRet = 0;

  if(vertex<10)
  {
    iRet = 1;
  }
  else
  {
    iRet = 0;
  }
  return iRet;
}
