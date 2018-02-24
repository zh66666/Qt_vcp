empty int ;

typedef int  adj_node;

typedef int  graph;

extern int  g_Status;

empty int  init_graph_adj_lists(int name);

empty void  make_Vertex(int node_i,int input_key);

empty void  make_adjlist(int ver_node,int adj_i,int edge_wei);

empty int  find_vertex(int now_ver,int pre_ver,int key);

typedef int  binheap_node;

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

static int  binheap[110];

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
    binheap[11*heap_i+3+2+2] = input_vertex;
    binheap[11*heap_i+10] = 268435455;
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
    if(binheap[11*now_compare_i+10]<=binheap[11*left_i+10])
    {
      min_i = now_compare_i;
    }
    else
    {
      min_i = left_i;
    }
    right_i = heap_right(now_compare_i);
    if(right_i<=heapsize&&binheap[11*right_i+10]<binheap[11*min_i+10])
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

  binheap[11*now_compare_i+10] = dec_key;
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
  empty int  temp[11];

  temp[10] = binheap[11*i_1+10];
  temp[0] = binheap[11*i_1];
  temp[0+2] = binheap[11*i_1+2];
  temp[0+1] = binheap[11*i_1+1];
  temp[0+3+1] = binheap[11*i_1+3+1];
  temp[0+3] = binheap[11*i_1+3];
  temp[0+3+2] = binheap[11*i_1+3+2];
  temp[0+3+2+1] = binheap[11*i_1+3+2+1];
  temp[0+3+2+2] = binheap[11*i_1+3+2+2];
  temp[0+3+2+3] = binheap[11*i_1+3+2+3];
  temp[0+3+2+4] = binheap[11*i_1+3+2+4];
  binheap[11*i_1+10] = i_1;
  binheap[11*i_2+10] = i_2;
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
  return binheap[21];
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
