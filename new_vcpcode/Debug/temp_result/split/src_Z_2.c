empty int  func1();

empty int  func2();

empty int  func3(int a);

empty int  func4();

empty int  matrix_multi();

empty int  prime_5000();

empty int  loop_ten();

empty int  graph();

empty int  line(int x1,int y1,int x2,int y2);

empty int  initgraph(int indriver,int inmode);

empty int  setcolor(int incolor);

empty int  matrix_1[2500];

empty int  matrix_2[2500];

empty int  array[10];

empty int  final[2500];

empty int  matrix_multi()
{
  empty int  row = 0;

  empty int  col = 0;

  empty int  i = 0;

  empty int  cur_sum = 0;

  AC_Tmp_1 = row<50;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    col = 0;
    AC_Tmp_2 = col<50;
    AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
    while(AC_Tmp_2==e_TRUE)
    {
      cur_sum = 0;
      i = 0;
      AC_Tmp_3 = i<50;
      AC_WhileSub_5 = AC_Tmp_3-e_TRUE;
      AC_WhileSub_6 = 0-AC_WhileSub_5;
      while(AC_Tmp_3==e_TRUE)
      {
        AC_Tmp_4 = row*50;
        AC_Tmp_5 = AC_Tmp_4+i;
        AC_Tmp_6 = matrix_1[AC_Tmp_5];
        AC_Tmp_7 = i*50;
        AC_Tmp_8 = AC_Tmp_7+col;
        AC_Tmp_9 = matrix_2[AC_Tmp_8];
        AC_Tmp_10 = AC_Tmp_6*AC_Tmp_9;
        cur_sum = cur_sum+AC_Tmp_10;
        i = i+1;
        AC_Tmp_3 = i<50;
        AC_WhileSub_5 = AC_Tmp_3-e_TRUE;
        AC_WhileSub_6 = 0-AC_WhileSub_5;
      }
      AC_Tmp_11 = row*50;
      AC_Tmp_12 = AC_Tmp_11+col;
      final[AC_Tmp_12] = cur_sum;
      col = col+1;
      AC_Tmp_2 = col<50;
      AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
    }
    row = row+1;
    AC_Tmp_1 = row<50;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  return 0;
}
empty int  prime_5000()
{
  empty int  prime_cnt = 0;

  empty int  num = 3;

  empty int  divior = 0;

  empty int  mod = 0;

  AC_Tmp_1 = num<=5000;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    divior = num-1;
    AC_Tmp_2 = divior>=2;
    AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
    AC_WhileSub_4 = 0-AC_WhileSub_3;
    while(AC_Tmp_2==e_TRUE)
    {
      mod = num/97;
      AC_Tmp_3 = mod*97;
      mod = num-AC_Tmp_3;
      AC_Tmp_4 = mod==0;
      AC_IfSub_1 = AC_Tmp_4-e_TRUE;
      AC_IfSub_2 = 0-AC_IfSub_1;
      if(AC_Tmp_4==e_TRUE)
      {
        divior = 2;
      }
      divior = divior-1;
      AC_Tmp_2 = divior>=2;
      AC_WhileSub_3 = AC_Tmp_2-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
    }
    AC_Tmp_5 = divior==1;
    AC_IfSub_3 = AC_Tmp_5-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_5==e_TRUE)
    {
      prime_cnt = prime_cnt+1;
    }
    num = num+1;
    AC_Tmp_1 = num<=5000;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  return 0;
}
empty int  loop_ten()
{
  empty int  i = 0;

  empty int  a = 0;

  AC_Tmp_1 = i<10;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    a = array[i];
    i = i+1;
    AC_Tmp_1 = i<10;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  return 0;
}
empty int  graph()
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
  AC_Tmp_1 = i<100;
  AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
  AC_WhileSub_2 = 0-AC_WhileSub_1;
  while(AC_Tmp_1==e_TRUE)
  {
    AC_void_func_RetVal = line(x1,y1,x2,y2);
    x1 = x1+dx1;
    y1 = y1+dy1;
    x2 = x2+dx2;
    y2 = y2+dy2;
    AC_Tmp_2 = x1<=0;
    AC_Tmp_3 = x1>=639;
    AC_Tmp_4 = AC_Tmp_2||AC_Tmp_3;
    AC_IfSub_1 = AC_Tmp_4-e_TRUE;
    AC_IfSub_2 = 0-AC_IfSub_1;
    if(AC_Tmp_4==e_TRUE)
    {
      dx1 = 0-dx1;
    }
    AC_Tmp_5 = y1<=0;
    AC_Tmp_6 = y1>=479;
    AC_Tmp_7 = AC_Tmp_5||AC_Tmp_6;
    AC_IfSub_3 = AC_Tmp_7-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_7==e_TRUE)
    {
      dy1 = 0-dy1;
    }
    AC_Tmp_8 = x2<=0;
    AC_Tmp_9 = x2>=639;
    AC_Tmp_10 = AC_Tmp_8||AC_Tmp_9;
    AC_IfSub_5 = AC_Tmp_10-e_TRUE;
    AC_IfSub_6 = 0-AC_IfSub_5;
    if(AC_Tmp_10==e_TRUE)
    {
      dx2 = 0-dx2;
    }
    AC_Tmp_11 = y2<=0;
    AC_Tmp_12 = y2>=479;
    AC_Tmp_13 = AC_Tmp_11||AC_Tmp_12;
    AC_IfSub_7 = AC_Tmp_13-e_TRUE;
    AC_IfSub_8 = 0-AC_IfSub_7;
    if(AC_Tmp_13==e_TRUE)
    {
      dy2 = 0-dy2;
    }
    count = count+1;
    AC_Tmp_14 = count>400;
    AC_IfSub_9 = AC_Tmp_14-e_TRUE;
    AC_IfSub_10 = 0-AC_IfSub_9;
    if(AC_Tmp_14==e_TRUE)
    {
      AC_void_func_RetVal = setcolor(color);
    }
    i = i+1;
    AC_Tmp_1 = i<100;
    AC_WhileSub_1 = AC_Tmp_1-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
  }
  F_VCE_CheckVar(AC_void_func_RetVal,0);
  return 0;
}
empty int  line(int x1,int y1,int x2,int y2)
{
  empty int  a[4];

  a[0] = x1;
  a[1] = x2;
  a[2] = y1;
  a[3] = y2;
  return 0;
}
empty int  initgraph(int indriver,int inmode)
{
  ;
  return 0;
}
empty int  setcolor(int incolor)
{
  empty int  a = 0;

  a = incolor;
  return 0;
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
