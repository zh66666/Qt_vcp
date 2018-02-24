empty void  main()
{
  empty int  a = 2;

  empty int  b = 4;

  empty int  c = 7;

  empty int  w = 0;

  empty int  x = 0;

  empty int  y = 0;

  empty int  z;

  w = a+b;
  F_VCE_CheckVar(w,1);
  x = w-c;
  F_VCE_CheckVar(x,2);
  y = a*x;
  F_VCE_CheckVar(y,3);
  z = y/2;
  F_VCE_CheckVar(z,4);
  w = w+1;
  F_VCE_CheckVar(w,5);
  x = 10-11;
  F_VCE_CheckVar(x,6);
  y = 5*b;
  F_VCE_CheckVar(y,7);
  z = z<<3;
  F_VCE_CheckVar(z,8);
  z = z>>3;
  F_VCE_CheckVar(z,9);
}
