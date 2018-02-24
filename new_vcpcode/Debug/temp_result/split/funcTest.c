typedef unsigned long int  INT32;

typedef int  E_Color;

typedef int  E_Blink;

typedef int  T_Property;

typedef int  T_Message;

extern int  g_GlobalMsg[11];

extern unsigned long int  g_GlobalInt;

extern long int  g_USR_BufRecv1[256];

extern long int  g_USR_BufRecv2[256];

extern long int  g_USR_BufFetch1[256];

extern long int  g_USR_BufFetch2[256];

extern long int  g_USR_BufSend1[256];

extern long int  g_USR_BufSend2[256];

extern long int  g_USR_BufDeliver1[256];

extern long int  g_USR_BufDeliver2[256];

extern int  F_USR_DataRecv(long int inBufId);

extern int  F_USR_DataFetch(long int inBufId);

extern int  F_USR_DataSend(long int inBufId);

extern int  F_USR_DataDeliver(long int inBufId);

empty unsigned long int  g_GlobalInt = 100;

empty int  g_GlobalMsg[11];

empty unsigned long int  compare(void );

empty int  set_sequence(unsigned long int inSeq);

empty int  main()
{
  empty unsigned long int  a;

  empty unsigned long int  b;

  empty unsigned long int  c;

  empty unsigned long int  i = 0;

  empty unsigned long int  z = 99;

  empty unsigned long int  L_USR_Buf[5] = { 11,22,33,44,55};

  F_USR_DataRecv(1);
  F_USR_DataRecv(2);
  g_GlobalInt = 66;
  AC_Tmp_1 = a==170;
  AC_IfSub_1 = AC_Tmp_1-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_1==e_TRUE)
  {
    a = g_USR_BufRecv1[1];
    b = L_USR_Buf[0];
    AC_Tmp_2 = a==0;
    AC_IfSub_3 = AC_Tmp_2-e_TRUE;
    AC_IfSub_4 = 0-AC_IfSub_3;
    if(AC_Tmp_2==e_TRUE)
    {
      g_GlobalMsg[1] = 1;
      b = g_USR_BufRecv1[2];
      c = L_USR_Buf[i];
      AC_Tmp_3 = b==0;
      AC_IfSub_5 = AC_Tmp_3-e_TRUE;
      AC_IfSub_6 = 0-AC_IfSub_5;
      if(AC_Tmp_3==e_TRUE)
      {
        g_GlobalMsg[2] = 1;
      }
      else
      {
        g_GlobalMsg[2] = 2;
      }
    }
    else
    {
      AC_Tmp_4 = a==1;
      AC_IfSub_7 = AC_Tmp_4-e_TRUE;
      AC_IfSub_8 = 0-AC_IfSub_7;
      if(AC_Tmp_4==e_TRUE)
      {
        g_GlobalMsg[1] = 2;
        b = g_USR_BufRecv1[2];
        AC_Tmp_5 = b==2;
        AC_IfSub_9 = AC_Tmp_5-e_TRUE;
        AC_IfSub_10 = 0-AC_IfSub_9;
        if(AC_Tmp_5==e_TRUE)
        {
          g_GlobalMsg[2] = 1;
        }
        else
        {
          g_GlobalMsg[2] = 2;
          AC_Tmp_6 = g_USR_BufRecv2[0];
          L_USR_Buf[i] = AC_Tmp_6;
          AC_Tmp_7 = b>0;
          AC_WhileSub_1 = AC_Tmp_7-e_TRUE;
          AC_WhileSub_2 = 0-AC_WhileSub_1;
          while(AC_Tmp_7==e_TRUE)
          {
            AC_Tmp_8 = i<8;
            AC_WhileSub_3 = AC_Tmp_8-e_TRUE;
            AC_WhileSub_4 = 0-AC_WhileSub_3;
            while(AC_Tmp_8==e_TRUE)
            {
              AC_Tmp_9 = i+3;
              AC_Tmp_10 = g_USR_BufRecv1[i];
              g_GlobalMsg[AC_Tmp_9] = AC_Tmp_10;
              AC_Tmp_11 = g_USR_BufRecv1[0];
              L_USR_Buf[2] = AC_Tmp_11;
              AC_Tmp_8 = i<8;
              AC_WhileSub_3 = AC_Tmp_8-e_TRUE;
              AC_WhileSub_4 = 0-AC_WhileSub_3;
            }
            AC_Tmp_7 = b>0;
            AC_WhileSub_1 = AC_Tmp_7-e_TRUE;
            AC_WhileSub_2 = 0-AC_WhileSub_1;
          }
        }
      }
      else
      {
        g_GlobalMsg[1] = 3;
        c = g_USR_BufRecv1[3];
        L_USR_Buf[3] = 66;
        AC_Tmp_12 = c==0;
        AC_IfSub_11 = AC_Tmp_12-e_TRUE;
        AC_IfSub_12 = 0-AC_IfSub_11;
        if(AC_Tmp_12==e_TRUE)
        {
          g_GlobalMsg[2] = 1;
          L_USR_Buf[4] = z;
        }
        else
        {
          g_GlobalMsg[2] = 2;
        }
      }
    }
  }
  return 0;
}
empty unsigned long int  compare(void )
{
  empty unsigned long int  result = 170;

  empty unsigned long int  i = 1;

  AC_Tmp_1 = g_USR_BufRecv1[0];
  AC_Tmp_2 = AC_Tmp_1==-1431655766;
  AC_Tmp_3 = g_USR_BufRecv2[0];
  AC_Tmp_4 = AC_Tmp_3==-1431655766;
  AC_Tmp_5 = AC_Tmp_2&&AC_Tmp_4;
  AC_IfSub_1 = AC_Tmp_5-e_TRUE;
  AC_IfSub_2 = 0-AC_IfSub_1;
  if(AC_Tmp_5==e_TRUE)
  {
    AC_Tmp_6 = i<8;
    AC_Tmp_7 = result==170;
    AC_Tmp_8 = AC_Tmp_6&&AC_Tmp_7;
    AC_WhileSub_1 = AC_Tmp_8-e_TRUE;
    AC_WhileSub_2 = 0-AC_WhileSub_1;
    while(AC_Tmp_8==e_TRUE)
    {
      AC_Tmp_9 = g_USR_BufRecv1[i];
      AC_Tmp_10 = g_USR_BufRecv2[i];
      AC_Tmp_11 = AC_Tmp_9!=AC_Tmp_10;
      AC_IfSub_3 = AC_Tmp_11-e_TRUE;
      AC_IfSub_4 = 0-AC_IfSub_3;
      if(AC_Tmp_11==e_TRUE)
      {
        result = 85;
      }
      AC_Tmp_6 = i<8;
      AC_Tmp_7 = result==170;
      AC_Tmp_8 = AC_Tmp_6&&AC_Tmp_7;
      AC_WhileSub_1 = AC_Tmp_8-e_TRUE;
      AC_WhileSub_2 = 0-AC_WhileSub_1;
    }
  }
  else
  {
    result = 85;
  }
}
empty int  set_sequence(unsigned long int inSeq)
{
  g_GlobalMsg[0] = inSeq;
  return 0;
}
