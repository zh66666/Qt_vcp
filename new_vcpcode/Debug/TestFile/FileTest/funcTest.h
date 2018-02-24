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
  e_Slow  = 2
}E_Blink;


typedef struct TEG_Property
{
  INT32 Color;
  INT32 Blink;
}T_Property;

#define DATA_SIZE 8
typedef struct TEG_Message
{
  INT32      Sequence;
  T_Property Property;
  INT32      Data[DATA_SIZE];
}T_Message;

extern T_Message g_GlobalMsg;
extern INT32 g_GlobalInt;

