empty void  main()
{
  empty int  a = 100;

  empty int  b = 120;

  empty int  c = 130;

  empty int  d = 140;

  while(a>0)
  {
    if(a<=30)
    {
      c = d*10;
      b = 130;
      while(b<=100)
      {
        c = c+10;
        b = b-1;
      }
    }
    else
    {
      c = 10;
      while(c>=0)
      {
        d = 10;
        b = d<<2;
        c = c-1;
      }
    }
    a = a-2;
  }
}
