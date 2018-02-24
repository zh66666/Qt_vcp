empty int  Partition(int a,int b,int c,int left,int right);

empty int  QuikSort(int a,int b,int c,int left,int right);

empty int  QuikSortSub(int a,int b,int c,int left,int right);

empty int  insert_sort(int a,int b,int c,int length);

empty int  voidBubblesort(int a,int b,int c,int n);

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
  AC_Tmp_1 = i<=right;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = array[i];
    AC_Tmp_3 = array[right];
    AC_Tmp_4 = AC_Tmp_2<=AC_Tmp_3;
    AC_IfSub_1 = AC_Tmp_4-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_4==e_TRUE)
    {
      j = j+1;
      temp = array[j];
      AC_Tmp_5 = array[i];
      array[j] = AC_Tmp_5;
      array[i] = temp;
    }
    else
    {
    }
    i = i+1;
    AC_Tmp_1 = i<=right;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  return j;
}
empty int  QuikSortSub(int a,int b,int c,int left,int right)
{
  empty int  array[3];

  empty int  pivot;

  array[0] = a;
  array[1] = b;
  array[2] = c;
  AC_Tmp_1 = left<right;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = array[0];
    AC_Tmp_3 = array[1];
    AC_Tmp_4 = array[2];
    pivot = Partition(AC_Tmp_2,AC_Tmp_3,AC_Tmp_4,left,right);
  }
  else
  {
  }
  return 0;
}
empty int  QuikSort(int a,int b,int c,int left,int right)
{
  empty int  array[3];

  empty int  pivot;

  array[0] = a;
  array[1] = b;
  array[2] = c;
  AC_Tmp_1 = left<right;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = array[0];
    AC_Tmp_3 = array[1];
    AC_Tmp_4 = array[2];
    pivot = Partition(AC_Tmp_2,AC_Tmp_3,AC_Tmp_4,left,right);
  }
  else
  {
  }
  return 0;
}
empty int  insert_sort(int a,int b,int c,int length)
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
  AC_Tmp_1 = i<length;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    temp = array[i];
    j = i-1;
    AC_Tmp_2 = j>=0;
    AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
    while(AC_Tmp_2==e_TRUE)
    {
      AC_Tmp_3 = array[j];
      AC_Tmp_4 = AC_Tmp_3>temp;
      AC_IfSub_1 = AC_Tmp_4-e_TRUE;
      AC_IfSub_2 = 0-AC_IfSub_1;
      if(AC_Tmp_4==e_TRUE)
      {
        temp_j = j+1;
        AC_Tmp_5 = array[j];
        array[temp_j] = AC_Tmp_5;
        array[j] = temp;
      }
      else
      {
      }
      j = j-1;
      AC_Tmp_2 = j>=0;
      AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
    }
    i = i+1;
    AC_Tmp_1 = i<length;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  return 0;
}
empty int  voidBubblesort(int a,int b,int c,int n)
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
  AC_Tmp_1 = tag==1;
  AC_Tmp_2 = n-1;
  AC_Tmp_3 = i<AC_Tmp_2;
  AC_Tmp_4 = AC_Tmp_1&&AC_Tmp_3;
  AC_WhileSub_1 = AC_Tmp_4-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_4==e_TRUE)
  {
    tag = 0;
    j = 1;
    AC_Tmp_5 = n-i;
    AC_Tmp_6 = j<AC_Tmp_5;
    AC_WhileSub_3 = AC_Tmp_6-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
    while(AC_Tmp_6==e_TRUE)
    {
      temp_j = j-1;
      AC_Tmp_7 = array[temp_j];
      AC_Tmp_8 = array[j];
      AC_Tmp_9 = AC_Tmp_7>AC_Tmp_8;
      AC_IfSub_1 = AC_Tmp_9-e_TRUE;
      AC_IfSub_2 = 0-AC_IfSub_1;
      if(AC_Tmp_9==e_TRUE)
      {
        temp_j = j-1;
        temp = array[temp_j];
        AC_Tmp_10 = array[j];
        array[temp_j] = AC_Tmp_10;
        array[j] = temp;
        tag = 1;
      }
      else
      {
      }
      j = j+1;
      AC_Tmp_5 = n-i;
      AC_Tmp_6 = j<AC_Tmp_5;
      AC_WhileSub_3 = AC_Tmp_6-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
    }
    i = i+1;
    AC_Tmp_1 = tag==1;
    AC_Tmp_2 = n-1;
    AC_Tmp_3 = i<AC_Tmp_2;
    AC_Tmp_4 = AC_Tmp_1&&AC_Tmp_3;
    AC_WhileSub_1 = AC_Tmp_4-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  return 0;
}
empty int  main()
{
  empty int  i = 0;

  empty int  a[3];

  i = 0;
  AC_Tmp_1 = i<3;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = i/2;
    AC_Tmp_3 = AC_Tmp_2==0;
    AC_IfSub_1 = AC_Tmp_3-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_3==e_TRUE)
    {
      AC_Tmp_4 = i*2;
      AC_Tmp_5 = AC_Tmp_4+1;
      a[i] = AC_Tmp_5;
    }
    else
    {
      AC_Tmp_6 = i*2;
      AC_Tmp_7 = AC_Tmp_6+2;
      a[i] = AC_Tmp_7;
    }
    i = i+1;
    AC_Tmp_1 = i<3;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  AC_Tmp_8 = a[0];
  AC_Tmp_9 = a[1];
  AC_Tmp_10 = a[2];
  QuikSort(AC_Tmp_8,AC_Tmp_9,AC_Tmp_10,0,2);
  AC_Tmp_11 = a[0];
  AC_Tmp_12 = a[1];
  AC_Tmp_13 = a[2];
  insert_sort(AC_Tmp_11,AC_Tmp_12,AC_Tmp_13,3);
  AC_Tmp_14 = a[0];
  AC_Tmp_15 = a[1];
  AC_Tmp_16 = a[2];
  voidBubblesort(AC_Tmp_14,AC_Tmp_15,AC_Tmp_16,3);
  return 0;
}
