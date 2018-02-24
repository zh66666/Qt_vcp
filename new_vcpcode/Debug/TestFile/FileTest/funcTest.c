#include "funcTest.h"
#include "IO_Interface.h"
INT32 g_GlobalInt = 100;
T_Message g_GlobalMsg;

INT32 compare(void);
void set_sequence(INT32 inSeq);



void main()
{
  INT32 a;
  INT32 b;
  INT32 c;
  INT32 i = 0;
  INT32 z=99;
  INT32 L_USR_Buf[5]={11,22,33,44,55};

  F_USR_DataRecv(1);
  F_USR_DataRecv(2);
  g_GlobalInt=66;
  if(a == 0xAA)
  {
    a = g_USR_BufRecv1[1];
    b=L_USR_Buf[0];

    if(a == 0)
    {
      g_GlobalMsg.Property.Color = e_Black;
      b = g_USR_BufRecv1[2];
      c=L_USR_Buf[i];
      if(b == 0)
      {
        g_GlobalMsg.Property.Blink = e_Rapid;
      }
      else
      {
        g_GlobalMsg.Property.Blink = e_Slow;
      }
    }
    else if(a == 1)
    {
      g_GlobalMsg.Property.Color = e_Green;
      b = g_USR_BufRecv1[2];
      if(b == 2)
      {
        g_GlobalMsg.Property.Blink = e_Rapid;
      }
      else
      {
        g_GlobalMsg.Property.Blink = e_Slow;
        L_USR_Buf[i]=g_USR_BufRecv2[0];
        while(b > 0)
        {

          while(i < 8)
          {
            g_GlobalMsg.Data[i] = g_USR_BufRecv1[i];
            L_USR_Buf[2]=g_USR_BufRecv1[0];
          }
        }
      }
    }
    else
    {
      g_GlobalMsg.Property.Color = e_White;
      c = g_USR_BufRecv1[3];
      L_USR_Buf[3]=66;
      if(c == 0)
      {
        g_GlobalMsg.Property.Blink = e_Rapid;
        L_USR_Buf[4]=z;
      }
      else
      {
        g_GlobalMsg.Property.Blink = e_Slow;
      }
    }
  }


}

INT32 compare(void)
{
  INT32 result = 0xAA;
  INT32 i = 1;

  if((g_USR_BufRecv1[0] == 0xAAAAAAAA) && (g_USR_BufRecv2[0] == 0xAAAAAAAA))
  {
    while((i < 8) && (result == 0xAA))
    {
      if(g_USR_BufRecv1[i] != g_USR_BufRecv2[i])
      {
        result = 0x55;
      }
    }
  }
  else
  {
    result = 0x55;
  }
}

void set_sequence(INT32 inSeq)
{
  g_GlobalMsg.Sequence = inSeq;
}
