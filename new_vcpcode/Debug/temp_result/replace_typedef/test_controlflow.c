empty int  main(void )
{
  empty int  i = 10;

  empty int  a = 1;

  empty int  b = 2;

  empty int  c;

  c = a+b;
  if(a>=2)
  {
    c = a+b;
    a = b+c;
  }
  else
  {
    c = a-b;
  }
  c = a+b;
  while(i>=0)
  {
    a = a+1;
    i = i-1;
  }
  c = a+b;
  return c;
}
