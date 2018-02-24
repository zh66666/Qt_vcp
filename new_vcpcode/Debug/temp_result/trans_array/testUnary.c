empty int  Test_result = 0;

empty void  main()
{
  empty int  a = -1;

  empty int  b = -2;

  empty int  c = -3;

  empty int  d = -4;

  empty int  e = -5;

  a = a+(-1);
  a = -2+a;
  b = b-(-3);
  b = -3-b;
  c = -3*c;
  c = c*-4;
  d = d/-3;
  e = e>>2;
  e = e<<2;
  Test_result = e;
  Test_result = (a+b-c)*e/4;
  return ;
}
