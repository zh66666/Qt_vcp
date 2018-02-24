typedef unsigned char  NIBBLE_4;

typedef unsigned char  BYTE_8;

/***************************************************************************/
typedef signed short  SHORT_16;

typedef unsigned short  uSHORT_16;

typedef signed long  NUMERIC_32;

typedef unsigned long  uNUMERIC_32;

/* 8bit */
/* 16bit */
typedef int  BOOLEAN1;

/* 32bit */
struct structVar1
{
    int  sVar1_1;
    int  sVar1_2;
    signed long  sVar1_3;
    signed long  sVar1_4;
    int  sVar1_5;
    int  sVar1_6;
    signed long  sVar1_7;
    signed long  sVar1_8;
} ;

/* in VC */
struct structVar2
{
    struct structVar1  sVar2_1;
    int  sVar2_2;
    char  sVar2_3;
} ;

typedef int  casco_46;

struct casco_47
{
    int  casco_48;
    int  casco_49;
    signed long  casco_50;
    signed long  casco_51;
    int  casco_52;
    int  casco_53;
    signed long  casco_54;
    signed long  casco_55;
} ;

struct casco_56
{
    int  casco_57;
    int  casco_58;
    char  casco_59;
} ;

extern struct casco_47  casco_60;

extern struct casco_56  casco_61[32];

extern void  casco_5(void );

extern int  casco_64(void );

extern int  casco_65(void );

extern int  casco_66(void );

extern signed long  casco_67(void );

extern signed long  casco_68(void );

extern int  casco_69(void );

struct casco_19
{
    signed long  casco_21;
    signed long  casco_22;
    signed long  casco_23;
    signed long  casco_24;
    signed long  casco_25;
    signed long  casco_26;
    signed long  casco_27;
    signed long  casco_28;
    int  casco_29;
    signed long  casco_30;
    int  casco_31;
} ;

extern struct casco_19  casco_20;

extern void  casco_6(void );

extern int  casco_Cal(int a,int b);

struct casco_7
{
    int  casco_10;
    int  casco_11;
    int  casco_12;
    int  casco_13;
    int  casco_14;
    int  casco_15;
    int  casco_16;
    int  casco_17;
    int  casco_18;
} ;

extern struct casco_7  casco_8;

struct casco_19  casco_20;

void  casco_6(void )
{
    int  local_zero = 10243;

    int  filterQvaTime;

    if(casco_60.casco_48==-1)
    {
        if(casco_20.casco_25<10273)
            local_zero = -1;

        else
            local_zero = 0;

        if(casco_8.casco_18==0)
        {
            casco_20.casco_30 = casco_20.casco_30+1;
            casco_20.casco_31 = -1;
        }
        else
        {
            if(local_zero==-1)
            {
                if(casco_8.casco_17==0)
                {
                    casco_20.casco_30 = casco_20.casco_30+1;
                    if(casco_20.casco_30>15)
                    {
                        filterQvaTime = casco_Cal(local_zero,local_zero);
                    }
                    else
                    {
                        filterQvaTime = casco_Cal(local_zero,1234);
                    }
                    if(filterQvaTime==-1)
                    {
                        casco_20.casco_29 = -1;
                    }
                }
                else
                    casco_20.casco_30 = 0;

            }
            else
            {
                if(casco_8.casco_17==-1)
                {
                    casco_20.casco_30 = casco_20.casco_30+1;
                    if(casco_20.casco_30>15)
                    {
                        filterQvaTime = -1;
                    }
                    else
                    {
                        filterQvaTime = 0;
                    }
                    if(filterQvaTime==-1)
                    {
                        casco_20.casco_29 = -1;
                    }
                }
                else
                    casco_20.casco_30 = 0;

            }
        }
    }
    return ;
}
int  casco_Cal(int a,int b)
{
    int  result,base,i;

    base = 5023;
    i = 0;
    result = base<<7;
    result = result>>7;
    result = base>>5;
    result = result<<5;
    result = (result+a)<<3;
    result = b<<3;
    while(i<=2)
    {
        result = result<<3;
        result = base>>3;
        i = i+1;
    }
    return result;
}
