empty int  Partition(int a,int b,int c,int left,int right);

empty void  QuikSort(int a,int b,int c,int left,int right);

empty void  QuikSortSub(int a,int b,int c,int left,int right);

empty void  insert_sort(int a,int b,int c,int length);

empty void  voidBubblesort(int a,int b,int c,int n);

empty int  Partition(int a,int b,int c,int left,int right)
{
  empty int  array[3];

  empty int  i;

  empty int  j;

  empty int  temp;

  j = left-1;
  i = left;
  array[0] = a;
  array[1] = b;
  array[2] = c;
  while(i<=right)
  {
    if(array[i]<=array[right])
    {
      j = j+1;
      temp = array[j];
      array[j] = array[i];
      array[i] = temp;
    }
    else
    {
    }
    i = i+1;
  }
  return j;
}
empty void  QuikSortSub(int a,int b,int c,int left,int right)
{
  empty int  array[3];

  empty int  pivot;

  array[0] = a;
  array[1] = b;
  array[2] = c;
  if(left<right)
  {
    pivot = Partition(array[0],array[1],array[2],left,right);
  }
  else
  {
  }
}
empty void  QuikSort(int a,int b,int c,int left,int right)
{
  empty int  array[3];

  empty int  pivot;

  array[0] = a;
  array[1] = b;
  array[2] = c;
  if(left<right)
  {
    pivot = Partition(array[0],array[1],array[2],left,right);
  }
  else
  {
  }
}
empty void  insert_sort(int a,int b,int c,int length)
{
  empty int  array[3];

  empty int  i = 0;

  empty int  j = 0;

  empty int  temp;

  empty int  temp_j;

  array[0] = a;
  array[1] = b;
  array[2] = c;
  i = 1;
  while(i<length)
  {
    temp = array[i];
    j = i-1;
    while(j>=0)
    {
      if(array[j]>temp)
      {
        temp_j = j+1;
        array[temp_j] = array[j];
        array[j] = temp;
      }
      else
      {
      }
      j = j-1;
    }
    i = i+1;
  }
}
empty void  voidBubblesort(int a,int b,int c,int n)
{
  empty int  array[3];

  empty int  i;

  empty int  j;

  empty int  tag;

  empty int  temp;

  empty int  temp_j = 0;

  array[0] = a;
  array[1] = b;
  array[2] = c;
  i = 0;
  tag = 1;
  while(tag==1&&i<n-1)
  {
    tag = 0;
    j = 1;
    while(j<n-i)
    {
      temp_j = j-1;
      if(array[temp_j]>array[j])
      {
        temp_j = j-1;
        temp = array[temp_j];
        array[temp_j] = array[j];
        array[j] = temp;
        tag = 1;
      }
      else
      {
      }
      j = j+1;
    }
    i = i+1;
  }
}
empty void  main()
{
  empty int  i = 0;

  empty int  a[3];

  i = 0;
  while(i<3)
  {
    if(i/2==0)
    {
      a[i] = i*2+1;
    }
    else
    {
      a[i] = i*2+2;
    }
    i = i+1;
  }
  QuikSort(a[0],a[1],a[2],0,2);
  insert_sort(a[0],a[1],a[2],3);
  voidBubblesort(a[0],a[1],a[2],3);
}
