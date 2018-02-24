empty void  func1();

empty int  func2();

empty void  func3(int a);

empty void  func4();

empty void  matrix_multi();

empty void  prime_5000();

empty void  loop_ten();

empty void  graph();

empty void  line(int x1,int y1,int x2,int y2);

empty void  initgraph(int indriver,int inmode);

empty void  setcolor(int incolor);

empty int  matrix_1[2500];

empty int  matrix_2[2500];

empty int  array[10];

empty int  final[2500];

empty void  matrix_multi()
{
  empty int  row = 0;

  empty int  col = 0;

  empty int  i = 0;

  empty int  cur_sum = 0;

  while(row<50)
  {
    col = 0;
    while(col<50)
    {
      cur_sum = 0;
      i = 0;
      while(i<50)
      {
        cur_sum = cur_sum+matrix_1[50*row+i]*matrix_2[50*i+col];
        i = i+1;
      }
      final[50*row+col] = cur_sum;
      col = col+1;
    }
    row = row+1;
  }
}
empty void  prime_5000()
{
  empty int  prime_cnt = 0;

  empty int  num = 3;

  empty int  divior = 0;

  empty int  mod = 0;

  while(num<=5000)
  {
    divior = num-1;
    while(divior>=2)
    {
      mod = num/97;
      mod = num-mod*97;
      if(mod==0)
      {
        divior = 2;
      }
      divior = divior-1;
    }
    if(divior==1)
    {
      prime_cnt = prime_cnt+1;
    }
    num = num+1;
  }
}
empty void  loop_ten()
{
  empty int  i = 0;

  empty int  a = 0;

  while(i<10)
  {
    a = array[i];
    i = i+1;
  }
}
empty void  graph()
{
  empty int  driver,mode,error;

  empty int  x1,y1;

  empty int  x2,y2;

  empty int  dx1,dy1,dx2,dy2,i = 1;

  empty int  count = 0;

  empty int  color = 0;

  driver = 3822;
  mode = 65535;
  initgraph(driver,mode);
  x1 = 10;
  x2 = 10;
  y1 = 10;
  y2 = y1;
  dy1 = 2;
  dy2 = 3;
  dx1 = dy1;
  dx2 = dy2;
  while(i<100)
  {
    line(x1,y1,x2,y2);
    x1 = x1+dx1;
    y1 = y1+dy1;
    x2 = x2+dx2;
    y2 = y2+dy2;
    if(x1<=0||x1>=639)
    {
      dx1 = 0-dx1;
    }
    if(y1<=0||y1>=479)
    {
      dy1 = 0-dy1;
    }
    if(x2<=0||x2>=639)
    {
      dx2 = 0-dx2;
    }
    if(y2<=0||y2>=479)
    {
      dy2 = 0-dy2;
    }
    count = count+1;
    if(count>400)
    {
      setcolor(color);
    }
    i = i+1;
  }
}
empty void  line(int x1,int y1,int x2,int y2)
{
  empty int  a[4];

  a[0] = x1;
  a[1] = x2;
  a[2] = y1;
  a[3] = y2;
}
empty void  initgraph(int indriver,int inmode)
{
  ;
}
empty void  setcolor(int incolor)
{
  empty int  a = 0;

  a = incolor;
}
empty int  main()
{
  func1();
  func2();
  func3(10);
  func4();
  matrix_multi();
  prime_5000();
  loop_ten();
  graph();
  return 0;
}
