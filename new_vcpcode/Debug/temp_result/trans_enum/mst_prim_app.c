empty int ;

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
  empty struct tag_adj_node
  {
    empty int  weight;
    empty int  edge_style;
    empty int  adj_array[5];
  }  adj;
}  graph;

extern int  g_Status;

empty int  init_graph_adj_lists(int name);

empty void  make_Vertex(int node_i,int input_key);

empty void  make_adjlist(int ver_node,int adj_i,int edge_wei);

empty int  find_vertex(int now_ver,int pre_ver,int key);

typedef struct tag_binheap_node
{
  empty struct tag_graph
  {
    empty int  key;
    empty int  relate_binheap_i;
    empty int  parent;
    empty struct tag_adj_node
    {
      empty int  weight;
      empty int  edge_style;
      empty int  adj_array[5];
    }  adj;
  }  relate_vertex;
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

empty void  print_mst(int src_vertex);

empty void  mst_prim_main_bin(int src_vertex);

empty int  g_Status = 0;

empty int  main()
{
  empty int  src_vertex = 1;

  src_vertex = init_graph_adj_lists(6);
  mst_prim_main_bin(src_vertex);
  print_mst(src_vertex);
  print_mst(src_vertex);
  return 0;
}
