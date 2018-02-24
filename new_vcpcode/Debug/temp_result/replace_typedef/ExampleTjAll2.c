empty int  fun_max(int a,int b);

empty int  Test_result_a = 0;

empty int  Test_result_b = 1;

empty int  fun_max(int a,int b)
{
  empty int  ret = 0;

  if(a>b)
  {
    ret = a;
  }
  else
  {
    ret = b;
  }
  return ret;
}
empty void  main()
{
  empty int  i = 0;

  empty int  j = 1;

  empty int  m = 2;

  empty int  n = 3;

  if(j>i&&m>n)
  {
    Test_result_a = 10;
    Test_result_b = fun_max(j,m);
  }
  else
  {
    if(j>i||m>n)
    {
      Test_result_a = 100;
    }
    else
    {
      Test_result_b = 2;
    }
  }
}
