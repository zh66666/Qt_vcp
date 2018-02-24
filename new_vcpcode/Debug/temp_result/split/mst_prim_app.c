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
