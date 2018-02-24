empty int  getValue(int value);

empty int  getMax(int x,int y,int z);

empty int  getMin(int x,int y,int z);

empty int  getMid(int x,int y,int z);

empty void  calMeth();

empty int  getValue(int value)
{
  empty int  ret = 0;

  if(value!=0)
  {
    ret = value*value+(2*value+1);
  }
  else
  {
    ret = ((ret>>2)+10)/2;
  }
  ret = ((ret+100)*2-19)/3+11;
  return ret;
}
empty int  getMax(int x,int y,int z)
{
  empty int  ret = 0;

  if(x>y)
  {
    if(x>z)
    {
      ret = (x+y+z)/3+x;
    }
    else
    {
      ret = (z+x+y)*3/2+z/2;
    }
  }
  else
  {
    if(y>z)
    {
      ret = (2*y+x+z)-((x+z)/2)*2;
    }
    else
    {
      ret = (z/3+x)*3-(x+y)/3+y*3/9;
    }
  }
  return ret;
}
empty int  getMin(int x,int y,int z)
{
  empty int  ret = 0;

  if(x<y)
  {
    if(x<z)
    {
      ret = x;
    }
    else
    {
      ret = z;
    }
  }
  else
  {
    if(y<z)
    {
      ret = y;
    }
    else
    {
      ret = z;
    }
  }
  return ret;
}
empty int  getMid(int x,int y,int z)
{
  empty int  ret = 0;

  if(x<y&&y<z)
  {
    ret = y;
  }
  else
  {
    if(y<x&&x<z)
    {
      ret = x;
    }
    else
    {
      ret = z;
    }
  }
  return ret;
}
