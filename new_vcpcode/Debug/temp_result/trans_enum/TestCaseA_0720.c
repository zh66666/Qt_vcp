typedef struct testNode
{
  empty int  value;
  empty int  next;
  empty int  pre;
  empty int  count;
}  testNode;

empty int  globalVar1 = 1;

empty int  globalVar2 = 100;

empty int  globalVar3 = 1001;

empty int  globalArr1[100];

empty int  func0();

empty int  func1(int inX,int inY);

empty void  func2(int inX,int inY,int inZ);

empty void  structFunc(int num);

empty int  func0()
{
  empty int  temp;

  globalVar1 = globalVar1+1;
  globalVar2 = globalVar2+2;
  temp = globalVar1+globalVar2;
  globalArr1[2] = globalVar2;
  temp = globalArr1[2]+globalVar1;
  return temp;
}
empty int  func1(int inX,int inY)
{
  empty int  func1Temp;

  inX = inX+1;
  inY = inY*3;
  globalArr1[0] = inX;
  func1Temp = func1Temp+globalArr1[0];
  func1Temp = inX+inY;
  return func1Temp;
}
empty void  func2(int inX,int inY,int inZ)
{
  empty int  temp1,temp2,temp3;

  if(inX>0)
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
}
empty void  structFunc(int num)
{
  empty int  i = 0;

  empty struct testNode
  {
    empty int  value;
    empty int  next;
    empty int  pre;
    empty int  count;
  }  nodes[num];

  while(i<num)
  {
    nodes[i].value = 0;
    nodes[i].next = 0;
    nodes[i].pre = 0;
    nodes[i].count = 0;
    i = i+1;
  }
}
empty void  main()
{
  empty int  a = 1;

  empty int  b = 2;

  empty int  c = 9999;

  empty int  d = -37;

  empty int  e = 3333;

  empty int  f = 901;

  empty int  arr[3] = { 32,43,21312};

  if(globalVar1>0)
  {
    while(a+b<100)
    {
      a = a+1;
      b = a+b;
      while(globalVar1<10)
      {
        a = a+b;
        a = a+arr[2];
        func2(d,e,f);
      }
    }
    arr[3] = arr[1]+arr[2];
  }
  else
  {
    if(c-e>4000)
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
}
