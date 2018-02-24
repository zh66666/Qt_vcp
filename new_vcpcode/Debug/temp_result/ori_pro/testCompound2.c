empty void  main()
{
  empty int  a = 100;

  empty int  b = 120;

  empty int  c = 130;

  empty int  d = 140;

  if(a==0)
  {
    if(b>10)
    {
      b = 10+c;
      if(c>=101)
      {
        d = 10*b;
        if(b>=10)
        {
          c = c*a;
        }
      }
      else
      {
        b = 10;
      }
    }
  }
  else
  {
    c = b;
    if(c>=10)
    {
      d = c*a;
    }
    else
    {
      c = a;
    }
  }
}
