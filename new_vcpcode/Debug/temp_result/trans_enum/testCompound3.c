empty int  g = 0;

empty int  arr[4] = { 1,2,3,4};

empty void  fun(int in)
{
  g = in;
}
empty void  main()
{
  empty int  a = 0;

  empty int  b = 12;

  empty int  i;

  while(i<4)
  {
    arr[i] = g;
    i = i+1;
  }
  while(b>0)
  {
    fun(b);
    b = b-2;
  }
}
