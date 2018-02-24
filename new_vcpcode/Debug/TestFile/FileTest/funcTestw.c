#line 1 "E:/nanche/code/vcp_code/Debug/TestFile/FileTest/funcTest.c"
#line 1 "funcTest.h"
typedef unsigned long int INT32;

typedef enum TEG_Color
{
  e_Black = 1,
  e_Green = 2,
  e_White = 3
}E_Color;

typedef enum TEG_Blink
{
  e_Rapid = 1,
  e_Slow = 2
}E_Blink;


typedef struct TEG_Property
{
  INT32 Color;
  INT32 Blink;
}T_Property;


typedef struct TEG_Message
{
  INT32 Sequence;
  T_Property Property;
  INT32 Data[ 8 ];
}T_Message;

extern T_Message g_GlobalMsg;
extern INT32 g_GlobalInt;
#line 2 "E:/nanche/code/vcp_code/Debug/TestFile/FileTest/funcTest.c"
#line 1 "IO_Interface.h"




extern long int g_USR_BufRecv1[256];
extern long int g_USR_BufRecv2[256];
extern long int g_USR_BufFetch1[256];
extern long int g_USR_BufFetch2[256];
extern long int g_USR_BufSend1[256];
extern long int g_USR_BufSend2[256];
extern long int g_USR_BufDeliver1[256];
extern long int g_USR_BufDeliver2[256];

extern void F_USR_DataRecv(long int inBufId);
extern void F_USR_DataFetch(long int inBufId);
extern void F_USR_DataSend(long int inBufId);
extern void F_USR_DataDeliver(long int inBufId);
#line 3 "E:/nanche/code/vcp_code/Debug/TestFile/FileTest/funcTest.c"
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
