empty int  Test_V = 5;

empty void  main()
{
  empty int  x = 2;

  empty int  y = 1;

  empty int  z = 3;

  empty int  arr[10];

  arr[0] = y;
  z = arr[0];
  arr[1] = Test_V;
  x = arr[y];
  arr[2] = 9;
  if(arr[1]<=24)
  {
    Test_V = (Test_V+arr[2])/3;
    z = arr[2]<<3;
  }
  else
  {
    arr[3] = arr[1]*arr[2];
  }
  return ;
}
