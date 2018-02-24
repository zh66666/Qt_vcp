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

empty void  func1()
{
  empty int  i = 1,j = 1,k = 1;

  empty int  x = 2,y = 1,z = 9,t = 0;

  while(i<5)
  {
    j = 1;
    while(j<5)
    {
      k = 1;
      while(k<5)
      {
        if(i!=k&&i!=j&&j!=k)
        {
          ;
        }
        k = k+1;
      }
      j = j+1;
    }
    i = i+1;
  }
  if(x>y)
  {
    t = x;
    x = y;
    y = t;
  }
  if(x>z)
  {
    t = z;
    z = x;
    x = t;
  }
  if(y>z)
  {
    t = y;
    y = z;
    z = t;
  }
}
empty int  func2()
{
  empty int  i,j,result,n,k;

  i = 1;
  while(i<10)
  {
    j = 1;
    while(j<10)
    {
      result = i*j;
      j = j+1;
    }
    i = i+1;
  }
  n = 100;
  while(n<1000)
  {
    i = n/100;
    j = n/10;
    k = n;
    if(i*100+j*10+k==i*i*i+j*j*j+k*k*k)
    {
      result = n;
    }
    n = n+1;
  }
  return result;
}
empty void  func3(int a)
{
  empty int  i,aa[4],t,temp = 0;

  aa[0] = a;
  aa[1] = a/10;
  aa[2] = a/100;
  aa[3] = a/1000;
  i = 0;
  while(i<=3)
  {
    aa[i] = aa[i]+5;
    aa[i] = aa[i]/10;
    i = i+1;
    func2();
  }
  i = 0;
  while(i<=3/2)
  {
    t = aa[i];
    temp = 3-i;
    aa[i] = aa[temp];
    aa[temp] = t;
    i = i+1;
  }
}
empty void  func4()
{
  empty int  i = 0,k = 0,m = 0,n = 0,p[100];

  i = 0;
  while(i<100)
  {
    p[i] = i+1;
    k = 0;
    m = 0;
    i = i+1;
  }
  m = 0;
  while(m<100)
  {
    if(p[i]!=0)
    {
      k = k+1;
    }
    if(k==3)
    {
      p[i] = 0;
      k = 0;
      m = m+1;
    }
    i = i+1;
    if(i==n)
    {
      i = 0;
    }
    m = m+1;
  }
}
