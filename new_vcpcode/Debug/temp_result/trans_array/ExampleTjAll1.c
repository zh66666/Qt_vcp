empty int  Test_result = 0;

empty void  main()
{
  empty int  x = 1;

  empty int  y = -1;

  empty int  z = 0;

  empty int  m = 1;

  empty int  n = 5;

  empty int  i = 0;

  empty int  j = 0;

  i = 1;
  j = i;
  Test_result = j;
  x = x+1;
  x = 1+x;
  x = x+x;
  y = y-1;
  y = -2-y;
  y = y-2;
  y = x+y;
  m = m*-2;
  m = -3*m;
  m = 1*10;
  m = m*n;
  n = n/2;
  z = ((x+1)/2+(y-2)*y)*3;
  z = ((-1+x)/3+(-2+y)*y)*-3;
  z = ((i+2)>>2)+(j+100)<<2;
  Test_result = (x+y+z)*m/2-i*n+j;
}
