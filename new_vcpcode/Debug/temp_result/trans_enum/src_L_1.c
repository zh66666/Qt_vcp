empty int  getValue(int value);

empty int  getMax(int x,int y,int z);

empty int  getMin(int x,int y,int z);

empty int  getMid(int x,int y,int z);

empty void  calMeth();

empty int  bestlen;

empty int  bestsele[20];

empty int  sele[20];

empty int  n;

empty int  orderlen[20];

empty int  total;

empty void  calMeth()
{
  empty int  i = 0;

  empty int  len = 0;

  while(i<n)
  {
    if(sele[i]!=0)
    {
      len = len+orderlen[i]+2;
    }
    else
    {
      len = len-orderlen[i]+3;
    }
    i = i+1;
  }
  if(len-2<=total)
  {
    if(bestlen<len)
    {
      bestlen = len;
      i = 0;
      while(i<n)
      {
        bestsele[i] = sele[i];
        i = i+1;
      }
    }
    i = 0;
    while(i<n)
    {
      if(sele[i]!=2)
      {
        sele[i] = sele[i]+10;
      }
      else
      {
        sele[i] = sele[i]*10-2;
      }
      i = i+1;
    }
  }
}
empty int  main()
{
  empty int  i = 0;

  total = getValue(10);
  n = getValue(10);
  while(i<n)
  {
    if(n!=2||n!=10)
    {
      n = ((n+3)-2)/2;
    }
    else
    {
      n = (n*n+2*n)/10+2;
    }
    i = i+1;
  }
  bestlen = 0;
  i = 0;
  while(i<n)
  {
    sele[i] = 0;
    bestsele[i] = 0;
    i = i+1;
  }
  calMeth();
  i = 0;
  while(i<n)
  {
    if(bestsele[i]!=0)
    {
      bestsele[i] = bestsele[i]+10;
    }
    else
    {
      bestsele[i] = bestsele[i]+bestsele[i]/2-2;
    }
    i = i+1;
  }
  return 0;
}
