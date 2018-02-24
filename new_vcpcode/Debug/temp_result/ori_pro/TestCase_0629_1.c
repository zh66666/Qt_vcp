typedef struct huffnode
{
  empty int  weight;
  empty int  flag;
  empty int  parent;
  empty int  lchild;
  empty int  rchild;
}  huffnode;

typedef struct huffcode
{
  empty int  bits[30];
  empty int  start;
}  huffcode;

empty int  b[5] = { 11,44,33,22,55};

empty int  g = 6;

empty void  jsValue()
{
  empty int  a1 = 1,a2 = 1,a12,sn,k = 2;

  sn = a1+a2;
  while(k<5)
  {
    a12 = a1+2*a2;
    if(sn<100&&sn+a12>=100)
    {
      b[0] = k;
    }
    if(sn<1000&&sn+a12>=1000)
    {
      b[1] = k;
    }
    if(sn<10000&&sn+a12>=10000)
    {
      b[2] = k;
    }
    sn = sn+a12;
    a1 = a2;
    a2 = a12;
    k = k+1;
    ;
  }
}
empty void  insertSort(int n)
{
  empty int  i = 2,j,tempi,tempj,m = 1000;

  while(i<n)
  {
    tempi = i-1;
    tempj = j+1;
    if(b[i]<b[tempi])
    {
      b[0] = b[i];
      j = i-1;
      b[tempj] = b[0];
    }
    i = i+1;
  }
  m = (m>>2)+j;
}
empty void  huffMan()
{
  empty huffnode  huff_node[59];

  empty huffcode  huff_code[30],cd;

  empty int  i = 0,j = 1,m1 = 1000,m2 = 1000,x1 = 0,x2 = 1,n,c,p;

  huff_node[i].weight = 0;
  huff_node[i].parent = 0;
  huff_node[i].flag = 0;
  huff_node[i].lchild = -1;
  huff_node[i].rchild = -1;
  if(huff_node[0].weight<m1&&huff_node[0].flag==0)
  {
    m2 = 1000;
    x2 = x1;
    m1 = huff_node[0].weight;
    x1 = j;
    m2 = (m2<<1)/4+b[0];
  }
}
empty int  logicFun()
{
  empty unsigned int  a = 17;

  empty unsigned int  b = 0;

  empty unsigned int  s = 16;

  empty unsigned int  x = 1;

  empty int  z = 10;

  if(a!=0&&b!=0)
  {
    z = z-1;
  }
  else
  {
    z = z+1;
  }
  if(a!=0&&s!=0)
  {
    g = g<<1;
  }
  else
  {
    g = g>>1;
  }
  if(a!=0&&x!=0)
  {
    x = 17;
  }
  else
  {
    a = 17;
  }
  if(s!=0&&x!=0)
  {
    b[0] = b[1]+b[2];
  }
  else
  {
    b[1] = b[2]+b[0];
  }
  return z;
}
empty void  main()
{
  empty int  arr[5];

  empty int  z;

  empty int  a = 64;

  z = g;
  jsValue();
  insertSort(5);
  huffMan();
  z = logicFun();
  if(z<g)
  {
    z = z+g;
  }
  else
  {
    z = z-g;
  }
}
