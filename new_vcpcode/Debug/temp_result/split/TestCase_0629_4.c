empty int  globalVar1 = 1;

empty int  globalVar2 = 100;

empty int  globalVar3 = 1001;

empty int  globalArr1[10];

empty int  func0();

empty int  func1(int inX,int inY);

empty int  func2(int inX,int inY,int inZ);

empty int  func0()
{
  empty int  temp;

  globalVar1 = globalVar1+1;
  globalVar2 = globalVar2+2;
  temp = globalVar1+globalVar2;
  globalArr1[2] = globalVar2;
  AC_Tmp_1 = globalArr1[2];
  temp = AC_Tmp_1+globalVar1;
  return temp;
}
empty int  func1(int inX,int inY)
{
  empty int  func1Temp;

  inX = inX+1;
  inY = inY*3;
  globalArr1[0] = inX;
  AC_Tmp_1 = globalArr1[0];
  func1Temp = func1Temp+AC_Tmp_1;
  func1Temp = inX+inY;
  return func1Temp;
}
empty int  func2(int inX,int inY,int inZ)
{
  empty int  temp1,temp2,temp3;

  AC_Tmp_1 = inX>0;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    temp1 = inX+inY;
    temp2 = temp1*3;
    temp3 = temp1-temp2;
  }
  else
  {
    temp1 = func1(inY,inZ);
    temp2 = func0();
    temp3 = temp1+temp2;
  }
  globalVar1 = temp3;
  return 0;
}
empty int  main()
{
  empty int  a = 1;

  empty int  b = 2;

  empty int  c = 9999;

  empty int  d = -37;

  empty int  e = 3333;

  empty int  f = 901;

  empty int  arr[3] = { 32,43,21312};

  AC_Tmp_1 = globalVar1>0;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    AC_Tmp_2 = a+b;
    AC_Tmp_3 = AC_Tmp_2<100;
    AC_WhileSub_1 = AC_Tmp_3-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
    while(AC_Tmp_3==e_TRUE)
    {
      a = a+1;
      b = a+b;
      AC_Tmp_4 = globalVar1<10;
      AC_WhileSub_3 = AC_Tmp_4-e_TRUE;
      AC_WhileSub_4 = 0-AC_WhileSub_3;
      while(AC_Tmp_4==e_TRUE)
      {
        a = a+b;
        AC_Tmp_5 = arr[2];
        a = a+AC_Tmp_5;
        AC_Void_Func_RetVal = func2(d,e,f);
        AC_Tmp_4 = globalVar1<10;
        AC_WhileSub_3 = AC_Tmp_4-e_TRUE;
        AC_WhileSub_4 = 0-AC_WhileSub_3;
      }
      AC_Tmp_2 = a+b;
      AC_Tmp_3 = AC_Tmp_2<100;
      AC_WhileSub_1 = AC_Tmp_3-e_TRUE;
      AC_WhileSub_2 = 0-AC_WhileSub_1;
    }
    AC_Tmp_6 = arr[1];
    AC_Tmp_7 = arr[2];
    AC_Tmp_8 = AC_Tmp_6+AC_Tmp_7;
    arr[3] = AC_Tmp_8;
  }
  else
  {
    AC_Tmp_9 = c-e;
    AC_Tmp_10 = AC_Tmp_9>4000;
    AC_IfSub_3 = AC_Tmp_10-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_10==e_TRUE)
    {
      f = f-d;
      f = d*a;
      e = b/2;
    }
    else
    {
      f = 901+1;
      e = func1(a,12);
      globalVar1 = func1(globalVar2,globalVar3);
      arr[1] = 12;
    }
  }
  F_VCE_CheckVar(a,1);
  F_VCE_CheckVar(b,2);
  F_VCE_CheckVar(c,3);
  F_VCE_CheckVar(d,4);
  F_VCE_CheckVar(e,5);
  F_VCE_CheckVar(f,6);
  F_VCE_CheckVar(globalVar1,7);
  F_VCE_CheckVar(globalVar2,8);
  F_VCE_CheckVar(globalVar3,9);
  F_VCE_CheckArr(arr,0,1,10);
  F_VCE_CheckArr(arr,1,1,11);
  F_VCE_CheckArr(arr,2,1,12);
  F_VCE_CheckVar(AC_Void_Func_RetVal,0);
  return 0;
}
